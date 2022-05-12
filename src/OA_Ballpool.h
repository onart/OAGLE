#ifndef __OA_BALLPOOL_H__
#define __OA_BALLPOOL_H__

#include "oaglem.h"

namespace onart {
	class BallCollider2D;
	class Ballpool2D {
	public:
		/// <summary>
		/// 2D ���� ��ü�� ������Ʈ�մϴ�.
		/// </summary>
		static void Update();
		/// <summary>
		/// ����׸� ���� ���� �浹ü�� ȭ�鿡 �������ϰ� ǥ���մϴ�.
		/// </summary>
		static void render();
	private:
		/// <summary>
		/// 2D ���� ��ü�� �浹�� Ȯ���ϰ� ������ ���մϴ�.
		/// </summary>
		static void makeCollisions();
		/// <summary>
		/// 2D ���� ��ü�� �浹�� Ȯ���ϰ� ������ ���մϴ�.
		/// </summary>
		/// <param name="b1"></param>
		/// <param name="b2"></param>
		/// <param name="posvel"></param>
		static void collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel);
	};
}

#endif // !__OA_BALLPOOL_H__

