/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "externals/gl/glad/glad.h"

#include "OA_Shader.h"
#include "OA_Vertex.h"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <string>
#include <map>

namespace onart {

	Shader* Shader::usingShader = nullptr;

	void Shader::initWithFile(const char* vert, const char* frag) {
		FILE* fp;
		fopen_s(&fp, vert, "rb");
		if (!fp) {
			fprintf(stderr, "셰이더 파일이 없습니다: %s\n", vert);
			return;
		}
		auto size = (size_t)std::filesystem::file_size(vert);
		char* vertScript = new char[size + 1];
		fread_s(vertScript, size, 1, size, fp);
		vertScript[size] = '\0';
		fclose(fp);

		fopen_s(&fp, frag, "rb");
		if (!fp) {
			fprintf(stderr, "셰이더 파일이 없습니다: %s\n", frag);
			delete[] vertScript;
			return;
		}
		size = (size_t)std::filesystem::file_size(frag);
		char* fragScript = new char[size + 1];
		fread_s(fragScript, size, 1, size, fp);
		fragScript[size] = '\0';
		fclose(fp);
		initWithMemory(vertScript, fragScript);
		delete[] vertScript, fragScript;
	}

	void Shader::initWithMemory(const char* vert, const char* frag) {
		id = glCreateProgram();
		
		unsigned vs = compileScript(vert, GL_VERTEX_SHADER);
		unsigned fs = compileScript(frag, GL_FRAGMENT_SHADER);

		if (vs == 0 || fs == 0) { 
			id = 0;
			return;
		}

		glAttachShader(id, vs);
		glAttachShader(id, fs);
		
		glLinkProgram(id);
		validateShaderProgram();
		
		// fixed sampler2d
		use();
		uniform("surface", 0);
		uniform("alphaMask", 1);
		uniform("bumpMap", 2);
		uniform("surface2", 3);
		uniform("view", mat4());
	}

