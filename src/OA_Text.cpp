/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cctype>

#include "externals/gl/glad/glad.h"
#include "OA_Text.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "externals/stb_truetype.h"
#include "OA_Shader.h"
#include "OA_Vertex.h"

extern onart::Shader program2;

namespace onart {

	std::map<std::string, Font*> Font::list;

	Font* Font::load(const std::string& file, const std::vector<oachar>& v, float resolution, const std::string& name) {
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
		if (dat) {
			fread(dat, 1, sz, fp);
		}
		fclose(fp);
		Font* ret = load(dat, name, v, resolution);
		free(dat);
		return ret;
	}

	Font* Font::load(const unsigned char* dat, const std::string& name, const std::vector<oachar>& v, float resolution) {
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
		return list[name] = new Font(&info, vset, resolution);
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

	Font::Font(void* info, const std::set<oachar>& vset, float resolution) :resolution(resolution*0.9f) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		stbtt_fontinfo* fi = (stbtt_fontinfo*)info;
		for (oachar c : vset) {
			charTex ct;
			float yScale = stbtt_ScaleForPixelHeight(fi, resolution);
			int width, height, bearingX, bearingY, advance;
			unsigned char* bitmap = stbtt_GetCodepointBitmap(fi, 0, yScale, c, &width, &height, &bearingX, &bearingY);
			if (!bitmap) {
				fprintf(stderr, "경고: 유니코드 0x%x 문자는 폰트에 없어서 로드되지 않습니다.\n", c);
				continue;
			}
			unsigned tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
			stbtt_FreeBitmap(bitmap, fi->userdata);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbtt_GetCodepointHMetrics(fi, c, &advance, nullptr);
			ct.bearing[0] = (float)bearingX; ct.bearing[1] = (float)bearingY;
			ct.size[0] = (float)width; ct.size[1] = (float)height;
			ct.advance = (float)advance * yScale;
			ct.id = tex;
			txs[c] = std::move(ct);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	void Font::draw(const oastring& content, const vec4& ldwh, Align align, bool fullFit, float rowGap) {
		program2["isText"] = true;
		program2["nopiv"] = true;
		program2["color"] = vec4(1);
		program2["useFull"] = true;

		vec4 totalLDWH;	// 최종 직사각형 변환용
		float curW = 0, curH = resolution;
		float curx = 1, cury = 1;
		std::vector<vec2> lineXY;	// 각 행의 폭과 밑줄 지점을 담았다가, 폭은 나중에 행 시작 지점으로 변경
		// 베이스 정사각형은 xy [-0.5,0.5] 범위, z=0
		// 각 라인 길이(px)를 잰다
		int charCount = (int)content.size();
		bool regular = false;	// 라인의 첫 글자가 아직 나오지 않은 경우
		for (int i = 0; i < charCount; i++) {	// 이 부분은 매번 수행할 필요 없음(totalLDWH만 도출되면 됨 -> 텍스트 개체를 만들어서 스트링 혹은 목표 직사각형이 변할 때마다 행렬 변경하도록)
			oachar c = content[i];
			switch (c)
			{
			case '\n':
				if (curW > totalLDWH.width)totalLDWH.width = curW;
				lineXY.push_back(vec2(curW, -totalLDWH.height - curH));
				totalLDWH.height += rowGap * curH;
				curH = resolution * cury;
				curW = 0;
				regular = false;
				continue;
			case '\a':
				if (i + 5 < charCount) {
					if (content[i + 1] == u'x' || content[i + 1] == u'X') {
						float tempx = parseSize(content, i + 2);
						if (tempx != -1) curx = tempx;
					}
					else if (content[i + 1] == u'y' || content[i + 1] == u'Y') {
						float tempy = parseSize(content, i + 2);
						if (tempy != -1) { 
							cury = tempy; 
							if (!regular || (curH < resolution * cury)) curH = resolution * cury;
						}
					}
					else if (content[i + 1] == u'a' || content[i + 1] == u'A') {
						float tempxy = parseSize(content, i + 2);
						if (tempxy != -1) {
							curx = tempxy;
							cury = tempxy;
							if (!regular || (curH < resolution * cury)) curH = resolution * cury;
						}
					}
				}
				i += 5;
				continue;
			case '\b':
				i += 8;
				continue;
			default:
				auto t = txs.find(c);
				if (t == txs.end()) continue;
				const charTex& ct = t->second;
				curW += (ct.bearing[0] + ct.size[0]) * curx;
				regular = true;
				break;
			}
		}
		if (curW > totalLDWH.width) totalLDWH.width = curW;
		totalLDWH.height += curH;
		totalLDWH.down = -totalLDWH.height;
		lineXY.push_back(vec2(curW, totalLDWH.down));
		// 전체 문장에 적용될 변환을 정한다: 매번 할 필요 없음
		vec4 targLDWH(ldwh);
		if (!fullFit) {
			float r = totalLDWH.width / totalLDWH.height;
			if (targLDWH.width < targLDWH.height * r) {	// 세로선을 맞출 것
				targLDWH.down += (targLDWH.height - targLDWH.width / r) / 2;
				targLDWH.height = targLDWH.width / r;
			}
			else {	// 가로선을 맞출 것
				targLDWH.left += (targLDWH.width - targLDWH.height * r) / 2;
				targLDWH.width = targLDWH.height * r;
			}
		}
		program2["textGroup"] = mat4::r2r(totalLDWH, targLDWH);
		// 라인별 첫 글자의 x, y 오프셋을 정한다(전체스케일 제외)
		switch (align)
		{
		case Align::CENTER:
			for (vec2& xy : lineXY) xy.x = (totalLDWH.width - xy.x) / 2;
			break;
		case Align::LEFT:
			for (vec2& xy : lineXY) xy.x = 0;
			break;
		case Align::RIGHT:
			for (vec2& xy : lineXY)xy.x = totalLDWH.width - xy.x;
			break;
		}
		Mesh** rect = Mesh::get("rect");	// 처음부터 여기까지는 매번 할 필요 없음
		// 그린다
		int line = 0;
		curW = lineXY[0].x;
		curH = lineXY[0].y;
		curx = cury = 1;
		float curScale = 1;
		program2.bind(**rect);
		for (int i = 0; i < charCount; i++) {
			oachar c = content[i];
			switch (c)
			{
			case '\n':
				curW = lineXY[++line].x;
				curH = lineXY[line].y;
				continue;
			case '\a':
				if (i + 5 < charCount) {
					if (content[i + 1] == u'x' || content[i + 1] == u'X') {
						float tempx = parseSize(content, i + 2);
						if (tempx > -1) curx = tempx;
					}
					else if (content[i + 1] == u'y' || content[i + 1] == u'Y') {
						float tempy = parseSize(content, i + 2);
						if (tempy != -1) cury = tempy;
					}
					else if (content[i + 1] == u'a' || content[i + 1] == u'A') {
						float tempxy = parseSize(content, i + 2);
						if (tempxy != -1) {
							curx = tempxy;
							cury = tempxy;
						}
					}
				}
				i += 5;
				continue;
			case '\b':
				if (i + 8 < charCount) {
					vec4 r = parseColor(content, i + 1);
					if (r[0] != -1)program2["color"] = r;
				}
				i += 8;
				continue;
			default:
				auto t = txs.find(c);
				if (t == txs.end()) continue;
				const charTex& ct = t->second;
				program2["transform"] = mat4::r2r(vec4(curW, curH - ct.size[1] - ct.bearing[1], ct.size[0] * curx, ct.size[1] * cury), -1.0f);
				program2.texture(ct.id);
				program2.draw();
				curW += ct.advance * curx;
			}
		}
	}

	float Font::parseSize(const oastring& str, int start) {	// start는 정수 자리를 가리키도록 주어짐
		if (str[start + 1] == u'.' && isdigit(str[start]) && isdigit(str[start + 2]) && isdigit(str[start + 3])) {
			return (float)(str[start] - u'0') + (str[start + 2] - u'0') * 0.1f + (str[start + 3] - u'0') * 0.01f;
		}
		return -1;
	}

	vec4 Font::parseColor(const oastring& str, int start) {	// start는 8자리 중 처음을 가리키도록 주어짐
		char* er = nullptr;
		char* hx = new char[9];	hx[8] = 0;
		char* p = hx;
		for (int i = start; i < start + 8; i++, p++) {
			*p = (char)str[i];
		}
		unsigned long clr = strtoul(hx, &er, 16);
		delete[] hx;
		if (*er) return -1;
		vec4 v;
		v.r = (float)((clr >> 24) & 0xff);
		v.g = (float)((clr >> 16) & 0xff);
		v.b = (float)((clr >> 8) & 0xff);
		v.a = (float)(clr & 0xff);
		return v /= 255;
	}
}
