#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"

namespace onart {
	class Entity;

	/// <summary>
	/// 2���� �浹ü�Դϴ�.
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
	/// ���簢�� �浹ü�Դϴ�.
	/// </summary>
	class BoxCollider2D :public Collider2D {
	public:
		inline BoxCollider2D(Entity* entity, const vec4& ldwh) :baseRect(ldwh), Collider2D(entity) { }
	private:
		vec4 baseRect;		// LDWH, LD�� transform ��� ������, WH�� �����Ͽ� ������ ����
	};

	/// <summary>
	/// ���� �浹ü�Դϴ�.
	/// </summary>
	class CircleCollider2D :public Collider2D {
	public:
		inline CircleCollider2D(Entity* entity, const vec2& offset, float baseRadius) :Collider2D(entity), offset(offset), baseRadius(baseRadius) { }
	private:
		float baseRadius;
		vec2 offset;	// transform ��� ������
	};

	/// <summary>
	/// �� �̾��� �������� ������ �浹ü�Դϴ�. (AKA line strip)
	/// </summary>
	class PolylineCollider2D : public Collider2D {
	public:
		inline PolylineCollider2D(Entity* entity, const std::vector<vec2>& points) :Collider2D(entity), basePoints(points) {  }
	private:
		std::vector<vec2> basePoints;	// transform ��� ������, �����Ͽ��� ���� ����
	};
}

#endif // !__OA_COLLIDER_H__

