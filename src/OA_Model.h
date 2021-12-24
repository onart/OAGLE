#ifndef __OA_MODEL_H__
#define __OA_MODEL_H__

#include <string>
#include <vector>
#include "oaglem.h"

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
		/// <summary>
		/// 메시가 갱신된 경우 다시 로드합니다.
		/// </summary>
		void reloadMesh();
		/// <summary>
		/// 해당 셰이더에 이 모델을 렌더링합니다.
		/// </summary>
		void render(const Shader& shader);
	private:
		Mesh** mesh;
		vec4 color;
		std::string meshName;
		
		std::vector<Material*> materials;
		std::vector<size_t> startIdx;
		std::vector<size_t> idxLen;
	};
}

#endif // !__OA_MODEL_H__


