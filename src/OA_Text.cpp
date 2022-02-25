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
			fprintf(stderr, "�ؽ�ó�� ���� ���ڰ� �Էµ��� �ʾҽ��ϴ�.\n");
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
			fprintf(stderr, "%s: ��Ʈ ������ ã�� �� �����ϴ�.\n", file.c_str());
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
			fprintf(stderr,"�ؽ�ó�� ���� ���ڰ� �Էµ��� �ʾҽ��ϴ�.\n");
			return nullptr;
		}
		if (list.find(name) != list.end()) {
			return list[name];
		}
		stbtt_fontinfo info;
		int c = stbtt_InitFont(&info, dat, 0);
		if (c == 0) {
			fprintf(stderr,"��Ʈ ������ ��ȿ���� �ʽ��ϴ�.\n");
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

	Font::Font(void* info, const std::set<oachar>& vset, float resolution) :resolution(resolution * 0.8f) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		stbtt_fontinfo* fi = (stbtt_fontinfo*)info;
		for (oachar c : vset) {
			charTex ct;
			float yScale = stbtt_ScaleForPixelHeight(fi, resolution);
			int width, height, bearingX, bearingY, advance;
			unsigned char* bitmap = stbtt_GetCodepointBitmap(fi, 0, yScale, c, &width, &height, &bearingX, &bearingY);
			if (!bitmap) {
				fprintf(stderr, "���: �����ڵ� 0x%x ���ڴ� ��Ʈ�� ��� �ε���� �ʽ��ϴ�.\n", c);
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

	void Font::draw(const oastring& content, const mat4& group, const std::vector<vec2>& lineXY, const vec4& color) {
		program2["isText"] = true;
		program2["nopiv"] = true;
		program2["color"] = color;
		program2["useFull"] = true;
		program2["textGroup"] = group;
		cdraw(content, lineXY, color);
	}

	void Font::draw(const oastring& content, const vec4& group, const std::vector<vec2>& lineXY, AlignH ha, AlignV va, const vec2& center, float size, const vec4& color) {
		constexpr float BASE_SIZE = 1.0f / 1024;
		size *= BASE_SIZE;
		program2["isText"] = true;
		program2["nopiv"] = true;
		program2["color"] = color;
		program2["useFull"] = true;
		float hf, vf;
		switch (ha)
		{
		case AlignH::LEFT:
			hf = 0;
			break;
		case AlignH::RIGHT:
			hf = 1;
			break;
		default:
			hf = 0.5f;
		}
		switch (va)
		{
		case AlignV::TOP:
			vf = 1;
			break;
		case AlignV::BOTTOM:
			vf = 0;
			break;
		default:
			vf = 0.5f;
		}
		vec2 thisCenter(group.left + group.width * hf, group.down + group.height * vf);
		thisCenter *= size;
		vec3 tl(center - thisCenter); tl.z = 0;
		program2["textGroup"] = mat4::translate(tl) * mat4::scale(size);
		cdraw(content, lineXY, color);
	}

	void Font::draw(const oastring& content, const vec4& group, const std::vector<vec2>& lineXY, const mat4& transform, AlignH ha, AlignV va, const vec2& center, float size, const vec4& color) {
		constexpr float BASE_SIZE = 1.0f / 1024;
		size *= BASE_SIZE;
		program2["isText"] = true;
		program2["nopiv"] = true;
		program2["color"] = color;
		program2["useFull"] = true;
		float hf, vf;
		switch (ha)
		{
		case AlignH::LEFT:
			hf = 0;
			break;
		case AlignH::RIGHT:
			hf = 1;
			break;
		default:
			hf = 0.5f;
		}
		switch (va)
		{
		case AlignV::TOP:
			vf = 1;
			break;
		case AlignV::BOTTOM:
			vf = 0;
			break;
		default:
			vf = 0.5f;
		}
		vec2 thisCenter(group.left + group.width * hf, group.down + group.height * vf);
		thisCenter *= size;
		vec3 tl(center - thisCenter); tl.z = 0;
		program2["textGroup"] = transform * mat4::translate(tl) * mat4::scale(size);
		cdraw(content, lineXY, color);
	}

	float Font::parseSize(const oastring& str, int start) {	// start�� ���� �ڸ��� ����Ű���� �־���
		if (str[start + 1] == u'.' && isdigit(str[start]) && isdigit(str[start + 2]) && isdigit(str[start + 3])) {
			return (float)(str[start] - u'0') + (str[start + 2] - u'0') * 0.1f + (str[start + 3] - u'0') * 0.01f;
		}
		return -1;
	}

	vec4 Font::parseColor(const oastring& str, int start) {	// start�� 8�ڸ� �� ó���� ����Ű���� �־���
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

	vec4 Font::getRectNLine(const oastring& content, std::vector<vec2>& lineXY, AlignH align, float rowGap) {
		lineXY.clear();
		vec4 totalLDWH;	// ���� ���簢�� ��ȯ��
		float curW = 0, curH = resolution;
		float curx = 1, cury = 1;
		// ���̽� ���簢���� xy [-0.5,0.5] ����, z=0
		// �� ���� ����(px)�� ���
		int charCount = (int)content.size();
		bool regular = false;	// ������ ù ���ڰ� ���� ������ ���� ���
		for (int i = 0; i < charCount; i++) {
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
					if (content[i + 1] == u'x') {
						float tempx = parseSize(content, i + 2);
						if (tempx != -1) curx = tempx;
					}
					else if (content[i + 1] == u'y') {
						float tempy = parseSize(content, i + 2);
						if (tempy != -1) {
							cury = tempy;
							if (!regular || (curH < resolution * cury)) curH = resolution * cury;
						}
					}
					else if (content[i + 1] == u'a') {
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
		// ���κ� ù ������ x, y �������� ���Ѵ�(��ü������ ����)
		switch (align)
		{
		case AlignH::CENTER:
			for (vec2& xy : lineXY) xy.x = (totalLDWH.width - xy.x) / 2;
			break;
		case AlignH::LEFT:
			for (vec2& xy : lineXY) xy.x = 0;
			break;
		case AlignH::RIGHT:
			for (vec2& xy : lineXY)xy.x = totalLDWH.width - xy.x;
			break;
		}

		return totalLDWH;
	}

	void Font::cdraw(const oastring& content, const std::vector<vec2>& lineXY, const vec4& color) {
		ppMesh rect = Mesh::get("rect");
		int line = 0;
		float curW = lineXY[0].x;
		float curH = lineXY[0].y;
		float curx = 1, cury = 1;
		float curScale = 1;
		program2.bind(**rect);
		int charCount = (int)content.size();
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
					if (content[i + 1] == u'x') {
						float tempx = parseSize(content, i + 2);
						if (tempx > -1) curx = tempx;
					}
					else if (content[i + 1] == u'y') {
						float tempy = parseSize(content, i + 2);
						if (tempy != -1) cury = tempy;
					}
					else if (content[i + 1] == u'a') {
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
					if (r[0] != -1) program2["color"] = r * color;
				}
				i += 8;
				continue;
			default:
				auto t = txs.find(c);
				if (t == txs.end()) continue;
				const charTex& ct = t->second;
				program2["transform"] = mat4::r2r(vec4(curW, curH - ct.size[1] - ct.bearing[1], ct.size[0] * curx, ct.size[1] * cury), -0.9f);
				program2.texture(ct.id);
				program2.draw();
				curW += ct.advance * curx;
			}
		}
	}

	oastring Font::cutLine(const oastring& content, float maxWidth) {
		oastring ret;
		float curW = 0, curx = 1;
		int charCount = (int)content.size();
		for (int i = 0; i < charCount; i++) {
			oachar c = content[i];
			switch (c)
			{
			case '\n':
				curW = 0;
				ret += c;
				continue;
			case '\a':
				if (i + 5 < charCount) {
					if (content[i + 1] == u'x' || content[i + 1] == u'a') {
						float tempx = parseSize(content, i + 2);
						if (tempx != -1) curx = tempx;
					}
					ret += content.substr(i, 6);
				}
				i += 5;
				continue;
			case '\b':
				if (i + 8 < charCount) {
					ret += content.substr(i, 9);
				}
				i += 8;
				continue;
			default:
				auto t = txs.find(c);
				if (t == txs.end()) continue;
				const charTex& ct = t->second;
				curW += (ct.bearing[0] + ct.size[0]) * curx;
				if (curW > maxWidth) {
					if (ret.size()) ret += u'\n';
					curW = (ct.bearing[0] + ct.size[0]) * curx;
				}
				ret += c;
				break;
			}
		}
		return ret;
	}
}
