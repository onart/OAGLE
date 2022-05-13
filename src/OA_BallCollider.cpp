/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_BallCollider.h"
#include "binaryIOvec.h"
#include "OA_Model.h"
#include "OA_Shader.h"

extern onart::Shader program3;

namespace onart {

	std::vector<BallCollider2D*> BallCollider2D::objs;

	BallCollider2D::BallCollider2D(Entity* entity, float radius, const vec2& offset, Rigidbody2D* body, PHYSICAL_SURFACE surface)
		:entity(entity), radius(radius), offset(offset), body(body), surface((int)surface), isActive(true) {
		insort(objs, this);
		range();
	}

	BallCollider2D::~BallCollider2D() {
		removeFromSorted(objs, this);
	}

	void BallCollider2D::range() {
		if (!isActive) return;
		// 절대 위치(원 중심) 계산
		vec3 off = entity->getTransform()->getGlobalPosition() + entity->getTransform()->getLocalRotation().toMat3() * offset;
		vec2 vel = (pos_vel - off) / dt;
		pos_vel = off;
		pos_vel[2] = vel[0];
		pos_vel[3] = vel[1];	// 충돌체 중심의 이전 프레임 대비 위치(=선속도)
		// 개괄 영역(직사각형) 계산
		vec4 ldru(off[0], off[1], off[0], off[1]);
		ldru += vec4(radius, radius, -radius, -radius);
		ldru *= INV_ONE_GRID;
		ivec4 ldruIDX;
		f2i(ldru.entry, ldruIDX.entry);
		rangex = rangey = (range_t)(~0);
		if (isInRange<0, PARTS>(ldruIDX[0])) { rangex >>= ldruIDX[0]; }
		if (isInRange<0, PARTS>(ldruIDX[1])) { rangey >>= ldruIDX[1]; }
		if (ldruIDX[2] < PARTS) { rangex -= ((range_t)(1) << (PARTS - 1 - ldruIDX[2])) - 1; }
		if (ldruIDX[3] < PARTS) { rangey -= ((range_t)(1) << (PARTS - 1 - ldruIDX[3])) - 1; }
	}

	void BallCollider2D::render() {
		program3["color"] = vec4(0, 1, 0, 0.3f);
		program3["model"] = mat4::TRS(vec3(pos_vel[0], pos_vel[1], -1), Quaternion(), radius);
		program3["piv"] = mat4();
		program3.texture(Material::get("white1x1")->id);
		program3.draw(**Mesh::get("circ"));
	}
}