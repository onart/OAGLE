#ifndef __OA_TESTENTITY2_H__
#define __OA_TESTENTITY2_H__

#include "OA_modules.h"

namespace onart {
	class TestEntity2 :public Entity {
	public:
		virtual void Update();
		TestEntity2();
		inline void onTrigger(Entity* other) {  }
	private:
		Rigidbody2D rb;
		BallCollider2D bc;
	};
}

#endif // !__OA_TESTENTITY2_H__

