/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Rigidbody.h"
#include "binaryIOvec.h"

extern float dt, idt;

namespace onart {
	std::vector<Rigidbody2D*> Rigidbody2D::objs;
	std::vector<Rigidbody3D*> Rigidbody3D::objs;

	Rigidbody2D::Rigidbody2D(float mass, float inertiaMoment, Transform& transform)
		:inverseMass(1 / mass), inverseMoment(1 / inertiaMoment), transform(transform), netTorque(0), angularVel(0), angularAcc(0) {
		insort(objs, this);
	}

	Rigidbody2D::~Rigidbody2D() {
		removeFromSorted(objs, this);
	}

	void Rigidbody2D::UpdateV() {
		velocity += (acceleration + netForce * inverseMass) * dt;
		angularVel += (angularAcc + netTorque * inverseMoment) * dt;
		netForce = 0;
		netTorque = 0;
	}

	void Rigidbody2D::UpdateP() {
		transform.dAddRotation(vec3(0, 0, angularVel * dt));
		transform.addPosition(vec3(velocity * dt, 0));
	}

	Rigidbody3D::Rigidbody3D(float mass, const mat3& inverseInertiaMoment, Transform& transform)
		:inverseMass(1 / mass), inverseInertiaTensor(inverseInertiaMoment), transform(transform) {
		insort(objs, this);
	}

	Rigidbody3D::~Rigidbody3D() {
		removeFromSorted(objs, this);
	}

	mat3 Rigidbody3D::worldIITensor() {
		const Quaternion& rot = transform.getRotation();
		return rot.toMat3() * inverseInertiaTensor * rot.conjugate().toMat3();
	}

	void Rigidbody3D::UpdateV() {
		velocity += (acceleration + netForce * inverseMass) * dt;
		angularVel += (angularAcc + worldIITensor() * netTorque) * dt;
		netForce = 0;
		netTorque = 0;
	}

	void Rigidbody3D::UpdateP() {
		Quaternion rot(transform.getLocalRotation());
		rot += dt * 0.5f * Quaternion(angularVel) * rot;
		/* https://fgiesen.wordpress.com/2012/08/24/quaternion-differentiation/
		* 결론: 사원수의 왼쪽에 axis, angle * dt 기반의 사원수를 곱하는 것은
		* 각속도 vec3의 사원수 버전(실수부분 0, 나머지 벡터성분 그대로)에 대하여 위 식으로 근사할 수 있음
		* 실험 결과 dt=1/60에서 정규화 시 안정적인 결과를 보임
		*/
		rot.normalize();
		transform.dSetRotation(rot);
		transform.addPosition(dt * velocity);
		
	}
}