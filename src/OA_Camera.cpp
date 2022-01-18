#include "OA_Camera.h"
#include "OA_Shader.h"

extern float dt;
extern onart::Shader program3;

namespace onart {
	void Camera::viewUpdate() {
		vec3 desiredEye;
		const vec3* desiredAt;

		desiredAt = at ? at : &fixedAt;
		desiredEye = *desiredAt + relativePos;
		float way = delay * dt;
		if (way > 1) {
			currentPos = desiredEye;
		}
		else if (way > 0) {
			currentPos = lerp(currentPos, desiredEye, way);
		}
		view = mat4::lookAt(currentPos, *desiredAt, up);
		program3.uniform("view", view);
	}

	void Camera::setDelay(float s) {
		constexpr float ln100 = 4.605170185988f;	// 실제 ln100보다 약 6.40e-8만큼 큼
		delay = s ? ln100 / fabs(s) : INFINITY;
	}
}