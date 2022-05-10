#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"
#include "OA_PhysicalSurface.h"
#include "OA_Entity.h"

namespace onart {
	class Rigidbody2D;

	/// <summary>
	/// 2���� �浹ü�Դϴ�.
	/// </summary>
	class Collider2D {
		friend class PhysicalSys2D;
		friend class HalfPartition2D;
	public:
		Collider2D(Entity* entity, float radius, const vec2& offset = 0.0f, Rigidbody2D* body = nullptr, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
		bool isActive = true;	// false�� ��� ���� ��ü�� �������ϴ�.
		inline Entity* getEntity() { return tr; }
		const int surface;
		/// <summary>
		/// ��ü �߽ɰ��� ��� ��ġ�� �����մϴ�.
		/// </summary>
		inline void setOffset(const vec2& off) { offset = vec3(off, 0); }
		/// <summary>
		/// ���� �浹ü�� ���Ͽ� ũ�⸦ �����մϴ�.
		/// </summary>
		inline void setScale(float scale) { this->scale = scale; }
		~Collider2D();
	protected:
		/// <summary>
		/// �� �浹ü�� ���� �浹�� Ȯ���մϴ�.
		/// </summary>
		virtual vec2 check(const vec2& center, float rad);
		/// <summary>
		/// �� �浹ü�� ������ �浹�� Ȯ���մϴ�.
		/// </summary>
		virtual vec2 check(const vec2& p1, const vec2& p2);
		/// <summary>
		/// �������� üũ
		/// </summary>
		inline bool check0(Collider2D* other) {
			vec2 p1 = getPos();
			vec2 p2 = other->getPos();
			float rs = baseRadius + other->baseRadius;
			return p1.distance2(p2) <= rs * rs;
		}
		inline vec2 getPos() { return colPos; }
		virtual void savePosc();
		// ȸ���� ���� ����� ������ �ʴ� �� �ٿ��
		Entity* const tr;
		Rigidbody2D* const body;	// ��ü�� ���� ���� ��� ������ ������ ������ ��ȣ�ۿ��� ������ �ʽ��ϴ�.
		float baseRadius, scale;
		vec3 offset;	// transform ��� ������
		vec2 colPos;
		static std::vector<Collider2D*> objs;
	};

	using CircleCollider2D = Collider2D;

	/// <summary>
	/// �� �̾��� �������� ������ �浹ü�Դϴ�. (AKA line strip)
	/// </summary>
	class PolylineCollider2D : public Collider2D {
	public:
		/// <summary>
		/// �ٰ��� �浹ü�� �����մϴ�.
		/// </summary>
		/// <param name="entity">���� ��ü(null�̸� �� �˴ϴ�.)</param>
		/// <param name="body">���� ��ü(null�̾ �˴ϴ�.)</param>
		/// <param name="closed">true�� ��� ù���� ������ ����˴ϴ�.</param>
		/// <param name="points">��ü �߽� ���� ���� �������� ��ġ�Դϴ�.</param>
		/// <param name="surface">ǥ�� ����(PhysicalSurface.h)</param>
		PolylineCollider2D(Entity* entity, Rigidbody2D* body, bool closed, const std::vector<vec2>& points, PHYSICAL_SURFACE surface = PHYSICAL_SURFACE::DEFAULT_SURFACE);
	protected:
		vec2 check(const vec2& p1, const vec2& p2);
		vec2 check(const vec2& center, float rad);
	private:
		bool closed;
		std::vector<vec4> basePoints;	// transform ��� ������, �����Ͽ��� ���� ����
		std::vector<vec2> transformed;	// ���� ������
		/// <summary>
		/// ���� �������� ���� ��ġ�� ����
		/// </summary>
		void savePosc();
		inline bool check1(Collider2D* other) {
			sizeof(*this);
			return true;
		}
		/*
		��� 1: �Ź� ���簢�� ������ ����ϱ�(�ĺ��� ���� ������ ������ �������� �ſ� ��ȿ����)
		��� 2 : ���� �ٿ��(���簢������ Ȯ���� �ſ� ����)�� ����� �� ȸ���� ����� �߽ɸ� �̵�
		��� 3 : ȸ���� �߽ɿ��� ���� �� �� �ϳ������� ū ���� �ٿ���� ����(������ ����� ��귮�� ũ�� ���������� ��쿡 ���� ������ �ſ� Ŀ�� �浹 �ĺ��� ������ ���ɼ��� ����)
		*/
	};
}

#endif // !__OA_COLLIDER_H__

