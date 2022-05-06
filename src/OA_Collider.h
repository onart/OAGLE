#ifndef __OA_COLLIDER_H__
#define __OA_COLLIDER_H__

#include <vector>
#include "oaglem.h"
#include "OA_PhysicalSurface.h"

namespace onart {
	class Entity;

	/// <summary>
	/// 2���� �浹ü�Դϴ�.
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
		PolylineCollider2D(Entity* entity, const std::vector<vec2>& points);
	private:
		std::vector<vec4> basePoints;	// transform ��� ������, �����Ͽ��� ���� ����
		float baseRadius;
		vec2 baseOffset;
		/*
		��� 1: �Ź� ���簢�� ������ ����ϱ�(�ĺ��� ���� ������ ������ �������� �ſ� ��ȿ����)
		��� 2 : ���� �ٿ��(���簢������ Ȯ���� �ſ� ����)�� ����� �� ȸ���� ����� �߽ɸ� �̵�
		��� 3 : ȸ���� �߽ɿ��� ���� �� �� �ϳ������� ū ���� �ٿ���� ����(������ ����� ��귮�� ũ�� ���������� ��쿡 ���� ������ �ſ� Ŀ�� �浹 �ĺ��� ������ ���ɼ��� ����)
		*/
	};
}

#endif // !__OA_COLLIDER_H__

