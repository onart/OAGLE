#ifndef __OA_PHYSICS_H__
#define __OA_PHYSICS_H__

#include "oaglem.h"
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// 일부 물리 법칙을 적용받는 질점입니다.
	/// </summary>
	class PointMass {
	public:
		/// <summary>
		/// 질점을 생성합니다.
		/// </summary>
		/// <param name="tr">물리적으로 움직이게 할 트랜스폼 객체입니다. null이면 의미가 없으므로 참조자를 사용합니다.</param>
		PointMass(Transform& tr);
		/// <summary>
		/// 매 프레임 물리 법칙을 적용합니다.
		/// </summary>
		void Update();
	protected:
		vec3 velocity;
		vec3 acceleration;
		float inverseMass;
	private:
		Transform* tr;
	};

	class PointMass2D {
	public:
		/// <summary>
		/// 질점을 생성합니다.
		/// </summary>
		/// <param name="tr">물리적으로 움직이게 할 트랜스폼 객체입니다. null이면 의미가 없으므로 참조자를 사용합니다.</param>
		PointMass2D(Transform& tr);
		/// <summary>
		/// 매 프레임 물리 법칙을 적용합니다.
		/// </summary>
		void Update();
	protected:
		vec2 velocity;
		vec2 acceleration;
		float inverseMass;
	private:
	};
}

#endif // !__OA_PHYSICS_H__

