#ifndef __OA_CAMERA_H__

#include "oaglem.h"

namespace onart {
	class Camera
	{
	public:
		/// <summary>
		/// 카메라가 3차원 벡터를 따라갑니다. nullptr를 대입하는 경우 카메라가 원래 가려던 위치까지 가서 정지하지만,
		/// 카메라가 따라가던 동적 할당된 vec3 객체가 해제된 경우 프로그램이 깨지니 주의하세요.
		/// </summary>
		/// <param name="relativePos">카메라가 볼 지점을 기준으로 한 카메라의 위치입니다.</param>
		/// <param name="at">카메라가 따라갈 벡터입니다.</param>
		inline void follow(const vec3& relativePos, const vec3* at = nullptr) { 
			this->relativePos = relativePos; 
			if (!at) {
				fixedAt = *(this->at);
				fixedEye = fixedAt + relativePos;
			}
			this->at = at; 
		}
		/// <summary>
		/// 카메라가 원하는 위치를 보도록 고정합니다.
		/// </summary>
		/// <param name="relativePos">카메라가 볼 지점을 기준(0,0,0)으로 한 카메라의 위치입니다.</param>
		/// <param name="fixedAt">카메라가 볼 지점입니다.</param>
		inline void fix(const vec3& relativePos, const vec3& fixedAt) { at = nullptr; this->fixedAt = fixedAt; fixedEye = fixedAt + relativePos; }

		/// <summary>
		/// 카메라의 움직임 딜레이를 설정합니다.
		/// </summary>
		/// <param name="s">딜레이의 결정 변수입니다. 카메라가 eye의 위치까지 완전히(at이 정지한 기준, 카메라가 이동해야 할 거리의 99%) 이동하는 데 걸리는 시간을 의미합니다. 음의 값을 넣을 경우 강제로 양의 값으로 변환됩니다.</param>
		void setDelay(float s);
		
		/// <summary>
		/// 카메라에 따른 뷰 행렬을 업데이트하여 셰이더에 넘깁니다. FPS가 20 이상인 경우 카메라의 움직임이 크게 차이 나지 않습니다.
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
