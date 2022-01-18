#ifndef __OA_MODEL_H__
#define __OA_MODEL_H__

#include <string>
#include <vector>
#include <map>
#include "oaglem.h"

struct aiScene;

namespace onart {
	
	class Mesh;
	class Material;
	class Shader;
	/// <summary>
	/// 최종적으로 화면에 등장할 모델 클래스입니다.
	/// 현재 단일 메시 및 텍스처에 대해서만 그릴 수 있습니다.
	/// </summary>
	class Model
	{
	public:
		enum class TexType { AMBIENT, DIFFUSE, SPECULAR, NORMAL };
		/// <summary>
		/// 메모리에서 모델을 불러옵니다.
		/// </summary>
		/// <param name="data">변수 주소</param>
		/// <param name="len">데이터 길이</param>
		/// <param name="meshName">메시 이름</param>
		/// <param name="hint">형식 힌트</param>
		static Model* load(const unsigned char* data, size_t len, const std::string& meshName, const char* hint = "");
		/// <summary>
		/// 파일에서 모델을 불러옵니다. 이때, 메시 이름은 파일 이름과 같게 저장됩니다.
		/// </summary>
		static Model* load(const std::string& file);
		/// <summary>
		/// 메시가 갱신된 경우 다시 로드합니다.
		/// </summary>
		void reloadMesh();
		/// <summary>
		/// 해당 셰이더에 이 모델을 렌더링합니다.
		/// </summary>
		/// <param name="shader">셰이더</param>
		/// <param name="material">메터리얼 번호(이 모델에서 사용하는 것만 따집니다.)</param>
		/// <param name="color">색상(mix가 아니라 *로 적용됩니다.)</param>
		void render(Shader& shader, const int material = 0, const vec4& color = 1);
		/// <summary>
		/// 이미 불러온 모델에 diffuse 텍스처 이미지를 추가합니다.
		/// </summary>
		/// <param name="index">채울 인덱스입니다. 디버그 모드의 오류 메시지에서 어떤 모델에 어떤 인덱스를 채울지 확인해 주세요.</param>
		/// /// <param name="tex">텍스처입니다. Material::genTextureFrom... 함수들에서 얻어올 수 있습니다.</param>
		/// <param name="typ">텍스처 유형입니다. 현재 엔진은 DIFFUSE, NORMAL만 사용합니다.</param>
		void addTex(unsigned index, unsigned tex, TexType typ);
	private:
		struct Geometry {
			unsigned start;
			unsigned count;
			unsigned material;
		};
		Model(const aiScene*, const std::string& meshName);
		Mesh** mesh;
		std::string meshName;

		std::vector<Material*> materials;
		std::vector<Geometry> geom;
		
		static std::map<std::string, Model*> list;
	};
}

#endif // !__OA_MODEL_H__


