/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Transform.h"

namespace onart {
	void Transform::mat2prs() {
		pos = model.col(4);
		scale = model.row(1);
		vec3 rsr2(model.row(2)), rsr3(model.row(3));
		scale *= scale;	rsr2 *= rsr2;	rsr3 *= rsr3;
		scale += rsr2;	scale += rsr3;
		sqrt4(scale.entry);
		mat4 rot(model * mat4::scale(vec3(1) / scale));
		float tr = rot.trace() - rot._44;		// = 2cosx + 1
		float cos2 = (tr + 1) * 0.25f;			// = (1 + cosx) / 2 = cos^2(x/2)
		float sin2 = 1 - cos2;
		float c = (tr - 1) * 0.5f;	// = cosx
		c = c > 1 ? 1 : c;
		float s = sqrtf(1 - c * c);
		if (s <= FLT_EPSILON) {
			memset(&rotation, 0, sizeof(rotation));
			rotation.c1 = 1;
		}
		else {
			rotation.ci = (rot._32 - rot._23);	// 2ci * sinx
			rotation.cj = (rot._13 - rot._31);	// 2cj * sinx
			rotation.ck = (rot._21 - rot._12);	// 2ck * sinx
			rotation *= 0.5f / s * sqrtf(sin2);
			rotation.c1 = sqrtf(cos2);
		}
		ready = true;
	}
}