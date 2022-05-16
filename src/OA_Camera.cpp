/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Camera.h"
#include "OA_Shader.h"
#include "OA_Scene.h"
#include "OA_Transform.h"

USE_SHADER_UNIFORM;

namespace onart {

	void Camera::follow(const vec3& relativePos, Transform* at) {
		this->relativePos = relativePos;
		if (!at) {
			fixedAt = *(this->at);
		}
		this->at = &(at->getGlobalPosition());
	}

	void Camera::viewUpdate() {
		vec3 desiredEye;
		const vec3* desiredAt;

		desiredAt = at ? at : &fixedAt;
		desiredEye = *desiredAt + relativePos;
		float way = delay * Game::dt();
		vec3 wantedPos;
		if (way > 1) {
			wantedPos = desiredEye;
		}
		else if (way > 0) {
			wantedPos = lerp(currentPos, desiredEye, way);
		}
		else {
			wantedPos = desiredEye;
		}
		currentPos = Scene::currentScene->constrainCamera(currentPos, wantedPos);
		if (fixdir) { 
			Game::program3[view] = viewM4 = mat4::lookAt(currentPos, currentPos - relativePos, up);
		}
		else { 
			Game::program3[view] = viewM4 = mat4::lookAt(currentPos, *desiredAt, up);
		}
		
	}

	void Camera::setZoom(float zoom) {
		this->zoom = zoom;
		Game::program3[::zoom] = zoom;
	}

	vec2 Camera::mouse2screen(const vec2& mousePos)
	{
		USE_NVEC_XYZW;
		const float& r = ratio.ratio;
		if (r > 1) {
			return vec2((mousePos[x] * 2 - 1) * r, -2 * mousePos[y] + 1);
		}
		else {
			return vec2(2 * mousePos[x] - 1, (-2 * mousePos[y] + 1) / r);
		}
	}

	void Camera::setDelay(float s) {
		constexpr float ln100 = 4.605170185988f;	// 실제 ln100보다 약 6.40e-8만큼 큼
		delay = s ? ln100 / fabs(s) : INFINITY;
	}

	mat4 Camera::Ratio::getAspectMatrix() {
		mat4 ret;
		ret[0] = ratio > 1 ? 1 / ratio : 1;
		ret[5] = ratio < 1 ? ratio : 1;
		return ret;
	}

	void Camera::Ratio::setProjMatrix2D() {
		projM4 = getAspectMatrix();
		Game::program2[aspect] = projM4;
		Game::program3[proj] = projM4;
	}

	void Camera::Ratio::setProjMatrix3D(float fovy, float dnear, float dfar) {
		this->fovy = fovy;	this->dnear = dnear;	this->dfar = dfar;
		Game::program2[aspect] = getAspectMatrix();
		Game::program3[proj] = projM4 = mat4::perspective(fovy, ratio, dnear, dfar);
	}

	void Camera::Ratio::setFovy(float fovy) {
		this->fovy = fovy;
		Game::program3[proj] = projM4 = mat4::perspective(fovy, ratio, dnear, dfar);
	}

	void Camera::Ratio::setNear(float near) {
		this->dnear = near;
		Game::program3[proj] = projM4 = mat4::perspective(fovy, ratio, dnear, dfar);
	}

	void Camera::Ratio::setFar(float far) {
		this->dfar = far;
		Game::program3[proj] = projM4 = mat4::perspective(fovy, ratio, dnear, dfar);
	}
}