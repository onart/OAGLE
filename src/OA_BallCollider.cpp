/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Game.h"
#include "OA_BallCollider.h"
#include "binaryIOvec.h"
#include "OA_Model.h"
#include "OA_Shader.h"
#include "OA_Rigidbody.h"

namespace onart {

	std::vector<BallCollider2D*> BallCollider2D::objs;
	std::vector<BallCollider3D*> BallCollider3D::objs;

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
		vec2 absOffset = entity->getTransform()->getRotation().toMat3() * offset;
		pos_vel = entity->getTransform()->getGlobalPosition() + absOffset;
		if (body) {
			vec2 vel = body->getVelocity();
			vec2 tan = absOffset;
			float temp = tan[1];	tan[1] = -tan[0];	tan[0] = temp;	// 각속도 방향은 + 아니면 -뿐이므로 선속도 방향은 +90도 회전만 하면 됨. 이는 결국 외적과 동일한 과정
			vel += tan * body->getAngularVel();
			pos_vel[2] = vel[0];
			pos_vel[3] = vel[1];
		}
		// 개괄 영역(직사각형) 계산
		vec4 ldru(pos_vel[0], pos_vel[1], pos_vel[0], pos_vel[1]);
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
		USE_SHADER_UNIFORM;
		Game::program3[color] = vec4(0, 1, 0, 0.3f);
		Game::program3[model] = mat4::TRS(vec3(pos_vel[0], pos_vel[1], -1), Quaternion(), radius);
		Game::program3[piv] = mat4();
		Game::program3.texture(Material::get("white1x1")->id);
		Game::program3.draw(**Mesh::get("circ"));
	}

	BallCollider3D::BallCollider3D(Entity* entity, float radius, const vec3& offset, Rigidbody3D* body, PHYSICAL_SURFACE surface)
		:entity(entity), radius(radius), offset(offset), body(body), surface((int)surface), isActive(true) {
		insort(objs, this);
	}

	BallCollider3D::~BallCollider3D(){
		removeFromSorted(objs, this);
	}

	void BallCollider3D::range() {
		if (!isActive) return;
		vec3 absOffset = entity->getTransform()->getRotation().toMat3() * offset;
		gpos = entity->getTransform()->getGlobalPosition() + absOffset;
		if (body) {
			vel = body->getVelocity();
			vec3 av = cross(absOffset, body->getAngularVel());
			vel += av;
		}
		vec3 lowers(gpos);	lowers -= radius;	lowers *= INV_ONE_GRID;
		vec3 uppers(gpos + radius);	uppers += radius;	uppers *= INV_ONE_GRID;
		ivec3 xyzIDX;
		f2i(lowers.entry, xyzIDX.entry);
		rangex = rangey = rangez = (range_t)(~0);
		if (isInRange<0, PARTS>(xyzIDX[0])) { rangex >>= xyzIDX[0]; }
		if (isInRange<0, PARTS>(xyzIDX[1])) { rangey >>= xyzIDX[1]; }
		if (isInRange<0, PARTS>(xyzIDX[2])) { rangez >>= xyzIDX[2]; }
		f2i(uppers.entry, xyzIDX.entry);
		if (xyzIDX[0] < PARTS) { rangex -= ((range_t)(1) << (PARTS - 1 - xyzIDX[0])) - 1; }
		if (xyzIDX[1] < PARTS) { rangey -= ((range_t)(1) << (PARTS - 1 - xyzIDX[1])) - 1; }
		if (xyzIDX[2] < PARTS) { rangez -= ((range_t)(1) << (PARTS - 1 - xyzIDX[2])) - 1; }
	}

	void BallCollider3D::render() {
		USE_SHADER_UNIFORM;
		Game::program3[color] = vec4(0, 1, 0, 0.3f);
		Game::program3[model] = mat4::TRS(vec3(gpos), Quaternion(), radius);
		Game::program3[piv] = mat4();
		Game::program3.texture(Material::get("white1x1")->id);
		Game::program3.draw(**Mesh::get("sphr"));
	}
}