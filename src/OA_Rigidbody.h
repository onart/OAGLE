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
	/// ���� ���� ��Ģ�� ����޴� 2���� ��ü�Դϴ�. ������ �ֻ��� transform���� �ϴ� ���� ���ɻ� �����ϴ�.
	/// </summary>
	class Rigidbody2D {
		friend class PhysicalSys2D;
	public:
		/// <summary>
		/// ��ü�� �����մϴ�.
		/// </summary>
		/// <param name="mass">����</param>
		/// <param name="inertiaMoment">�������Ʈ</param>
		/// <param name="transform">��ü�� ��ȯ ������</param>
		inline Rigidbody2D(float mass, float inertiaMoment, Transform* transform) :inverseMass(1 / mass), inverseMoment(1 / inertiaMoment), transform(transform), netTorque(0), angularVel(0), angularAcc(0) {
			objs.push_back(this);
		}
		/// <summary>
		/// ���� �����ӿ� �ӵ��� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateV();
		/// <summary>
		/// ���� �����ӿ� ��ġ�� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateP();
		/// <summary>
		/// �� ������ ���� ��ü�� ���� ���մϴ�. ���������� ���� ���� ���� �ƴ϶�� impulse() �Լ��� ����ϼ���.
		/// </summary>
		inline void addForce(const vec2& force) { netForce += force; }
		/// <summary>
		/// ������ �ð��� ���� ���� ��ü�� ������ �ݺ���ϰ� ���ӽ�ŵ�ϴ�. ��, ������ 1�� ��ü�� ���������� ���ڴ� ���� �ָ� �˴ϴ�.
		/// ������ ���� ���� Ư�� �ӵ��� ����� �ʹٸ� accelerate() �Լ��� ����ϼ���.
		/// </summary>
		inline void impulse(const vec2& force) { velocity += inverseMass * force; }
		/// <summary>
		/// ������ �ð� �� ������ ���� ���� �ӵ��� �־��� ����ŭ ���մϴ�. setVelocity() �Լ��� �����ϼ���.
		/// </summary>
		inline void accelerate(const vec2& dv) { velocity += dv; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocity(const vec2& v) { velocity = v; }
		/// <summary>
		/// ���������� ���� ���մϴ�. ������ �ݺ���մϴ�.
		/// �̸� ���ַ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�.
		/// </summary>
		inline void addConstantForce(const vec2& force) { acceleration += force * inverseMass; }
		/// <summary>
		/// �������� ������ �߰��մϴ�. ������ �����ϹǷ� �߷��� �̰����� ������ �� �ֽ��ϴ�.
		/// �̸� ���ַ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�.
		/// </summary>
		inline void addConstantAcceleration(const vec2& da) { acceleration += da; }
		/// <summary>
		/// �� ������ ���� +Z��(�ð� ����)���� ��ũ�� ���մϴ�.
		/// </summary>
		inline void addTorque(float torque) { netTorque += torque; }
	private:
		Transform* transform;	// ��ü ��ġ, ȸ��
		vec2 velocity;			// �ӵ�
		vec2 acceleration;		// ���ӵ�
		vec2 netForce;			// �շ�
		float netTorque = 0;	// ����ũ
		float inverseMass;		// ������
		float angularVel = 0;	// ���ӵ� (+z����)
		float angularAcc = 0;	// �����ӵ� (+z����)
		float inverseMoment;	// ���������Ʈ
		static std::vector<Rigidbody2D*> objs;
	};
}

#endif
