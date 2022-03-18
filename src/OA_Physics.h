#ifndef __OA_PHYSICS_H__
#define __OA_PHYSICS_H__

#include "oaglem.h"
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// �Ϻ� ���� ��Ģ�� ����޴� �����Դϴ�.
	/// </summary>
	class PointMass {
	public:
		/// <summary>
		/// ������ �����մϴ�.
		/// </summary>
		/// <param name="tr">���������� �����̰� �� Ʈ������ ��ü�Դϴ�. null�̸� �ǹ̰� �����Ƿ� �����ڸ� ����մϴ�.</param>
		PointMass(Transform& tr);
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�.
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
		/// ������ �����մϴ�.
		/// </summary>
		/// <param name="tr">���������� �����̰� �� Ʈ������ ��ü�Դϴ�. null�̸� �ǹ̰� �����Ƿ� �����ڸ� ����մϴ�.</param>
		PointMass2D(Transform& tr);
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�.
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

