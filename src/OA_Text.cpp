/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include <cstdio>
#include <cstdlib>

#include "externals/gl/glad/glad.h"
#include "OA_Text.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "externals/stb_truetype.h"
#include "OA_Shader.h"

extern onart::Shader program2;

namespace onart {

	std::map<std::string, Font*> Font::list;

	Font* Font::load(const std::string& file, const std::vector<oachar>& v, const std::string& name) {
		if (v.size() == 0) {
			fprintf(stderr, "텍스처를 만들 문자가 입력되지 않았습니다.\n");
			return nullptr;
		}
		std::string fn(name);
		if (fn.size() == 0) {
			fn = file;
		}
		if (list.find(fn) != list.end()) {
			return list[fn];
		}
		FILE* fp;
		fopen_s(&fp, file.c_str(), "rb");
		if (!fp) {
			fprintf(stderr, "%s: 폰트 파일을 찾을 수 없습니다.\n", file.c_str());
			return nullptr;
		}
		fseek(fp, 0, SEEK_END);
		int sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		unsigned char* dat = (unsigned char*)malloc(sz);
		Font* ret = load(dat, name, v);
		free(dat);
		return ret;
	}

	Font* Font::load(const unsigned char* dat, const std::string& name, const std::vector<oachar>& v) {
		if (v.size() == 0) {
			fprintf(stderr,"텍스처를 만들 문자가 입력되지 않았습니다.\n");
			return nullptr;
		}
		if (list.find(name) != list.end()) {
			return list[name];
		}
		stbtt_fontinfo info;
		int c = stbtt_InitFont(&info, dat, 0);
		if (c == 0) {
			fprintf(stderr,"폰트 파일이 유효하지 않습니다.\n");
			return nullptr;
		}

		std::set<oachar> vset(v.begin(), v.end());
		return list[name] = new Font(&info, vset);
	}

	void Font::drop(const std::string& name) {
		if (list.find(name) != list.end()) {
			delete list[name];
			list.erase(name);
		}
	}

	Font* Font::get(const std::string& name) {
		if (list.find(name) != list.end()) {
			return list[name];
		}
		return nullptr;
	}

	Font::~Font() {
		for (auto& t : txs) {
			glDeleteTextures(1, &t.second.id);
		}
	}

	Font::Font(void* info, const std::set<oachar>& vset) {
		stbtt_fontinfo* fi = (stbtt_fontinfo*)info;
		for (oachar c : vset) {
			charTex ct;
			float yScale = stbtt_ScaleForPixelHeight(fi, OA_FONT_RESOLUTION);
			unsigned char* bitmap = stbtt_GetCodepointBitmap(fi, 0, yScale, c, &ct.size[0], &ct.size[1], &ct.bearing[0], &ct.bearing[1]);
			if (!bitmap) {
				fprintf(stderr, "경고: 유니코드 0x%x 문자는 폰트에 없어서 로드되지 않습니다.\n", c);
				continue;
			}
			unsigned tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ct.size[0], ct.size[1], 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
			stbtt_FreeBitmap(bitmap, fi->userdata);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbtt_GetCodepointHMetrics(fi, c, &ct.advance, nullptr);
			ct.bearing[1] *= -1;
			ct.advance = int((float)ct.advance * yScale);
			txs[c] = std::move(ct);
		}
	}

	void Font::draw(const oastring& content, const vec4& ldwh, Align align, bool fullFit) {
		
	}
}
