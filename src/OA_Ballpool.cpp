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
		size_t sz = BallCollider2D::objs.size();
		if (sz == 0) return;
		for (size_t i = 0; i < sz - 1; i++) {
			BallCollider2D::objs[i]->range();
		}
		for (size_t i = 0; i < sz - 1; i++) {
			BallCollider2D* o1 = BallCollider2D::objs[i];
			vec4 p1(o1->pos_vel);
			for (size_t j = i + 1; j < sz; j++) {
				BallCollider2D* o2 = BallCollider2D::objs[j];
				if (o1->coarseCheck(o2) && o1->isActive && o2->isActive && o1->entity != o2->entity) {	// 개괄 검사
					vec4 rel(p1 - o2->pos_vel);					
					if (reinterpret_cast<vec2*>(&rel)->length2() <= (o1->radius + o2->radius) * (o1->radius + o2->radius)) {	// 터치 검사
						o1->entity->onTrigger(o2->entity); o2->entity->onTrigger(o1->entity);
						if (o1->body && o2->body) {
							collide(o1, o2, rel);
						}
					}
				}				
			}
		}
	}

	void Ballpool2D::collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel) {
		float im = b1->body->inverseMass + b2->body->inverseMass;
		vec2 p3(posvel);
		vec2 v3(posvel[2], posvel[3]);
		float r = RESTITUTIONS[b1->surface][b2->surface];
		float proj = v3.dot(p3);
		if (proj >= 0) { return; }
		vec2 imp = -proj * (r + 1) / (p3.length2() * im) * p3;
		b1->body->impulse(imp, b1->pos_vel);
		b2->body->impulse(-imp, b2->pos_vel);
		b1->pos_vel[2] = b1->body->velocity[0];
		b1->pos_vel[3] = b1->body->velocity[1];
		b2->pos_vel[2] = b2->body->velocity[0];
		b2->pos_vel[3] = b2->body->velocity[1];
		// 속도가 보통 수준인 경우라면 겹침 해소를 하지 않아도 괜찮을까..?
	}

	void Ballpool2D::render() {
		for (BallCollider2D* obj : BallCollider2D::objs) {
			obj->render();
		}
	}
}