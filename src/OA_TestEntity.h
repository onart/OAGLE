#ifndef __OA_TESTENTITY_H__
#define __OA_TESTENTITY_H__

#include "OA_Entity.h"
#include "OA_Rigidbody.h"
#include "OA_BallCollider.h"

namespace onart {
	class TestEntity : public Entity
	{
	public:
		virtual void Update();
		virtual void Act(int kp, float progress);
		TestEntity();
		inline void onTrigger(Entity* other) {  }
	private:
		Rigidbody2D rb;
		BallCollider2D bc;
	};
}

#endif // !__OA_TESTENTITY_H__

