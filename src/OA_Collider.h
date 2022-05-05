#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"

namespace onart {
	class Entity;

	/// <summary>
	/// 2차원 충돌체입니다.
	/// </summary>
	class Collider2D {
		friend class PhysicalSys2D;
	public:
		inline Collider2D(Entity* entity) :tr(entity) {}
		bool isActive = true;
		inline Entity* getEntity() { return tr; }
	private:
		Entity* tr;
	};

	/// <summary>
	/// 직사각형 충돌체입니다.
	/// </summary>
	class BoxCollider2D :public Collider2D {
	public:
		inline BoxCollider2D(Entity* entity, const vec4& ldwh) :baseRect(ldwh), Collider2D(entity) { }
	private:
		vec4 baseRect;		// LDWH, LD는 transform 상대 오프셋, WH는 스케일에 영향을 받음
	};

	/// <summary>
	/// 원형 충돌체입니다.
	/// </summary>
	class CircleCollider2D :public Collider2D {
	public:
		inline CircleCollider2D(Entity* entity, const vec2& offset, float baseRadius) :Collider2D(entity), offset(offset), baseRadius(baseRadius) { }
	private:
		float baseRadius;
		vec2 offset;	// transform 상대 오프셋
	};

	/// <summary>
	/// 쭉 이어진 선분으로 구성된 충돌체입니다. (AKA line strip)
	/// </summary>
	class PolylineCollider2D : public Collider2D {
	public:
		inline PolylineCollider2D(Entity* entity, const std::vector<vec2>& points) :Collider2D(entity), basePoints(points) {  }
	private:
		std::vector<vec2> basePoints;	// transform 상대 오프셋, 스케일에도 영향 받음
	};
}

#endif // !__OA_COLLIDER_H__

