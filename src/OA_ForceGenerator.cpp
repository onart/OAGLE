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
#include "OA_ForceGenerator.h"
#include "OA_Physics.h"

namespace onart {
	void DragGenerator::generate(PointMass* pm) {
		const vec3& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void DragGenerator2D::generate(PointMass2D* pm) {
		const vec2& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void HookeSpring::generate(PointMass* pm) {
		vec3 f(pm->getPosition()); f -= other->getPosition();
		float mag = fabs(1 - restLength / f.length()) * k;
		f *= -mag;
		pm->addForce(f);
	}

	void HookeSpring2D::generate(PointMass2D* pm) {
		vec2 f(pm->getPosition()); f -= other->getPosition();
		float mag = fabs(1 - restLength / f.length()) * k;
		f *= -mag;
		pm->addForce(f);
	}

	void HookeBungee::generate(PointMass* pm) {
		vec3 f(pm->getPosition()); f -= other->getPosition();
		float mag = (1 - restLength / f.length()) * k;
		if (mag <= 0) return;
		f *= mag;
		pm->addForce(f);
	}

	void HookeBungee2D::generate(PointMass2D* pm) {
		vec2 f(pm->getPosition()); f -= other->getPosition();
		float mag = (1 - restLength / f.length()) * k;
		if (mag <= 0) return;
		f *= mag;
		pm->addForce(f);
	}
}