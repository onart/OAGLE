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
		Rigidbody2D(float mass, float inertiaMoment, Transform* transform);
		/// <summary>
		/// ���� �����ӿ� ���¸� ������Ʈ�մϴ�.
		/// </summary>
		void Update();
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
		/// </summary>
		inline void addConstantForce(const vec2& force) { acceleration += force * inverseMass; }
		/// <summary>
		/// �������� ������ �߰��մϴ�. ������ �����ϹǷ� �߷��� �̰����� ������ �� �ֽ��ϴ�.
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
		float netTorque;		// ����ũ
		float inverseMass;		// ������
		float angularVel;		// ���ӵ� (+z����)
		float angularAcc;		// �����ӵ� (+z����)
		float inverseMoment;	// ���������Ʈ
		static std::vector<Rigidbody2D*> objs;
	};
}

#endif
