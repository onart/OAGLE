/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Rigidbody.h"
#include "binaryIOvec.h"

extern float dt;

namespace onart {
	std::vector<Rigidbody2D*> Rigidbody2D::objs;

	Rigidbody2D::Rigidbody2D(float mass, float inertiaMoment, Transform& transform)
		:inverseMass(1 / mass), inverseMoment(1 / inertiaMoment), transform(transform), netTorque(0), angularVel(0), angularAcc(0) {
		insort(objs, this);
	}

	Rigidbody2D::~Rigidbody2D() {
		removeFromSorted(objs, this);
	}

	void Rigidbody2D::UpdateV() {
		velocity += acceleration * dt;
		velocity += netForce * inverseMass * dt;
		netForce = 0;
	}

	void Rigidbody2D::UpdateP() {
		transform.addPosition(vec3(velocity * dt, 0));
	}
}