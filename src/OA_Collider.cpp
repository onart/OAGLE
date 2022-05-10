#include "OA_Collider.h"
#include "OA_PhysicalSys.h"
#include "OA_Rigidbody.h"

namespace onart {
	std::vector<Collider2D*> Collider2D::objs;

	template <class T>
	void insort(std::vector<T*>& v, T* e) {
		v.insert(std::lower_bound(v.begin(), v.end(), e), e);
	}

	template <class T>
	void removeFromSorted(std::vector<T*>& v, T* e) {
		auto it = std::lower_bound(v.begin(), v.end(), e);
		if (*it == e)v.erase(it);
	}

	Collider2D::Collider2D(Entity* entity, float radius, const vec2& offset, Rigidbody2D* body, PHYSICAL_SURFACE surface)
		:tr(entity), surface((int)surface), body(body), baseRadius(radius), scale(1) {
		insort(objs, this);
	}

	Collider2D::~Collider2D() {
		removeFromSorted(objs, this);
	}

	void Collider2D::savePosc() {	// 프레임 당 한 번 호출
		const vec3& pos0 = tr->getTransform()->getGlobalPosition();
		vec3 off = tr->getTransform()->getLocalRotation().toMat3() * offset;
		colPos = pos0 + off;
	}

	PolylineCollider2D::PolylineCollider2D(Entity* entity, Rigidbody2D* body, bool closed, const std::vector<vec2>& points, PHYSICAL_SURFACE surface) : Collider2D(entity, 0, 0.0f, body, surface) {
		size_t sz = points.size();
		assert(sz >= 2 && "최소 2개 이상의 위치벡터가 필요합니다.");
		basePoints.reserve(sz);
		transformed.reserve(sz);
		for (const vec2& v : points) { basePoints.push_back(vec4(v[0], v[1], 0, 1)); }
		vec2 e1, e2;
		for (size_t i = 0; i < sz - 1; i++) {
			const vec2& v = points[i];
			for (size_t j = i + 1; j < sz; j++) {
				const vec2& v2 = points[j];
				float thisDis2 = v.distance2(v2);
				if (thisDis2 > baseRadius) {
					e1 = v;
					e2 = v2;
					baseRadius = thisDis2;
				}
			}
		}
		baseRadius = sqrtf(baseRadius);
		offset = (e1 + e2) * 0.5f;
	}

	void PolylineCollider2D::savePosc() {
		mat4 model = tr->getTransform()->getModel();
		transformed.clear();
		for (const vec4& v : basePoints) {
			transformed.push_back(model * v);
		}
		const vec3& pos0 = tr->getTransform()->getGlobalPosition();
		vec3 off = tr->getTransform()->getLocalRotation().toMat3() * offset;
		colPos = pos0 + off;
	}

	vec2 Collider2D::check(const vec2& center, float rad) {
		// 바운딩은 이미 겹침을 확인
		if (body) {
			vec2 rp(this->colPos - center);
			// 상대 속도와 상대 위치를 내적한 값이 음수인 경우 멀어지므로 충돌 없는 걸로
			// body->impulse(구해진 힘);
			return;
		}
		else {	// 판정만
			// tr->onTrigger;
			return vec2(NAN, 0);	// 이것을 받은 충돌체도 판정만
		}
	}

	vec2 Collider2D::check(const vec2& p1, const vec2& p2) {
		vec2 pos = lineCircleIntersect(colPos, baseRadius * scale, p1, p2);
		// body->impulse(구해진 힘)
		return pos;
	}

	vec2 PolylineCollider2D::check(const vec2& center, float rad) {
		vec2 met;
		float count = 0;
		for (size_t i = 0; i < transformed.size() - 1; i++) {
			vec2 point = lineCircleIntersect(center, rad, transformed[i], transformed[i + 1]);
			if (isfinite(point[0])) { 
				if (!body) {
					// tr->onTrigger;
					return vec2(NAN, 0);	// 이것을 받은 충돌체도 판정만
				}
				met += point; count += 1.0f;
			}
		}
		// addforce(met/count);
		return met / count;
	}
}