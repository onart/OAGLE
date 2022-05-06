#include "OA_Collider.h"

namespace onart {
	std::vector<Collider2D*> Collider2D::objs;

	PolylineCollider2D::PolylineCollider2D(Entity* entity, const std::vector<vec2>& points) : Collider2D(entity) {
		size_t sz = points.size();
		basePoints.reserve(sz);
		for (const vec2& v : points) {
			basePoints.push_back(vec4(v[0], v[1], 0, 1));
		}
		baseRadius = 0;
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
		baseOffset = (e1 + e2) * 0.5f;
	}
}