#include "gl/glad/glad.h"

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
			printf("셰이더 파일이 없습니다: %s\n", vert);
			return;
		}
		auto size = (size_t)std::filesystem::file_size(vert);
		char* vertScript = new char[size + 1];
		fread_s(vertScript, size, 1, size, fp);
		vertScript[size] = '\0';
		fclose(fp);

		fopen_s(&fp, frag, "rb");
		if (!fp) {
			printf("셰이더 파일이 없습니다: %s\n", frag);
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
		glUseProgram(id);

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

	Shader::UniformCode Shader::uniform(const char* name, int i) const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1i(uloc, i);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, float f) const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1f(uloc, f);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, bool b) const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1i(uloc, b);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec2& v2) const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform2fv(uloc, 1, v2);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec3& v3)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform3fv(uloc, 1, v3);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec4& v4)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform4fv(uloc, 1, v4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec2& v2)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform2iv(uloc, 1, v2);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec3& v3)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform3iv(uloc, 1, v3);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec4& v4)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform4iv(uloc, 1, v4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const mat4& m4, bool tr)const {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniformMatrix4fv(uloc, 1, tr, m4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	void Shader::texture(unsigned tex, TexCode idx) const {
		int i = (int)idx;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	void Shader::use() {
		if (id == 0) {
			printf("%p: 사용할 수 없는 셰이더 프로그램입니다.\n", this);
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
		glBindVertexArray(m.getID());
		recentLen = m.getLength();
	}

	void Shader::draw() const {
		glDrawElements(GL_TRIANGLES, recentLen, GL_UNSIGNED_INT, nullptr);
	}

	/* //예정: 정점 데이터=메시, 메시+메터리얼+애니메이션=모델, 모델+기능=개체. 이때 name은 모델을 기준으로 검색됨
	void Shader::draw(const char* name) {
		Mesh* m = list[name];
		glBindVertexArray(m.getID());
		glDrawElements(GL_TRIANGLES, m.getLength(), GL_UNSIGNED_INT, nullptr);
	}
	*/
}