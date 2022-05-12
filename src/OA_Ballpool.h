#ifndef __OA_BALLPOOL_H__
#define __OA_BALLPOOL_H__

#include "oaglem.h"

namespace onart {
	class BallCollider2D;
	class Ballpool2D {
	public:
		/// <summary>
		/// 2D 물리 개체를 업데이트합니다.
		/// </summary>
		static void Update();
		/// <summary>
		/// 디버그를 위해 원형 충돌체를 화면에 반투명하게 표시합니다.
		/// </summary>
		static void render();
	private:
		/// <summary>
		/// 2D 물리 개체의 충돌을 확인하고 판정을 가합니다.
		/// </summary>
		static void makeCollisions();
		/// <summary>
		/// 2D 물리 개체의 충돌을 확인하고 판정을 가합니다.
		/// </summary>
		/// <param name="b1"></param>
		/// <param name="b2"></param>
		/// <param name="posvel"></param>
		static void collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel);
	};
}

#endif // !__OA_BALLPOOL_H__

