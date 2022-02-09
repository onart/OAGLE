#ifndef __OA_TESTENTITY_H__
#define __OA_TESTENTITY_H__

#include "OA_Entity.h"

namespace onart {
	class TestEntity : public Entity
	{
	public:
		virtual void Update();
		TestEntity();
	private:
	};
}

#endif // !__OA_TESTENTITY_H__

