#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"
#include "OA_PhysicalSurface.h"
#include "OA_Entity.h"

namespace onart {
	class Rigidbody2D;

	/// <summary>
	/// 2차원 충돌체입니다.
	/// </summary>
	class Collider2D {
		friend class PhysicalSys2D;
		friend class HalfPartition2D;
	public:
		Collider2D(Entity* entity, float radius, const vec2& offset = 0.0f, Rigidbody2D* body = nullptr, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
		bool isActive = true;	// false일 경우 판정 자체가 없어집니다.
		inline Entity* getEntity() { return tr; }
		const int surface;
		/// <summary>
		/// 개체 중심과의 상대 위치를 변경합니다.
		/// </summary>
		inline void setOffset(const vec2& off) { offset = vec3(off, 0); }
		/// <summary>
		/// 원형 충돌체에 한하여 크기를 변경합니다.
		/// </summary>
		inline void setScale(float scale) { this->scale = scale; }
		~Collider2D();
	protected:
		/// <summary>
		/// 이 충돌체와 원의 충돌을 확인합니다.
		/// </summary>
		virtual vec2 check(const vec2& center, float rad);
		/// <summary>
		/// 이 충돌체와 선분의 충돌을 확인합니다.
		/// </summary>
		virtual vec2 check(const vec2& p1, const vec2& p2);
		/// <summary>
		/// 원형볼륨 체크
		/// </summary>
		inline bool check0(Collider2D* other) {
			vec2 p1 = getPos();
			vec2 p2 = other->getPos();
			float rs = baseRadius + other->baseRadius;
			return p1.distance2(p2) <= rs * rs;
		}
		inline vec2 getPos() { return colPos; }
		virtual void savePosc();
		// 회전에 의해 모양이 변하지 않는 원 바운딩
		Entity* const tr;
		Rigidbody2D* const body;	// 강체에 붙지 않은 경우 판정은 있지만 물리적 상호작용을 하지는 않습니다.
		float baseRadius, scale;
		vec3 offset;	// transform 상대 오프셋
		vec2 colPos;
		static std::vector<Collider2D*> objs;
	};

	using CircleCollider2D = Collider2D;

	/// <summary>
	/// 쭉 이어진 선분으로 구성된 충돌체입니다. (AKA line strip)
	/// </summary>
	class PolylineCollider2D : public Collider2D {
	public:
		/// <summary>
		/// 다각선 충돌체를 정의합니다.
		/// </summary>
		/// <param name="entity">붙일 개체(null이면 안 됩니다.)</param>
		/// <param name="body">붙일 강체(null이어도 됩니다.)</param>
		/// <param name="closed">true인 경우 첫점과 끝점이 연결됩니다.</param>
		/// <param name="points">개체 중심 기준 선분 끝점들의 위치입니다.</param>
		/// <param name="surface">표면 재질(PhysicalSurface.h)</param>
		PolylineCollider2D(Entity* entity, Rigidbody2D* body, bool closed, const std::vector<vec2>& points, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
	protected:
		vec2 check(const vec2& p1, const vec2& p2);
		vec2 check(const vec2& center, float rad);
	private:
		bool closed;
		std::vector<vec4> basePoints;	// transform 상대 오프셋, 스케일에도 영향 받음
		std::vector<vec2> transformed;	// 절대 오프셋
		/// <summary>
		/// 현재 프레임의 절대 위치를 저장
		/// </summary>
		void savePosc();
		inline bool check1(Collider2D* other) {
			sizeof(*this);
			return true;
		}
		/*
		방법 1: 매번 직사각형 범위를 계산하기(후보가 가장 적지만 지점이 많을수록 매우 비효율적)
		방법 2 : 원형 바운딩(직사각형으로 확장이 매우 쉬움)을 계산한 후 회전이 생기면 중심만 이동
		방법 3 : 회전의 중심에서 가장 먼 점 하나만으로 큰 원형 바운딩을 만듦(보통의 경우의 계산량이 크게 감소하지만 경우에 따라 영역이 매우 커져 충돌 후보가 많아질 가능성이 있음)
		*/
	};
}

#endif // !__OA_COLLIDER_H__

