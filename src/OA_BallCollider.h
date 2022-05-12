#ifndef __OA_BALLCOLLIDER_H__
#define __OA_BALLCOLLIDER_H__

#include <stdint.h>
#include <vector>
#include "oaglem.h"
#include "OA_Entity.h"
#include "OA_PhysicalSurface.h"

namespace onart {
	class Rigidbody2D;
	class BallCollider2D {
		friend class Ballpool2D;
		using range_t = uint64_t;
		static constexpr int PARTS = sizeof(range_t) * 8;
		static constexpr float WORLD_RANGE = 100.0f;	// 분할 범위: x,y축 +- WORLD_RANGE
		static constexpr float ONE_GRID = WORLD_RANGE * 2 / PARTS;
		static constexpr float INV_ONE_GRID = 1 / ONE_GRID;
	public:
		BallCollider2D(Entity* entity, float radius, const vec2& offset = 0.0f, Rigidbody2D* body = nullptr, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
		~BallCollider2D();
		bool isActive = true;	// false일 경우 판정 자체가 없어집니다.
		const int surface;
		inline range_t rx() const { return rangex; }
		inline range_t ry() const { return rangey; }
		inline float setRadius() const { return radius; }
		inline void setOffset(const vec2& off) { offset = off; }
		inline bool coarseCheck(BallCollider2D* other) { return (rangex & other->rx()) && (rangey & other->ry()); }
		inline Entity* getEntity() { return entity; }
		void render();
	private:
		void range();	// 현 프레임의 순간 선속도, 위치, 개괄 영역을 계산합니다.
		range_t rangex, rangey;
		vec2 offset;	// Entity를 질량중심으로 칠 때 충돌체 중심의 위치
		vec4 pos_vel;
		Entity* const entity;
		Rigidbody2D* const body;
		float radius;	// 기본 반지름
		static std::vector<BallCollider2D*> objs;
	};
}

#endif // !__OA_BALLCOLLIDER_H__

