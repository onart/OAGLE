/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_RIGIDBODY_H__
#define __OA_RIGIDBODY_H__

#include "oaglem.h"
#include <vector>

namespace onart {

	class Transform;

	/// <summary>
	/// 고전 역학 법칙을 적용받는 2차원 강체입니다. 가급적 최상위 transform에만 하는 것이 성능상 좋습니다.
	/// </summary>
	class Rigidbody2D {
		friend class PhysicalSys2D;
	public:
		/// <summary>
		/// 강체를 생성합니다.
		/// </summary>
		/// <param name="mass">질량</param>
		/// <param name="inertiaMoment">관성모멘트</param>
		/// <param name="transform">개체의 변환 포인터</param>
		inline Rigidbody2D(float mass, float inertiaMoment, Transform* transform) :inverseMass(1 / mass), inverseMoment(1 / inertiaMoment), transform(transform), netTorque(0), angularVel(0), angularAcc(0) {
			objs.push_back(this);
		}
		/// <summary>
		/// 현재 프레임에 속도를 업데이트합니다.
		/// </summary>
		void UpdateV();
		/// <summary>
		/// 현재 프레임에 위치를 업데이트합니다.
		/// </summary>
		void UpdateP();
		/// <summary>
		/// 현 프레임 동안 강체에 힘을 가합니다. 지속적으로 힘을 가할 것이 아니라면 impulse() 함수를 사용하세요.
		/// </summary>
		inline void addForce(const vec2& force) { netForce += force; }
		/// <summary>
		/// 프레임 시간에 관계 없이 물체를 질량에 반비례하게 가속시킵니다. 즉, 질량이 1인 물체가 가속했으면 좋겠는 양을 주면 됩니다.
		/// 질량에 관계 없이 특정 속도를 만들고 싶다면 accelerate() 함수를 사용하세요.
		/// </summary>
		inline void impulse(const vec2& force) { velocity += inverseMass * force; }
		/// <summary>
		/// 프레임 시간 및 질량에 관계 없이 속도를 주어진 값만큼 더합니다. setVelocity() 함수도 참고하세요.
		/// </summary>
		inline void accelerate(const vec2& dv) { velocity += dv; }
		/// <summary>
		/// 속도를 강제로 정합니다.
		/// </summary>
		inline void setVelocity(const vec2& v) { velocity = v; }
		/// <summary>
		/// 지속적으로 힘을 가합니다. 질량에 반비례합니다.
		/// 이를 없애려면 반대 방향의 벡터를 주면 됩니다.
		/// </summary>
		inline void addConstantForce(const vec2& force) { acceleration += force * inverseMass; }
		/// <summary>
		/// 지속적인 가속을 추가합니다. 질량에 무관하므로 중력을 이것으로 적용할 수 있습니다.
		/// 이를 없애려면 반대 방향의 벡터를 주면 됩니다.
		/// </summary>
		inline void addConstantAcceleration(const vec2& da) { acceleration += da; }
		/// <summary>
		/// 현 프레임 동안 +Z축(시계 방향)으로 토크를 가합니다.
		/// </summary>
		inline void addTorque(float torque) { netTorque += torque; }
	private:
		Transform* transform;	// 개체 위치, 회전
		vec2 velocity;			// 속도
		vec2 acceleration;		// 가속도
		vec2 netForce;			// 합력
		float netTorque = 0;	// 합토크
		float inverseMass;		// 역질량
		float angularVel = 0;	// 각속도 (+z방향)
		float angularAcc = 0;	// 각가속도 (+z방향)
		float inverseMoment;	// 역관성모멘트
		static std::vector<Rigidbody2D*> objs;
	};
}

#endif
