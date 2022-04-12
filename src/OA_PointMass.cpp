/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Copyright (c) 2003-2009 Ian Millington
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_PointMass.h"
#include "OA_PhysicalSystem.h"

extern float dt;

namespace onart {
	PointMass::PointMass(Transform& tr) :tr(tr) { PointMassSystem::addIndividual(this); }
	PointMass2D::PointMass2D(Transform& tr) : tr(tr) { PointMassSystem2D::addIndividual(this); }
	PointMass::~PointMass() { PointMassSystem::removeIndividual(this); }
	PointMass2D::~PointMass2D() { PointMassSystem2D::removeIndividual(this); }

	void PointMass::Update() {
		tr.addPosition(dt * velocity);
		impulse(dt * netForce);
		velocity += dt * acceleration;
		velocity *= std::powf(damp, dt);
		netForce = 0;
	}

	void PointMass::Update(float dt) {
		tr.addPosition(dt * velocity);
		impulse(dt * netForce);
		velocity += dt * acceleration;
		velocity *= std::powf(damp, dt);
		netForce = 0;
	}

	void RigidBody::Update() {
		if (!isAwake) return;
		Quaternion rot(tr.getLocalRotation());
		rot += dt * 0.5f * Quaternion(aVelocity) * rot;
		/* https://fgiesen.wordpress.com/2012/08/24/quaternion-differentiation/ 
		* 결론: 사원수의 왼쪽에 axis, angle * dt 기반의 사원수를 곱하는 것은
		* 각속도 vec3의 사원수 버전(실수부분 0, 나머지 벡터성분 그대로)에 대하여 위 식으로 근사할 수 있음
		* 실험 결과 dt=1/60에서 정규화 시 안정적인 결과를 보임
		*/
		rot.normalize();
		tr.dSetRotation(rot);
		tr.addPosition(dt * velocity);
		impulse(dt * netForce);
		velocity += dt * acceleration;
		velocity *= std::powf(linearDamping, dt);
		vec3 aa = worldTensor() * netTorque;
		aVelocity += aa * dt;
		aVelocity *= std::powf(angularDamping, dt);
		netTorque = 0;
		netForce = 0;
	}

	void PointMass2D::Update() {
		tr.addPosition(dt * velocity);
		impulse(dt * netForce);
		velocity += dt * acceleration;
		velocity *= std::powf(damp, dt);
		netForce = 0;
	}

	void PointMass2D::Update(float dt) {
		tr.addPosition(dt * velocity);
		impulse(dt * netForce);
		velocity += dt * acceleration;
		velocity *= std::powf(damp, dt);
		netForce = 0;
	}

	void PointMass::impulse(const vec3& a) {
		velocity += a * inverseMass;
	}

	void RigidBody::impulse(const vec3& a) {
		velocity += a * inverseMass;
	}

	void PointMass2D::impulse(const vec2& a) {
		velocity += a * inverseMass;
	}

	void PointMass::accelerate(const vec3& a) {
		velocity += a;
	}

	void PointMass2D::accelerate(const vec2& a) {
		velocity += a;
	}

	void RigidBody::accelerate(const vec3& a) {
		velocity += a;
	}

	vec3 PointMass::getAcceleration() {
		return acceleration + netForce * inverseMass;
	}

	vec3 RigidBody::getAcceleration() {
		return acceleration + netForce * inverseMass;
	}

	void PointMass::addForce(const vec3& f) {
		netForce += f;
	}

	void PointMass2D::addForce(const vec2& f) {
		netForce += f;
	}

	void PointMass::addConstantForce(const vec3& f) {
		acceleration += f * inverseMass;
	}

	void PointMass::addAbsoluteForce(const vec3& f) {
		acceleration += f;
	}

	void RigidBody::addConstantForce(const vec3& f) {
		acceleration += f * inverseMass;
	}

	void PointMass2D::addConstantForce(const vec2& f) {
		acceleration += f * inverseMass;
	}

	void PointMass2D::addAbsoluteForce(const vec2& f) {
		acceleration += f;
	}

	void RigidBody::addAbsoluteForce(const vec3& f) {
		acceleration += f;
	}

	vec2 PointMass2D::getAcceleration() {
		return acceleration * netForce * inverseMass;
	}

	mat3 RigidBody::worldTensor() {
		mat3 rot(tr.getRotation().toMat3());
		return rot * inverseInertiaTensor * rot.transpose();
	}

	void RigidBody::addForce(const vec3& force) {
		netForce += force;
		isAwake = true;
	}

	void RigidBody::addForceAtPoint(const vec3& force, const vec3& point) {
		vec3 pt(point);
		pt -= tr.getGlobalPosition();
		netForce += force;
		netTorque += cross(pt, force);
		isAwake = true;
	}

	void RigidBody::addForceAtBodyPoint(const vec3& force, const vec3& point) {
		addForceAtPoint(force, tr.getGlobalPosition() + point);
	}

	void RigidBody::addTorque(const vec3& q) {
		netTorque += q;
	}

	mat3 RigidBody2D::worldTensor() {	// 공사 예정
		mat3 rot(tr.getRotation().toMat3());
		return rot * inverseInertiaTensor * rot.transpose();
	}

	void RigidBody2D::addForce(const vec2& force) {
		netForce += force;
		isAwake = true;
	}

	void RigidBody2D::addForceAtPoint(const vec2& force, const vec2& point) {
		vec2 pt(point);
		pt -= tr.getGlobalPosition();
		netForce += force;
		float tq = cross2(pt, force);
	}
}