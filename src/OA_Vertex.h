#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

#include <map>
#include <string>

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
	/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)의 클래스입니다. 기본적으로 스택 생성이 불가능합니다.
	/// </summary>
	class Mesh {
		public:
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 이름으로 찾아옵니다. 없는 경우 nullptr를 리턴합니다.
			/// </summary>
			/// <param name="name">불러올 때 직접 정한 이름</param>
			/// <returns>메시의 위치를 가리키는 포인터의 주소(Mesh**)</returns>
			static Mesh** get(const std::string& name);
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 새로 추가합니다. 단, 예약된 이름을 사용할 수 없으며 그 외에 이미 있는 이름을 대상으로 시도하면 기존 모델을 덮어씁니다. 성공 여부를 리턴합니다.
			/// <para>예약된 이름: rect, circ, sphr, clnd, cubo, (빈 문자열)</para>
			/// </summary>
			static bool add(const std::string& name, Mesh* m);
			/// <summary>
			/// 기초 모델을 메모리에서 제거합니다. 단, 빌트인 모델은 제거할 수 없습니다. 성공 여부를 리턴합니다. 이미 없는 이름을 제거하려고 시도할 경우 성공으로 취급됩니다.
			/// </summary>
			static bool unload(const std::string& name);
			/// <summary>
			/// 정점 배열 오브젝트의 id를 얻습니다.
			/// </summary>
			inline unsigned getID() const { return vao; }
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
			/// 정점 버퍼와 인덱스 버퍼를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			static unsigned createVAO(unsigned vb, unsigned ib);
			inline Mesh() {}
		private:
			inline Mesh(unsigned vb, unsigned ib, unsigned vao, unsigned length) :vb(vb), ib(ib), vao(vao), length(length) {};
			~Mesh();	// 프로그램 종료 전까지 임의로 메시 삭제 불가능
			unsigned vb = 0, ib = 0, vao = 0;
			const unsigned length = 0;

			static std::map<std::string, Mesh*> list;
	};
}

#endif // !__OA_VERTEX_H__



