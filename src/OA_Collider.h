#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"
#include "OA_PhysicalSurface.h"

namespace onart {
	class Entity;

	/// <summary>
	/// 2차원 충돌체입니다.
	/// </summary>
	class Collider2D {
		friend class PhysicalSys2D;
	public:
		inline Collider2D(Entity* entity, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE) :tr(entity), surface((int)surface) {}
		bool isActive = true;
		inline Entity* getEntity() { return tr; }
		const int surface;
	protected:
		float baseRadius;
		Entity* tr;
		static std::vector<Collider2D*> objs;
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
		PolylineCollider2D(Entity* entity, const std::vector<vec2>& points);
	private:
		std::vector<vec4> basePoints;	// transform 상대 오프셋, 스케일에도 영향 받음
		float baseRadius;
		vec2 baseOffset;
		/*
		방법 1: 매번 직사각형 범위를 계산하기(후보가 가장 적지만 지점이 많을수록 매우 비효율적)
		방법 2 : 원형 바운딩(직사각형으로 확장이 매우 쉬움)을 계산한 후 회전이 생기면 중심만 이동
		방법 3 : 회전의 중심에서 가장 먼 점 하나만으로 큰 원형 바운딩을 만듦(보통의 경우의 계산량이 크게 감소하지만 경우에 따라 영역이 매우 커져 충돌 후보가 많아질 가능성이 있음)
		*/
	};
}

#endif // !__OA_COLLIDER_H__

