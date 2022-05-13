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
		for (Rigidbody2D* obj : Rigidbody2D::objs) {
			obj->UpdateP();
		}
		makeCollisions();
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
					if (dist <= 0) {	// 터치 검사
						o1->entity->onTrigger(o2->entity); o2->entity->onTrigger(o1->entity);
						if (o1->body && o2->body) {
							collide(o1, o2, rel);
							resolveOverlap(o1, o2, dist, rel);
						}
					}
				}				
			}
		}
	}

	void Ballpool2D::resolveOverlap(BallCollider2D* b1, BallCollider2D* b2, float dist, const vec4& posvel) {
		vec2 p3(posvel);
		p3.normalize();
		p3 *= dist;
		float im = 1 / (b1->body->inverseMass + b2->body->inverseMass);
		b1->entity->getTransform()->addPosition(p3 * im * b1->body->inverseMass * 2);
		b2->entity->getTransform()->addPosition(-p3 * im * b2->body->inverseMass * 2);
	}

	void Ballpool2D::collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel) {
		float im = b1->body->inverseMass + b2->body->inverseMass;
		vec2 p3(posvel);
		vec2 v3(posvel[2], posvel[3]);
		float r = RESTITUTIONS[b1->surface][b2->surface];
		float proj = v3.dot(p3);
		if (proj <= 0) { return; }
		vec2 imp = -proj * (r + 1) / (p3.length2() * im) * p3;
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
}