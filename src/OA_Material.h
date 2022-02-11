#ifndef __OA_MATERIAL_H__
#define __OA_MATERIAL_H__

#include "oaglem.h"
#include <map>
#include <string>

namespace onart {
	struct Texture {
		unsigned id;
		ivec2 size;
		inline Texture(unsigned id = 0, const ivec2& size = 0) : id(id), size(size) {}
	};
	/// <summary>
	/// 텍스처와 표면 광택에 대한 클래스입니다.
	/// </summary>
	class Material
	{
	public:
		inline void setAmbientTex(unsigned ambient) { textures.ambient = ambient; }
		inline void setTex(unsigned tex) { textures.diffuse = tex; }
		inline void setDiffuseTex(unsigned diffuse) { textures.diffuse = diffuse; }
		inline void setSpecularTex(unsigned specular) { textures.specular = specular; }
		inline void setBumpTex(unsigned bump) { textures.bump = bump; }
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
		/// </summary>
		/// <param name="file">파일 이름</param>
		/// <param name="reset">true인 경우 기존의 동일 이름의 텍스처를 지우고 새로 생성합니다.</param>
		/// <param name="name">프로그램 내에서 사용할 텍스처 이름(입력하지 않는 경우 파일 이름을 그대로 사용)</param>
		static Texture genTextureFromFile(const char* file, bool reset = false, const char* name = nullptr);
		/// <summary>
		/// 메모리의 변수(이미지)로부터 2D 텍스처를 생성합니다. 24비트/32비트(알파채널) 이미지만 가능합니다.
		/// </summary>
		/// <param name="bts">이미지 데이터입니다.</param>
		/// <param name="len">데이터 길이입니다.</param>
		/// <param name="name">프로그램 내에서 사용할 텍스처 이름</param>
		/// <param name="reset">true인 경우 기존의 동일 이름의 텍스처를 지우고 새로 생성합니다.</param>
		static Texture genTextureFromMemory(unsigned char* bts, unsigned len, const std::string& name, bool reset = false);
		/// <summary>
		/// 해당 이름의 텍스처를 제거합니다.
		/// </summary>
		static void drop(const std::string& name);
	private:
		static std::map<std::string, Texture> texList;

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

		float shininess;
		float alpha;
		float refractIdx;
	};
}

#endif // !__OA_MATERAIL_H__


