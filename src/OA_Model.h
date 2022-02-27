/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_MODEL_H__
#define __OA_MODEL_H__

#include <string>
#include <vector>
#include <map>

#include "OA_Vertex.h"
#include "oaglem.h"

struct aiScene;

namespace onart {
	struct Texture;
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
		/// 메모리에서 모델을 불러옵니다. Mesh 객체는 모델과 동일한 이름을 가집니다.
		/// </summary>
		/// <param name="data">변수 주소</param>
		/// <param name="len">데이터 길이</param>
		/// <param name="meshName">모델 이름</param>
		/// <param name="hint">형식 힌트</param>
		static std::shared_ptr<Model> load(const unsigned char* data, size_t len, const std::string& meshName, const char* hint = "");
		/// <summary>
		/// 파일에서 모델을 불러옵니다. 이때, 메시 이름은 파일 이름과 같게 저장됩니다.
		/// </summary>
		static std::shared_ptr<Model> load(const std::string& file);
		/// <summary>
		/// 셰이더에 이 모델을 렌더링합니다.
		/// </summary>
		/// <param name="color">색상(mix가 아니라 *로 적용됩니다.)</param>
		void render(const vec4& color = 1) const;
		/// <summary>
		/// 이미 불러온 모델에 diffuse 텍스처 이미지를 추가합니다.
		/// </summary>
		/// <param name="index">채울 인덱스입니다. 디버그 모드의 오류 메시지에서 어떤 모델에 어떤 인덱스를 채울지 확인해 주세요.</param>
		/// <param name="tex">텍스처입니다. Material::genTextureFrom... 함수들에서 얻어올 수 있습니다.</param>
		/// <param name="typ">텍스처 유형입니다. 현재 엔진은 DIFFUSE, NORMAL만 사용합니다.</param>
		void addTex(unsigned index, std::shared_ptr<Texture>& tex, TexType typ);
	private:
		struct Geometry {
			unsigned start;
			unsigned count;
			unsigned material;
		};
		Model(const aiScene*, const std::string& meshName);
		ppMesh mesh;
		std::string meshName;

		std::vector<std::unique_ptr<Material>> materials;
		std::vector<Geometry> geom;
		
		static std::map<std::string, std::shared_ptr<Model>> list;
	};

	using pModel = std::shared_ptr<Model>;
}

#endif // !__OA_MODEL_H__


