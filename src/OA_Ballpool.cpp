/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Ballpool.h"
#include "OA_Rigidbody.h"
#include "OA_BallCollider.h"

namespace onart {
	void Ballpool2D::Update() {
		for (Rigidbody2D* obj : Rigidbody2D::objs) {
			obj->UpdateV();
		}
		makeCollisions();
		for (Rigidbody2D* obj : Rigidbody2D::objs) {
			obj->UpdateP();
		}
	}

	void Ballpool2D::makeCollisions() {
		int sz = (int)BallCollider2D::objs.size();
		for (int i = 0; i < sz; i++) {
			BallCollider2D::objs[i]->range();
		}
		for (int i = 0; i < sz - 1; i++) {
			BallCollider2D* o1 = BallCollider2D::objs[i];
			vec4 p1(o1->pos_vel);
			for (int j = i + 1; j < sz; j++) {
				BallCollider2D* o2 = BallCollider2D::objs[j];
				if (o1->coarseCheck(o2) && o1->isActive && o2->isActive && o1->entity != o2->entity) {	// 개괄 검사
					vec4 rel(o2->pos_vel - p1);
					float dist = reinterpret_cast<vec2*>(&rel)->length();
					dist -= (o1->radius + o2->radius);
					if (dist <= 9.536743e-7f) {	// 터치 검사. 충돌 인/아웃 검사의 경우 dist 제한을 약간 늘려 주면 ok
						o1->entity->onTrigger(o2->entity); o2->entity->onTrigger(o1->entity);
						if (o1->body && o2->body) {
							collide(o1, o2, rel);
							resolveOverlap(o1, o2, dist, rel);	// 보통의 가속도 기준, 1회면 충분한 것으로 판명
						}
					}
				}
			}
		}
	}

	void Ballpool2D::resolveOverlap(BallCollider2D* b1, BallCollider2D* b2, float dist, vec2 p3) {
		p3.normalize();
		p3 *= dist;
		float im = 1 / (b1->body->inverseMass + b2->body->inverseMass);
		b1->entity->getTransform()->addPosition(p3 * im * b1->body->inverseMass);
		b2->entity->getTransform()->addPosition(-p3 * im * b2->body->inverseMass);
		/*
		b1->pos_vel[0] = b1->entity->getTransform()->getGlobalPosition()[0];
		b1->pos_vel[1] = b1->entity->getTransform()->getGlobalPosition()[1];
		b2->pos_vel[0] = b2->entity->getTransform()->getGlobalPosition()[0];
		b2->pos_vel[1] = b2->entity->getTransform()->getGlobalPosition()[1];
		*/
	}

	void Ballpool2D::collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel) {
		float im = b1->body->inverseMass + b2->body->inverseMass;
		vec2 p3(posvel);
		vec2 v3(posvel[2], posvel[3]);
		float r = RESTITUTIONS[b1->surface][b2->surface];
		float proj = v3.dot(p3);
		if (proj >= 0) { return; }
		vec2 imp = proj * (r + 1) / (p3.length2() * im) * p3;
		b1->body->impulse(imp, b1->pos_vel);
		b2->body->impulse(-imp, b2->pos_vel);
		b1->pos_vel[2] = b1->body->velocity[0];
		b1->pos_vel[3] = b1->body->velocity[1];
		b2->pos_vel[2] = b2->body->velocity[0];
		b2->pos_vel[3] = b2->body->velocity[1];
	}

	void Ballpool2D::render() {
		for (BallCollider2D* obj : BallCollider2D::objs) {
			obj->render();
		}
	}

	void Ballpool3D::Update() {
		for (Rigidbody3D* obj : Rigidbody3D::objs) {
			obj->UpdateV();
		}
		makeCollisions();
		for (Rigidbody3D* obj : Rigidbody3D::objs) {
			obj->UpdateP();
		}
	}

	void Ballpool3D::makeCollisions() {
		int sz = (int)BallCollider3D::objs.size();
		for (int i = 0; i < sz; i++) {
			BallCollider3D::objs[i]->range();
		}
		for (int i = 0; i < sz - 1; i++) {
			BallCollider3D* o1 = BallCollider3D::objs[i];
			vec3 p1(o1->gpos);
			vec3 v1(o1->vel);
			for (int j = i + 1; j < sz; j++) {
				BallCollider3D* o2 = BallCollider3D::objs[j];
				if (o1->coarseCheck(o2) && o1->isActive && o2->isActive && o1->entity != o2->entity) {	// 개괄 검사
					vec3 v3(o2->vel - v1);
					vec3 p3(o2->gpos - v1);
					float dist = reinterpret_cast<vec3*>(&p3)->length();
					dist -= (o1->radius + o2->radius);
					if (dist <= 0) {	// 터치 검사
						o1->entity->onTrigger(o2->entity); o2->entity->onTrigger(o1->entity);
						if (o1->body && o2->body) {
							collide(o1, o2, p3, v3);
							resolveOverlap(o1, o2, dist, p3);
						}
					}
				}
			}
		}
	}

	void Ballpool3D::collide(BallCollider3D* b1, BallCollider3D* b2, const vec3& p3, const vec3& v3) {
		float im = b1->body->inverseMass + b2->body->inverseMass;
		float r = RESTITUTIONS[b1->surface][b2->surface];
		float proj = v3.dot(p3);
		if (proj >= 0) { return; }
		vec2 imp = proj * (r + 1) / (p3.length2() * im) * p3;
		b1->body->impulse(imp, b1->gpos);
		b2->body->impulse(-imp, b2->gpos);
		b1->vel = b1->body->velocity;
		b2->vel = b1->body->velocity;
	}

	void Ballpool3D::resolveOverlap(BallCollider3D* b1, BallCollider3D* b2, float dist, vec3 p3) {
		p3.normalize();
		p3 *= dist;
		float im = 1 / (b1->body->inverseMass + b2->body->inverseMass);
		b1->entity->getTransform()->addPosition(p3 * im * b1->body->inverseMass);
		b2->entity->getTransform()->addPosition(-p3 * im * b2->body->inverseMass);
		/*
		b1->gpos=b1->entity->getTransform()->getGlobalPosition();
		b2->gpos=b1->entity->getTransform()->getGlobalPosition();
		*/
	}
}
