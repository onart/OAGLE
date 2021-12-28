#ifndef __OA_ENTITY_H__
#define __OA_ENTITY_H__

#include <string>
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// ���� �� ���� �����ϴ� ��ü���Դϴ�.
	/// </summary>
	class Entity
	{
	public:
		/// <summary>
		/// ��ü�� active�ϴٴ� ���� update()�� �� ������ ȣ��ȴٴ� �ǹ��Դϴ�.
		/// </summary>
		bool isActive = true;
	protected:
		/// <summary>
		/// ��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.
		/// </summary>
		Transform transform;
		/// <summary>
		/// ��ü�� �ð�(timepoint)�Դϴ�. ��ü�� Active�� ��쿡�� �����մϴ�.
		/// </summary>
		const float& localTp;
		/// <summary>
		/// ������ ����� �� ���� �����ӿ� ��������� �帥 �ð�(��)�Դϴ�.
		/// </summary>
		static const float& tp;
	private:
		float lt = 0;
	};
}


#endif // !__OA_ENTITY_H__



