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
#include "OA_Transform.h"
#include <vector>

namespace onart {

	class Transform;

	/// <summary>
	/// ���� ���� ��Ģ�� ����޴� 2���� ��ü�Դϴ�. ������ �ֻ��� transform���� �ϴ� ���� ���ɻ� �����ϴ�.
	/// </summary>
	class Rigidbody2D {
		friend class Ballpool2D;
	public:
		/// <summary>
		/// ��ü�� �����մϴ�.
		/// </summary>
		/// <param name="mass">����</param>
		/// <param name="inertiaMoment">�������Ʈ</param>
		/// <param name="transform">��ü�� ��ȯ ������</param>
		Rigidbody2D(float mass, float inertiaMoment, Transform& transform);
		~Rigidbody2D();
		/// <summary>
		/// �� ������ ���� ��ü�� ���� ���մϴ�. ��ũ�� �߻����� �ʽ��ϴ�. ���������� ���� ���� ���� �ƴ϶�� impulse() �Լ��� ����ϼ���.
		/// </summary>
		inline void addForce(const vec2& force) { netForce += force; }
		/// <summary>
		/// Ư�� ��ġ�� ��ü�� ���� ���մϴ�. ��ũ�� �߻��� �� �ֽ��ϴ�.
		/// </summary>
		inline void addForce(const vec2& force, const vec2& globalPoint) {
			netForce += force;
			netTorque += cross2(globalPoint - transform.getGlobalPosition(), force);
		}
		/// <summary>
		/// ������ �ð��� ���� ���� ��ü�� ������ �ݺ���ϰ� ���ӽ�ŵ�ϴ�. ��, ������ 1�� ��ü�� ���������� ���ڴ� ���� �ָ� �˴ϴ�.
		/// ������ ���� ���� Ư�� �ӵ��� ����� �ʹٸ� accelerate() �Լ��� ����ϼ���.
		/// </summary>
		inline void impulse(const vec2& force) { velocity += inverseMass * force; }
		/// <summary>
		/// ������ �ð��� ���� ���� ��ü�� ������ �ݺ���ϰ� ���ӽ�ŵ�ϴ�. ��ũ�� ���������� �߻��� �� �ֽ��ϴ�.
		/// ������ 1�� ��ü�� ���������� ���ڴ� ���� �ݴϴ�.
		/// </summary>
		inline void impulse(const vec2& force, const vec2& globalPoint) {
			velocity += inverseMass * force;
			angularVel += inverseMoment * cross2(globalPoint - transform.getGlobalPosition(), force);
		}
		/// <summary>
		/// ������ �ð� �� ������ ���� ���� �ӵ��� �־��� ����ŭ ���մϴ�. setVelocity() �Լ��� �����ϼ���.
		/// </summary>
		inline void accelerate(const vec2& dv) { velocity += dv; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocity(const vec2& v) { velocity = v; }
		/// <summary>
		/// X�� ���� �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocityX(float x) { velocity[0] = x; }
		/// <summary>
		/// X�� ���� �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocityY(float y) { velocity[1] = y; }
		/// <summary>
		/// �� ��ü�� ���ӵ��� �����մϴ�.
		/// </summary>
		inline vec2 getVelocity() { return velocity; }
		/// <summary>
		/// �� ��ü�� +z���� ���ӵ�(rad/s)�� �����մϴ�.
		/// </summary>
		inline float getAngularVel() { return angularVel; }
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
		/// �� ������ ���� +Z��(�ð� ����)���� ��ũ�� ���մϴ�. ȸ�� �߽��� �׻� �����߽�, �� ��ü�� �۷ι� ��ġ�Դϴ�.
		/// </summary>
		inline void addTorque(float torque) { netTorque += torque; }
		/// <summary>
		/// ���� ���Ʈ�� �����ϰ� ���ӵ��� ��ȭ��ŵ�ϴ�.
		/// </summary>
		inline void angularAccelerate(float t) { angularVel += t; }
	private:
		/// <summary>
		/// ���� �����ӿ� �ӵ��� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateV();
		/// <summary>
		/// ���� �����ӿ� ��ġ�� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateP();
		Transform& transform;	// ��ü ��ġ, ȸ��
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

	class Rigidbody3D {
		friend class Ballpool3D;
	public:
		/// <summary>
		/// ��ü�� �����մϴ�.
		/// </summary>
		/// <param name="mass">����</param>
		/// <param name="inertiaMoment">�����ټ��� ����� (3x3)</param>
		/// <param name="transform">��ü�� ��ȯ ������</param>
		Rigidbody3D(float mass, const mat3& inverseInertiaMoment, Transform& transform);
		~Rigidbody3D();
		/// <summary>
		/// �� ������ ���� ��ü�� ���� ���մϴ�. ��ũ�� �߻����� �ʽ��ϴ�. ���������� ���� ���� ���� �ƴ϶�� impulse() �Լ��� ����ϼ���.
		/// </summary>
		inline void addForce(const vec3& force) { netForce += force; }
		/// <summary>
		/// Ư�� ��ġ�� ��ü�� ���� ���մϴ�. ��ũ�� �߻��� �� �ֽ��ϴ�.
		/// </summary>
		inline void addForce(const vec3& force, const vec3& globalPoint) {
			netForce += force;
			netTorque += cross(globalPoint - transform.getGlobalPosition(), force);
		}
		/// <summary>
		/// ������ �ð��� ���� ���� ��ü�� ������ �ݺ���ϰ� ���ӽ�ŵ�ϴ�. ��, ������ 1�� ��ü�� ���������� ���ڴ� ���� �ָ� �˴ϴ�.
		/// ������ ���� ���� Ư�� �ӵ��� ����� �ʹٸ� accelerate() �Լ��� ����ϼ���.
		/// </summary>
		inline void impulse(const vec3& force) { velocity += inverseMass * force; }
		/// <summary>
		/// ������ �ð��� ���� ���� ��ü�� ������ �ݺ���ϰ� ���ӽ�ŵ�ϴ�. ��ũ�� ���������� �߻��� �� �ֽ��ϴ�.
		/// ������ 1�� ��ü�� ���������� ���ڴ� ���� �ݴϴ�.
		/// </summary>
		inline void impulse(const vec3& force, const vec3& globalPoint) {
			velocity += inverseMass * force;
			angularVel += worldIITensor() * force;
		}
		/// <summary>
		/// ������ �ð� �� ������ ���� ���� �ӵ��� �־��� ����ŭ ���մϴ�. setVelocity() �Լ��� �����ϼ���.
		/// </summary>
		inline void accelerate(const vec3& dv) { velocity += dv; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocity(const vec3& v) { velocity = v; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocityX(float x) { velocity[0] = x; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocityY(float y) { velocity[1] = y; }
		/// <summary>
		/// �ӵ��� ������ ���մϴ�.
		/// </summary>
		inline void setVelocityZ(float z) { velocity[2] = z; }
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
		/// <summary>
		/// ������ �ð� �� ���� ���Ʈ�� ���� ���� �־��� ��ŭ ���ӵ��� ���մϴ�. (rad/s x ���� ȸ����)
		/// </summary>
		inline void angularAccelerate(const vec3& v) { angularVel += v; }
		/// <summary>
		/// �� ��ü�� ���ӵ��� �����մϴ�.
		/// </summary>
		inline vec3 getVelocity() { return velocity; }
		/// <summary>
		/// �� ��ü�� ���ӵ��� �����մϴ�.
		/// </summary>
		inline vec3 getAngularVel() { return angularVel; }
	private:
		/// <summary>
		/// ���� �����ӿ� �ӵ��� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateV();
		/// <summary>
		/// ���� �����ӿ� ��ġ�� ������Ʈ�մϴ�.
		/// </summary>
		void UpdateP();
		mat3 worldIITensor();
		Transform& transform;
		vec3 velocity;
		vec3 acceleration;
		vec3 netForce;
		vec3 netTorque;		// �� ���� ������ ȸ����(����) x ȸ����(��������)�� ǥ���Ǹ� �ð� ������ ����� ���� ��� Quaternion(0, ȸ������)
		vec3 angularVel;
		vec3 angularAcc;
		const mat3 inverseInertiaTensor;
		float inverseMass;
		static std::vector<Rigidbody3D*> objs;
	};
}

#endif
