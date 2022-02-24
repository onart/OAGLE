/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include <cstdio>

#include "OA_Material.h"
#include "externals/gl/glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"
namespace onart {
	std::map<std::string, Texture> Material::texList;

	Texture Material::genTextureFromFile(const char* file, bool reset, const char* name) {
		std::string texName(name ? name : file);
		auto t = texList.find(texName);
		if (t != texList.end()) { 
			if (!reset) return t->second;
			else drop(texName);
		}
		int width, height, nrChannels;
		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
		if (!data) {
			fprintf(stderr, "텍스처 로드에 실패했습니다.\n");
			return Texture();
		}
		GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;

		unsigned tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		texList[texName] = Texture(tex, ivec2(width, height));;
		glBindTexture(GL_TEXTURE_2D, 0);
		return texList[texName];
	}

	Texture Material::genTextureFromMemory(unsigned char* bts, unsigned len, const std::string& name, bool reset) {
		auto t = texList.find(name);
		if (t != texList.end()) { 
			if (!reset) return t->second;
			else drop(name);
		}
		int width, height, nrChannels;
		unsigned char* data = stbi_load_from_memory(bts, len, &width, &height, &nrChannels, 0);
		if (!data) {
			fprintf(stderr, "텍스처 로드에 실패했습니다.\n");
			return Texture();
		}
		GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
		unsigned tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		texList[name] = Texture(tex, ivec2(width, height));
		glBindTexture(GL_TEXTURE_2D, 0);
		return texList[name];
	}

	void Material::genWhite() {
		unsigned tex;	unsigned char data[] = { 0xff,0xff,0xff,0xff };
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		texList["white1x1"] = Texture(tex, ivec2(1));
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Material::drop(const std::string& name) {
		auto t = texList.find(name);
		if (t != texList.end()) {
			glDeleteTextures(1, &(t->second.id));
			texList.erase(t);
		}
	}

	Texture Material::get(const std::string& name) {
		auto t = texList.find(name);
		if (t != texList.end()) {
			return t->second;
		}
		return Texture();
	}
}