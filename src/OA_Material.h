/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_MATERIAL_H__
#define __OA_MATERIAL_H__

#include "oaglem.h"
#include <map>
#include <string>
#include <memory>

namespace onart {
	/// <summary>
	/// �ؽ�ó Ŭ�����Դϴ�. Material::get���� ���� �� �ֽ��ϴ�.
	/// </summary>
	struct Texture {
		friend class Material;
	public:
		const unsigned id;
		const ivec2 size;
	private:
		inline Texture(unsigned id = 0, const ivec2& size = 0) : id(id), size(size) {}
		~Texture();
	};

	using pTexture = std::shared_ptr<Texture>;	// �ؽ�ó �������Դϴ�.

	/// <summary>
	/// �ؽ�ó�� ǥ�� ���ÿ� ���� Ŭ�����Դϴ�.
	/// </summary>
	class Material
	{
	public:
		inline void setAmbientTex(pTexture& ambient) { savedTex[0] = ambient; textures.ambient = ambient->id; }
		inline void setTex(pTexture& tex) { savedTex[1] = tex; textures.diffuse = tex->id; }
		inline void setDiffuseTex(pTexture& diffuse) { savedTex[1] = diffuse; textures.diffuse = diffuse->id; }
		inline void setSpecularTex(pTexture& specular) { savedTex[2] = specular; textures.specular = specular->id; }
		inline void setBumpTex(pTexture& bump) { savedTex[3] = bump; textures.bump = bump->id; }
		inline void setAmbient(const vec4& ambient) { this->ambient = ambient; }
		inline void setDiffuse(const vec4& diffuse) { this->diffuse = diffuse; }
		inline void setSpecular(const vec4& specular) { this->specular = specular; }
		inline void setEmissive(const vec4& emissive) { this->emissive = emissive; }
		inline void setShininess(float shininess) { this->shininess = shininess; }
		inline void setAlpha(float alpha) { this->alpha = alpha; }
		inline void setRefractiveIndex(float refr) { refractIdx = refr; }

		inline unsigned getAmbientTex() const { return textures.ambient; }
		inline unsigned getDiffuseTex() const { return textures.diffuse; }
		inline unsigned getSpecularTex() const { return textures.specular; }
		inline unsigned getBumpTex() const { return textures.bump; }
		inline const vec4& getAmbient() const { return ambient; }
		inline const vec4& getDiffuse() const { return diffuse; }
		inline const vec4& getSpecular() const { return specular; }
		inline const vec4& getEmissive() const { return emissive; }
		inline float getShininess() const { return shininess; }
		inline float getAlpha() const { return alpha; }
		inline float getRefractiveIndex() const { return refractIdx; }

		/// <summary>
		/// �̹��� ���Ϸκ��� 2D �ؽ�ó�� �����մϴ�. 24��Ʈ/32��Ʈ(����ä��) �̹����� �����մϴ�.
		/// white1x1�̶�� �̸��� ����Ǿ� �ֽ��ϴ�. ������� ������.
		/// </summary>
		/// <param name="file">���� �̸�</param>
		/// <param name="reset">true�� ��� ������ ���� �̸��� �ؽ�ó�� ����� ���� �����մϴ�. �̹� ��� ���� �ؽ�ó�� ��� ��� ����� ����Ǹ� �ڵ����� �޸𸮰� ȸ���˴ϴ�.</param>
		/// <param name="name">���α׷� ������ ����� �ؽ�ó �̸�(�Է����� �ʴ� ��� ���� �̸��� �״�� ���)</param>
		static pTexture genTextureFromFile(const char* file, bool reset = false, const char* name = nullptr);
		/// <summary>
		/// �޸��� ����(�̹���)�κ��� 2D �ؽ�ó�� �����մϴ�. 24��Ʈ/32��Ʈ(����ä��) �̹����� �����մϴ�.
		/// white1x1�̶�� �̸��� ����Ǿ� �ֽ��ϴ�. ������� ������.
		/// </summary>
		/// <param name="bts">�̹��� �������Դϴ�.</param>
		/// <param name="len">������ �����Դϴ�.</param>
		/// <param name="name">���α׷� ������ ����� �ؽ�ó �̸�</param>
		/// <param name="reset">true�� ��� ������ ���� �̸��� �ؽ�ó�� ����� ���� �����մϴ�. �̹� ��� ���� �ؽ�ó�� ��� ��� ����� ����Ǹ� �ڵ����� �޸𸮰� ȸ���˴ϴ�.</param>
		static pTexture genTextureFromMemory(unsigned char* bts, unsigned len, const std::string& name, bool reset = false);
		/// <summary>
		/// ��� 1x1 �ؽ�ó�� �����մϴ�. �̸��� "white1x1"�Դϴ�.
		/// </summary>
		static void genWhite();
		/// <summary>
		/// �ش� �̸��� �ؽ�ó�� �����մϴ�. ���� �ִϸ��̼��̳� �� ��� ��� ���� �ؽ�ó�� ��� �ش� �ִϸ��̼�/���� ��� ������ �� �ڵ����� �������ϴ�.
		/// ����� �̸��� ���ŵ��� �ʽ��ϴ�.
		/// ��� ���� �ؽ�ó�� ���� ����, �ش� �̸��� �ؽ�ó�� �ٽ� �ε����� ������ �߰��� ������ ����� �� �����ϴ�.
		/// </summary>
		static void drop(const std::string& name);
		/// <summary>
		/// ����ϰ� ���� ���� ��� �ؽ�ó�� �޸𸮿��� �����մϴ�. ����ϰ� �ִ� �ؽ�ó�� ��� drop()ó�� ��� ���� ��� �����Ϸ��� ��� �Ű������� true�� �����մϴ�.
		/// </summary>
		static void collect(bool removeUsing = false);
		/// <summary>
		/// �ش� �̸��� �ؽ�ó�� �����ɴϴ�.
		/// </summary>
		static pTexture get(const std::string& name);
	private:
		static std::map<std::string, pTexture> texList;

		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 emissive;

		struct {
			unsigned ambient = 0;
			unsigned diffuse = 0;
			unsigned specular = 0;
			unsigned bump = 0;
		}textures;

		pTexture savedTex[4];

		float shininess;
		float alpha;
		float refractIdx;
	};
}

#endif // !__OA_MATERIAL_H__