	unsigned Shader::compileScript(const char* script, unsigned type) {
		unsigned prog = glCreateShader(type);
		glShaderSource(prog, 1, &script, nullptr);
		glCompileShader(prog);
		int buf;
		glGetShaderiv(prog, GL_COMPILE_STATUS, &buf);
		if (buf != GL_TRUE) {
			printf("셰이더 컴파일 오류가 있습니다.\n");
			glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &buf);
			if (buf > 0 && buf < 4096) {
				char log[4096];
				int length;
				glGetShaderInfoLog(prog, buf, &length, log);
				printf("%s\n", log);
			}
			return 0;
		}
		return prog;
	}

	bool Shader::validateShaderProgram() {
		constexpr int MAX_LOG = 4096;
		static char msg[MAX_LOG] = { 0, };
		int logLen;
		glGetProgramInfoLog(id, MAX_LOG, &logLen, msg);
		if (logLen > 1 && logLen <= MAX_LOG) {
			printf("셰이더 %d: %s\n", id, msg);
		}

		int linkStatus;
		glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) { 
			glDeleteProgram(id);
			id = 0;
			return false; 
		}

		glValidateProgram(id);
		glGetProgramInfoLog(id, MAX_LOG, &logLen, msg);
		if (logLen > 1 && logLen <= MAX_LOG) {
			printf("셰이더 %d: %s\n", id, msg);
		}

		int valStatus;
		glGetProgramiv(id, GL_VALIDATE_STATUS, &valStatus);
		if (valStatus != GL_TRUE) {
			glDeleteProgram(id);
			id = 0;
			return false;
		}

		return true;
	}

	void Shader::uniform(const char* name, int i) {
		use();
		int uloc = getUloc(name);
		glUniform1i(uloc, i);
	}

	void Shader::uniform(const char* name, float f) {
		use();
		int uloc = getUloc(name);
		glUniform1f(uloc, f);
	}

	void Shader::uniform(const char* name, bool b) {
		use();
		int uloc = getUloc(name);
		glUniform1i(uloc, b);
	}

	void Shader::uniform(const char* name, const vec2& v2) {
		use();
		int uloc = getUloc(name);
		glUniform2fv(uloc, 1, v2);
	}

	void Shader::uniform(const char* name, const vec3& v3) {
		use();
		int uloc = getUloc(name);
		glUniform3fv(uloc, 1, v3);
	}

	void Shader::uniform(const char* name, const vec4& v4) {
		use();
		int uloc = getUloc(name);
		glUniform4fv(uloc, 1, v4);
	}

	void Shader::uniform(const char* name, const ivec2& v2) {
		use();
		int uloc = getUloc(name);
		glUniform2iv(uloc, 1, v2);
	}

	void Shader::uniform(const char* name, const ivec3& v3) {
		use();
		int uloc = getUloc(name);
		glUniform3iv(uloc, 1, v3);
	}

	void Shader::uniform(const char* name, const ivec4& v4) {
		use();
		int uloc = getUloc(name);
		glUniform4iv(uloc, 1, v4);
	}

	void Shader::uniform(const char* name, const mat4& m4) {
		use();
		int uloc = getUloc(name);
		glUniformMatrix4fv(uloc, 1, GL_TRUE, m4);
	}

	void Shader::uniform(const char* name, size_t index, bool b) {
		use();
		int uloc = getUloc(name, index);
		glUniform1i(uloc, b);
	}

	void Shader::uniform(const char* name, size_t index, int b) {
		use();
		int uloc = getUloc(name, index);
		glUniform1i(uloc, b);
	}

	void Shader::uniform(const char* name, size_t index, float b) {
		use();
		int uloc = getUloc(name, index);
		glUniform1f(uloc, b);
	}

	void Shader::uniform(const char* name, size_t index, const vec2& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform2fv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const vec3& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform3fv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const vec4& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform4fv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const ivec2& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform2iv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const ivec3& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform3iv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const ivec4& b) {
		use();
		int uloc = getUloc(name, index);
		glUniform4iv(uloc, 1, b);
	}

	void Shader::uniform(const char* name, size_t index, const mat4& b) {
		use();
		int uloc = getUloc(name, index);
		glUniformMatrix4fv(uloc, 1, GL_TRUE, b);
	}

	void Shader::texture(unsigned tex, TexCode idx) {
		use();
		int i = (int)idx;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	void Shader::use() {
		if (id == 0) {
			fprintf(stderr, "%p: 사용할 수 없는 셰이더 프로그램입니다.\n", this);
			return;
		}
		if (usingShader != this) { 
			glUseProgram(id);
			usingShader = this;
		}
	}

	void Shader::draw(Mesh& m) const {
		glBindVertexArray(m.getID());
		glDrawElements(GL_TRIANGLES, m.getLength(), GL_UNSIGNED_INT, nullptr);
	}

	void Shader::bind(Mesh& m) {
		use();
		glBindVertexArray(m.getID());
		recentLen = m.getLength();
	}

	void Shader::draw(unsigned begin, unsigned count) {
		use();
		if (count == unsigned(-1)) { count = recentLen - begin; }
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(begin * sizeof(unsigned)));
	}

	int Shader::getUloc(const char* name) {
		auto u = uLocs.find(name);
		if (u != uLocs.end()) return u->second;
		int uloc = glGetUniformLocation(id, name);
		if (uloc > -1) return uLocs[name] = uloc;
		return -1;
	}

	int Shader::getUloc(const char* name, size_t index) {
		auto u = uaLocs.find(name);
		if (u != uaLocs.end()) {
			if (u->second.size() <= index) return -1;
			return u->second[index];
		}
		int uloc;
		std::vector<int> v;
		std::string stName(name);
		stName += '[';
		int i = 0;
		while(true) {
			uloc = glGetUniformLocation(id, (stName + std::to_string(i++) + ']').c_str());
			if (uloc > -1) v.push_back(uloc);
			else break;
		}
		if (v.size() <= index) uloc = -1;
		else uloc = v[index];
		uaLocs[name] = std::move(v);
		return uloc;
	}

	Shader::_uniformSender_& Shader::operator[](const char* name) {
		use();
		__uniformSender.loc = getUloc(name);
		__uniformSender.point = name;
		return __uniformSender;
	}

	void Shader::_uniformSender_::operator=(int i) {
		glUniform1i(loc, i);
	}

	void Shader::_uniformSender_::operator=(float f) {
		glUniform1f(loc, f);
	}

	void Shader::_uniformSender_::operator=(bool i) {
		glUniform1i(loc, i);
	}

	void Shader::_uniformSender_::operator=(const vec2& v) {
		glUniform2fv(loc, 1, v);
	}
	
	void Shader::_uniformSender_::operator=(const vec3& v) {
		glUniform3fv(loc, 1, v);
	}

	void Shader::_uniformSender_::operator=(const vec4& v) {
		glUniform4fv(loc, 1, v);
	}

	void Shader::_uniformSender_::operator=(const ivec2& v) {
		glUniform2iv(loc, 1, v);
	}

	void Shader::_uniformSender_::operator=(const ivec3& v) {
		glUniform3iv(loc, 1, v);
	}

	void Shader::_uniformSender_::operator=(const ivec4& v) {
		glUniform4iv(loc, 1, v);
	}
	
	void Shader::_uniformSender_::operator=(const mat4& m) {
		glUniformMatrix4fv(loc, 1, GL_TRUE, m);
	}

	Shader::_uniformSender_& Shader::_uniformSender_::operator[](size_t i) {
		loc = usingShader->getUloc(point, i);
		return *this;
	}
}