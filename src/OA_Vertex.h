/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

#define USE_BUMP
#define USE_ANIM

namespace onart {

	/// <summary>
	/// 렌더링 파이프라인의 정점입니다.
	/// <para>법선 매핑을 위해서는 USE_BUMP 매크로가 정의되어야 합니다.</para>
	/// <para>관절 애니메이션을 위해서는 USE_ANIM 매크로가 정의되어야 합니다.</para>
	/// </summary>
	struct Vertex
	{
		/// <summary>
		/// 모델 내 정점의 위치입니다.
		/// </summary>
		vec3 pos;
		/// <summary>
		/// 정점에서의 법선입니다.
		/// </summary>
		vec3 norm;
		/// <summary>
		/// 2D 텍스처 좌표입니다.
		/// </summary>
		vec2 tc;
#ifdef USE_BUMP
		/// <summary>
		/// 법선 매핑을 위한 접선 벡터입니다.
		/// </summary>
		vec3 tan;
		/// <summary>
		/// 법선 매핑을 위한 접선 벡터에 수직이 되는 벡터입니다. 
		/// </summary>
		vec3 bitan;
#endif // USE_BUMP
#ifdef USE_ANIM
		/// <summary>
		/// 관절 애니메이션을 위한 뼈의 인덱스입니다.
		/// </summary>
		ivec4 boneIDs = { -1,-1,-1,-1 };
		/// <summary>
		/// 관절 애니메이션을 위한 뼈의 영향력(가중치)입니다.
		/// </summary>
		vec4 boneWeights = { 0,0,0,0 };
#endif // USE_ANIM

	};

	/// <summary>
	/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)의 클래스입니다.
	/// </summary>
	class Mesh {
		friend struct std::default_delete<Mesh>;
		public:
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 이름으로 찾아옵니다. 없는 경우 nullptr를 담은 포인터를 리턴합니다.
			/// </summary>
			/// <param name="name">불러올 때 직접 정한 이름</param>
			/// <returns>메시의 위치를 가리키는 포인터의 주소(Mesh**)</returns>
			static std::shared_ptr<std::unique_ptr<Mesh>> get(const std::string& name);
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 새로 추가합니다.
			/// 단, 예약된 이름을 사용할 수 없으며 그 외에 이미 있는 이름을 대상으로 시도하면 기존 모델을 덮어씁니다.
			/// 덮어쓰이면 기존의 그 모델을 사용하고 있던 모든 모델이 새로 추가한 것으로 바뀝니다. 성공 여부를 리턴합니다(메모리 부족 혹은 빌트인 이름을 사용한 것이 아니면 성공).
			/// <para>예약된 이름: rect, circ, sphr, clnd, cubo, icubo, (빈 문자열)</para>
			/// </summary>
			static bool add(const std::string& name, const std::vector<Vertex>& v, const std::vector<unsigned>& i);
			/// <summary>
			/// 기초 모델을 메모리에서 제거합니다. 빌트인 모델과 사용 중인 모델은 제거할 수 없습니다.
			/// </summary>
			/// <param name="name">제거하려는 모델 이름</param>
			/// <returns>성공 여부</returns>
			static bool drop(const std::string& name);
			/// <summary>
			/// 사용하고 있지 않은 메시를 모두 메모리에서 제거합니다.
			/// </summary>
			static void collect();
			/// <summary>
			/// 정점 배열 오브젝트의 id를 얻습니다.
			/// </summary>
			inline unsigned getID() const { return vao; }
			inline unsigned getVB() const { return vb; }
			inline unsigned getIB() const { return ib; }
			/// <summary>
			/// 정점 배열의 길이를 얻습니다.
			/// </summary>			
			inline unsigned getLength() const { return length; }
			/// <summary>
			/// 직사각형 정점 배열 오브젝트를 생성합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 그대로 직사각형에 들어갑니다.
			/// </summary>
			static void rectModel();
			/// <summary>
			/// 원 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 정사각형 이미지에 내접하는 원입니다.
			/// </summary>
			static void circleModel();
			/// <summary>
			/// 구 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 지구본과 세계지도의 관계와 동일합니다.
			/// </summary>
			static void sphereModel();
			/// <summary>
			/// 직육면체 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 4x3 배열의 가득 찬 직사각형이 있을 때 1행 2열이 상,
			/// 2행은 각각 좌, 전, 우, 후,
			/// 3행 2열은 하 방향입니다.
			/// </summary>
			static void cuboidModel();
			/// <summary>
			/// 직육면체 정점 배열 오브젝트를 생성하고 리턴합니다. 텍스처가 직육면체 내부에서만 보이도록 하는 차이가 있습니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 4x3 배열의 가득 찬 직사각형이 있을 때 1행 2열이 상,
			/// 2행은 각각 좌, 전, 우, 후,
			/// 3행 2열은 하 방향입니다.
			/// </summary>
			static void iCuboidModel();
			/// <summary>
			/// 정점 벡터와 인덱스 벡터를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			/// <param name="v">정점 벡터</param>
			/// <param name="i">인덱스 벡터</param>
			/// <param name="vb">정점 버퍼를 받을 주소</param>
			/// <param name="ib">인덱스 버퍼를 받을 주소</param>
			/// <returns></returns>
			static unsigned createVAO(const std::vector<Vertex>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib);
			/// <summary>
			/// 정점 버퍼와 인덱스 버퍼를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			static unsigned createVAO(unsigned vb, unsigned ib);
			inline Mesh() {}
		private:
			inline Mesh(unsigned vb, unsigned ib, unsigned vao, unsigned length) :vb(vb), ib(ib), vao(vao), length(length) {};
			~Mesh();	// 프로그램 종료 전까지 임의로 메시 삭제 불가능
			unsigned vb = 0, ib = 0, vao = 0;
			const unsigned length = 0;

			static std::map<std::string, std::shared_ptr<std::unique_ptr<Mesh>>> list;
	};

	using ppMesh = std::shared_ptr<std::unique_ptr<Mesh>>;	// Mesh 클래스에 대한 이중 포인터입니다.
}

#endif // !__OA_VERTEX_H__



