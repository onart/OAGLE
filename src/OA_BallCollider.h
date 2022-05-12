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
		static constexpr float WORLD_RANGE = 100.0f;	// ���� ����: x,y�� +- WORLD_RANGE
		static constexpr float ONE_GRID = WORLD_RANGE * 2 / PARTS;
		static constexpr float INV_ONE_GRID = 1 / ONE_GRID;
	public:
		BallCollider2D(Entity* entity, float radius, const vec2& offset = 0.0f, Rigidbody2D* body = nullptr, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
		~BallCollider2D();
		bool isActive = true;	// false�� ��� ���� ��ü�� �������ϴ�.
		const int surface;
		inline range_t rx() const { return rangex; }
		inline range_t ry() const { return rangey; }
		inline float setRadius() const { return radius; }
		inline void setOffset(const vec2& off) { offset = off; }
		inline bool coarseCheck(BallCollider2D* other) { return (rangex & other->rx()) && (rangey & other->ry()); }
		inline Entity* getEntity() { return entity; }
		void render();
	private:
		void range();	// �� �������� ���� ���ӵ�, ��ġ, ���� ������ ����մϴ�.
		range_t rangex, rangey;
		vec2 offset;	// Entity�� �����߽����� ĥ �� �浹ü �߽��� ��ġ
		vec4 pos_vel;
		Entity* const entity;
		Rigidbody2D* const body;
		float radius;	// �⺻ ������
		static std::vector<BallCollider2D*> objs;
	};
}

#endif // !__OA_BALLCOLLIDER_H__

