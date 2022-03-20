/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_PHYSICS_H__
#define __OA_PHYSICS_H__

#include "oaglem.h"
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// 일부 물리 법칙을 적용받는 질점입니다. 트랜스폼에 운동 법칙을 적용하는 형태가 되는데, 성능상 루트 트랜스폼에만 부착하는 것을 강하게 권장합니다.
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
		/// <summary>
		/// 매 프레임 물리 법칙을 적용합니다. 이것은 개체의 로컬 시간 흐름을 위해 준비된 함수입니다. 매개변수가 없는 Update()와 이것 중 하나만을 선택합니다.
		/// </summary>
		void Update(float dt);
		/// <summary>
		/// 개체를 순간 가속시킵니다. 속도 변화량은 질량에 반비례합니다.
		/// 접촉 시간 등에 비례하는 지속적 힘 가하기는 addForce 함수를 이용하세요.
		/// </summary>
		void impulse(const vec3&);
		/// <summary>
		/// 개체를 순간 가속시킵니다. 속도 변화량은 질량에 관계 없이 주어진 만큼입니다.
		/// 접촉 시간 등에 비례하는 지속적 힘 가하기는 addForce 함수를 이용하세요.
		/// </summary>
		void accelerate(const vec3&);
		/// <summary>
		/// 현재 프레임에 힘을 가합니다.
		/// </summary>
		void addForce(const vec3&);
		/// <summary>
		/// 지속적으로 힘을 가합니다. 가속도는 질량에 반비례합니다.
		/// 주고 있던 힘을 없애려면 동일한 함수에 반대 방향의 벡터를 주면 됩니다. 항력과 같이 상수가 아닌 힘에 대해서는 힘 발생기를 사용합니다.
		/// </summary>
		void addConstantForce(const vec3&);
		/// <summary>
		/// 지속적으로 힘을 가합니다. 가속도는 질량에 관계 없이 주어진 만큼입니다. 예를 들어 중력을 이렇게 적용합니다.
		/// 주고 있던 힘을 없애려면 동일한 함수에 반대 방향의 벡터를 주면 됩니다.
		/// </summary>
		void addAbsoluteForce(const vec3&);
		/// <summary>
		/// 현재 속도를 리턴합니다.
		/// </summary>
		inline const vec3& getVelocity() const { return velocity; }
		/// <summary>
		/// 역질량을 리턴합니다.
		/// </summary>
		inline float getInverseMass() const { return inverseMass; }
	protected:
		vec3 velocity;
		vec3 acceleration;
		vec3 netForce;
		float inverseMass;
		float damp;
	private:
		Transform& tr;
	};

	/// <summary>
	/// 일부 물리 법칙을 적용받는 질점입니다. 트랜스폼에 운동 법칙을 적용하는 형태가 되는데, 성능상 루트 트랜스폼에만 부착하는 것을 강하게 권장합니다.
	/// </summary>
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
		/// <summary>
		/// 매 프레임 물리 법칙을 적용합니다. 이것은 개체의 로컬 시간 흐름을 위해 준비된 함수입니다. 매개변수가 없는 Update()와 이것 중 하나만을 선택합니다.
		/// </summary>
		void Update(float dt);
		/// <summary>
		/// 개체를 순간 가속시킵니다. 속도 변화량은 질량에 반비례합니다.
		/// 접촉 시간 등에 비례하는 지속적 힘 가하기는 addForce 함수를 이용하세요.
		/// </summary>
		void impulse(const vec2&);
		/// <summary>
		/// 개체를 순간 가속시킵니다. 속도 변화량은 질량에 관계 없이 주어진 만큼입니다.
		/// 접촉 시간 등에 비례하는 지속적 힘 가하기는 addForce 함수를 이용하세요.
		/// </summary>
		void accelerate(const vec2&);
		/// <summary>
		/// 현재 프레임에 힘을 가합니다.
		/// </summary>
		void addForce(const vec2&);
		/// <summary>
		/// 지속적으로 힘을 가합니다. 가속도는 질량에 반비례합니다.
		/// 주고 있던 힘을 없애려면 동일한 함수에 반대 방향의 벡터를 주면 됩니다. 항력과 같이 상수가 아닌 힘에 대해서는 힘 발생기를 사용합니다.
		/// </summary>
		void addConstantForce(const vec2&);
		/// <summary>
		/// 지속적으로 힘을 가합니다. 가속도는 질량에 관계 없이 주어진 만큼입니다. 예를 들어 중력을 이렇게 적용합니다.
		/// 주고 있던 힘을 없애려면 동일한 함수에 반대 방향의 벡터를 주면 됩니다.
		/// </summary>
		void addAbsoluteForce(const vec2&);
		/// <summary>
		/// 현재 속도를 리턴합니다.
		/// </summary>
		inline const vec2& getVelocity() const { return velocity; }
		/// <summary>
		/// 역질량을 리턴합니다.
		/// </summary>
		inline float getInverseMass() const { return inverseMass; }
	protected:
		vec2 velocity;
		vec2 acceleration;
		vec2 netForce;
		float inverseMass;
		float damp;
	private:
		Transform& tr;
	};

	/// <summary>
	/// 매 프레임 힘을 지속적으로 가합니다. 중력 같은 거의 상수인 힘이 아닌 항력 등에 대하여 사용합니다.
	/// </summary>
	class ForceGenerator {
	public:
		virtual void generate(PointMass* pm) = 0;
		inline virtual void generate(PointMass* pm, float dt) {}
	};

	/// <summary>
	/// 매 프레임 힘을 지속적으로 가합니다. 중력 같은 거의 상수인 힘이 아닌 항력 등에 대하여 사용합니다.
	/// </summary>
	class ForceGenerator2D {
	public:
		virtual void generate(PointMass2D* pm) = 0;
		inline virtual void generate(PointMass2D* pm, float dt) {}
	};

	/// <summary>
	/// 이것은 힘 발생기의 한 예입니다. 계수 2개를 미리 정해 두고 항력을 적용합니다.
	/// </summary>
	class DragGenerator :public ForceGenerator {
	public:
		inline DragGenerator(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass* pm);
	private:
		float k1, k2;
	};
}

#endif // !__OA_PHYSICS_H__

