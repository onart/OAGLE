/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Physics.h"

extern float dt;

namespace onart {
	PointMass::PointMass(Transform& tr) :tr(tr) {}
	PointMass2D::PointMass2D(Transform& tr) :tr(tr) {}

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

	void PointMass2D::impulse(const vec2& a) {
		velocity += a * inverseMass;
	}

	void PointMass::accelerate(const vec3& a) {
		velocity += a;
	}

	void PointMass2D::accelerate(const vec2& a) {
		velocity += a;
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

	void PointMass2D::addConstantForce(const vec2& f) {
		acceleration += f * inverseMass;
	}

	void PointMass2D::addAbsoluteForce(const vec2& f) {
		acceleration += f;
	}

	void DragGenerator::generate(PointMass* pm) {
		const vec3& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void DragGenerator2D::generate(PointMass2D* pm) {
		const vec2& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}
}