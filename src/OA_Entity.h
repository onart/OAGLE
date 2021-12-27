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
		Transform transform;
	private:
		/// <summary>
		/// ��ü�� �ð�(timepoint)�Դϴ�. ��ü�� Active�� ��쿡�� �귯���ϴ�.
		/// </summary>
		float localTp = 0;
		/// <summary>
		/// ������ ����� �� ���� �����ӿ� ��������� �帥 �ð�(��)�Դϴ�.
		/// </summary>
		static const float& tp;
		
		
	};
}


#endif // !__OA_ENTITY_H__



