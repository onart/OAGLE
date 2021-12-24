#include <cstdio>

#include "OA_Material.h"
#include "gl/glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace onart {
	std::map<void*, unsigned> Material::texFromMemory;
	std::map<std::string, unsigned> Material::texFromFile;

	unsigned Material::genTextureFromFile(const char* file, bool reset) {
		auto t = texFromFile.find(file);
		if (t != texFromFile.end()) { 
			if (reset) return t->second;
			else texFromFile.erase(t);
		}
		int width, height, nrChannels;
		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
		if (!data) {
			printf("텍스처 로드에 실패했습니다.\n");
			return 0;
		}
		GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
		unsigned tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, format, 0, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		texFromFile[file] = tex;
		return tex;
	}

	unsigned Material::genTextureFromMemory(unsigned char* bts, unsigned len, bool hasAlpha, bool reset) {
		auto t = texFromMemory.find(bts);
		if (t != texFromMemory.end()) { 
			if (reset) return t->second;
			else texFromMemory.erase(t);
		}
		int width, height, nrChannels;
		unsigned char* data = stbi_load_from_memory(bts, len, &width, &height, &nrChannels, hasAlpha ? 4 : 3);
		if (!data) {
			printf("텍스처 로드에 실패했습니다.\n");
			return 0;
		}
		unsigned tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, hasAlpha ? GL_RGBA : GL_RGB, 0, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		texFromMemory[bts] = tex;
		return tex;
	}

	void Material::drop(void* var) {
		auto t = texFromMemory.find(var);
		if (t != texFromMemory.end()) {
			glDeleteTextures(1, &(t->second));
			texFromMemory.erase(t);
		}
	}

	void Material::drop(const char* file) {
		auto t = texFromFile.find(file);
		if (t != texFromFile.end()) {
			glDeleteTextures(1, &(t->second));
			texFromFile.erase(t);
		}
	}
}