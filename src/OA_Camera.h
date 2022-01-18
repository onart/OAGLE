#ifndef __OA_CAMERA_H__

#include "oaglem.h"

namespace onart {
	class Camera
	{
	public:
		/// <summary>
		/// ī�޶� 3���� ���͸� ���󰩴ϴ�. nullptr�� �����ϴ� ��� ī�޶� ���� ������ ��ġ���� ���� ����������,
		/// ī�޶� ���󰡴� ���� �Ҵ�� vec3 ��ü�� ������ ��� ���α׷��� ������ �����ϼ���.
		/// </summary>
		/// <param name="relativePos">ī�޶� �� ������ �������� �� ī�޶��� ��ġ�Դϴ�.</param>
		/// <param name="at">ī�޶� ���� �����Դϴ�.</param>
		inline void follow(const vec3& relativePos, const vec3* at = nullptr) { 
			this->relativePos = relativePos; 
			if (!at) {
				fixedAt = *(this->at);
				fixedEye = fixedAt + relativePos;
			}
			this->at = at; 
		}
		/// <summary>
		/// ī�޶� ���ϴ� ��ġ�� ������ �����մϴ�.
		/// </summary>
		/// <param name="relativePos">ī�޶� �� ������ ����(0,0,0)���� �� ī�޶��� ��ġ�Դϴ�.</param>
		/// <param name="fixedAt">ī�޶� �� �����Դϴ�.</param>
		inline void fix(const vec3& relativePos, const vec3& fixedAt) { at = nullptr; this->fixedAt = fixedAt; fixedEye = fixedAt + relativePos; }

		/// <summary>
		/// ī�޶��� ������ �����̸� �����մϴ�.
		/// </summary>
		/// <param name="s">�������� ���� �����Դϴ�. ī�޶� eye�� ��ġ���� ������(at�� ������ ����, ī�޶� �̵��ؾ� �� �Ÿ��� 99%) �̵��ϴ� �� �ɸ��� �ð��� �ǹ��մϴ�. ���� ���� ���� ��� ������ ���� ������ ��ȯ�˴ϴ�.</param>
		void setDelay(float s);
		
		/// <summary>
		/// ī�޶� ���� �� ����� ������Ʈ�Ͽ� ���̴��� �ѱ�ϴ�. FPS�� 20 �̻��� ��� ī�޶��� �������� ũ�� ���� ���� �ʽ��ϴ�.
		/// </summary>
		void viewUpdate();
	private:
		vec3 fixedEye, fixedAt, up;
		vec3 relativePos;
		vec3 currentPos;
		
		float delay = 0;

		const vec3* at = nullptr;
		
		mat4 view;
	};
}

#endif // !__OA_CAMERA_H__
