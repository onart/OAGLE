#include "gl/glad/glad.h"

#include "OA_Shader.h"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <string>

namespace onart {
	
	Shader* Shader::usingShader = nullptr;

	void Shader::initWithFile(const char* vert, const char* frag) {
		FILE* fp;
		fopen_s(&fp, vert, "rb");
		if (!fp) {
			printf("셰이더 파일이 없습니다: %s\n", vert);
			return;
		}
		auto size = std::filesystem::file_size(vert);
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
		size = std::filesystem::file_size(frag);
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
		unsigned fs = compileScript(vert, GL_FRAGMENT_SHADER);

		if (vs == 0 || fs == 0) { 
			id = 0;
			return;
		}

		glAttachShader(id, vs);
		glAttachShader(id, fs);

		glLinkProgram(id);
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

	Shader::UniformCode Shader::uniform(const char* name, int i) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1i(uloc, i);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, float f) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1f(uloc, f);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, bool b) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform1i(uloc, b);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec2& v2) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform2fv(uloc, 1, v2);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec3& v3) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform3fv(uloc, 1, v3);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const vec4& v4) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform4fv(uloc, 1, v4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec2& v2) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform2iv(uloc, 1, v2);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec3& v3) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform3iv(uloc, 1, v3);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const ivec4& v4) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniform4iv(uloc, 1, v4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
	}

	Shader::UniformCode Shader::uniform(const char* name, const mat4& m4, bool tr) {
		assert(usingShader == this);
		int uloc = glGetUniformLocation(id, name);
		glUniformMatrix4fv(uloc, 1, tr, m4);
#ifdef _DEBUG
		if (uloc <= -1) return UniformCode::NOTFOUND;
		else if (glGetError() != GL_NO_ERROR) return UniformCode::INVALID;
#endif
		return UniformCode::SUCCESS;
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
}