#include "OA_Camera.h"
#include "OA_Shader.h"
#include "OA_Scene.h"

extern float dt;
extern onart::Shader program3;

namespace onart {

	void Camera::viewUpdate() {
		vec3 desiredEye;
		const vec3* desiredAt;

		desiredAt = at ? at : &fixedAt;
		desiredEye = *desiredAt + relativePos;
		float way = delay * dt;
		vec3 wantedPos;
		if (way > 1) {
			wantedPos = desiredEye;
		}
		else if (way > 0) {
			wantedPos = lerp(currentPos, desiredEye, way);
		}
		currentPos = Scene::currentScene->constrainCamera(currentPos, wantedPos);
		if (fixdir) program3.uniform("view", mat4::lookAt(currentPos, currentPos - relativePos, up));
		else program3.uniform("view", mat4::lookAt(currentPos, *desiredAt, up));
		
	}

	void Camera::setZoom(float zoom) {
		program3.uniform("zoom", zoom);
	}

	void Camera::setDelay(float s) {
		constexpr float ln100 = 4.605170185988f;	// 실제 ln100보다 약 6.40e-8만큼 큼
		delay = s ? ln100 / fabs(s) : INFINITY;
	}

	mat4 Camera::Ratio::getAspectMatrix() {
		mat4 ret;
		ret[0] = ratio > 1 ? ratio : 1;
		ret[5] = ratio < 1 ? 1 / ratio : 1;
		return ret;
	}

	void Camera::Ratio::setProjMatrix2D() {
		program3.uniform("proj", getAspectMatrix());
	}

	void Camera::Ratio::setProjMatrix3D(float fovy, float dnear, float dfar) {
		this->fovy = fovy;	this->dnear = dnear;	this->dfar = dfar;
		program3.uniform("proj", mat4::perspective(fovy, ratio, dnear, dfar));
	}

	void Camera::Ratio::setFovy(float fovy) {
		this->fovy = fovy;
		program3.uniform("proj", mat4::perspective(fovy, ratio, dnear, dfar));
	}

	void Camera::Ratio::setNear(float near) {
		this->dnear = near;
		program3.uniform("proj", mat4::perspective(fovy, ratio, dnear, dfar));
	}

	void Camera::Ratio::setFar(float far) {
		this->dfar = far;
		program3.uniform("proj", mat4::perspective(fovy, ratio, dnear, dfar));
	}
}