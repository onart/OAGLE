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
	/// 텍스처 클래스입니다. Material::get으로 얻을 수 있습니다.
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

	using pTexture = std::shared_ptr<Texture>;	// 텍스처 포인터입니다.

	/// <summary>
	/// 텍스처와 표면 광택에 대한 클래스입니다.
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
		/// 이미지 파일로부터 2D 텍스처를 생성합니다. 24비트/32비트(알파채널) 이미지만 가능합니다.
		/// white1x1이라는 이름은 예약되어 있습니다. 사용하지 마세요.
		/// </summary>
		/// <param name="file">파일 이름</param>
		/// <param name="reset">true인 경우 기존의 동일 이름의 텍스처를 지우고 새로 생성합니다. 이미 사용 중인 텍스처인 경우 모든 사용이 종료되면 자동으로 메모리가 회수됩니다.</param>
		/// <param name="name">프로그램 내에서 사용할 텍스처 이름(입력하지 않는 경우 파일 이름을 그대로 사용)</param>
		static pTexture genTextureFromFile(const char* file, bool reset = false, const char* name = nullptr);
		/// <summary>
		/// 메모리의 변수(이미지)로부터 2D 텍스처를 생성합니다. 24비트/32비트(알파채널) 이미지만 가능합니다.
		/// white1x1이라는 이름은 예약되어 있습니다. 사용하지 마세요.
		/// </summary>
		/// <param name="bts">이미지 데이터입니다.</param>
		/// <param name="len">데이터 길이입니다.</param>
		/// <param name="name">프로그램 내에서 사용할 텍스처 이름</param>
		/// <param name="reset">true인 경우 기존의 동일 이름의 텍스처를 지우고 새로 생성합니다. 이미 사용 중인 텍스처인 경우 모든 사용이 종료되면 자동으로 메모리가 회수됩니다.</param>
		static pTexture genTextureFromMemory(unsigned char* bts, unsigned len, const std::string& name, bool reset = false);
		/// <summary>
		/// 백색 1x1 텍스처를 생성합니다. 이름은 "white1x1"입니다.
		/// </summary>
		static void genWhite();
		/// <summary>
		/// 해당 이름의 텍스처를 제거합니다. 현재 애니메이션이나 모델 등에서 사용 중인 텍스처의 경우 해당 애니메이션/모델이 모두 없어질 때 자동으로 없어집니다.
		/// 예약된 이름은 제거되지 않습니다.
		/// 사용 중인 텍스처가 남은 경우라도, 해당 이름의 텍스처는 다시 로드하지 않으면 추가로 가져다 사용할 수 없습니다.
		/// </summary>
		static void drop(const std::string& name);
		/// <summary>
		/// 사용하고 있지 않은 모든 텍스처를 메모리에서 제거합니다. 사용하고 있는 텍스처도 모두 drop()처럼 사용 종료 즉시 제거하려는 경우 매개변수에 true를 전달합니다.
		/// </summary>
		static void collect(bool removeUsing = false);
		/// <summary>
		/// 해당 이름의 텍스처를 가져옵니다.
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


