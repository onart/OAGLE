#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

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

	class Model {
		public:
			/// <summary>
			/// 정점 배열 오브젝트의 id를 얻습니다.
			/// </summary>
			inline unsigned getID() const { return id; }
			/// <summary>
			/// 정점 배열의 길이를 얻습니다.
			/// </summary>			
			inline unsigned getLength() const { return length; }
			/// <summary>
			/// 직사각형 정점 배열 오브젝트를 생성합니다. STL map인 models에서 찾을 수 있습니다. ("rect")
			/// 텍스처 이미지는 그대로 직사각형에 들어갑니다.
			/// </summary>
			static void rectModel(bool reset = false);
			/// <summary>
			/// 원 정점 배열 오브젝트를 생성하고 리턴합니다. STL map인 models에서 찾을 수 있습니다. ("circ")
			/// 텍스처 이미지는 정사각형 이미지에 내접하는 원입니다.
			/// </summary>
			static void circleModel(bool reset = false);
			/// <summary>
			/// 구 정점 배열 오브젝트를 생성하고 리턴합니다. STL map인 models에서 찾을 수 있습니다. ("sphr")
			/// 텍스처 이미지는 지구본과 세계지도의 관계와 동일합니다.
			/// </summary>
			static void sphereModel(bool reset = false);
			/// <summary>
			/// 직육면체 정점 배열 오브젝트를 생성하고 리턴합니다. STL map인 models에서 찾을 수 있습니다. ("cubo")
			/// 텍스처 이미지는 4x3 배열의 가득 찬 직사각형이 있을 때 1행 2열이 상,
			/// 2행은 각각 좌, 전, 우, 후,
			/// 3행 2열은 하 방향입니다.
			/// </summary>
			static void cuboidModel(bool reset = false);
			inline Model() :id(0), length(0) {}
		private:
			inline Model(unsigned id, unsigned length) :id(id), length(length) {}
			const unsigned id;
			const unsigned length;
	};
}

#endif // !__OA_VERTEX_H__



