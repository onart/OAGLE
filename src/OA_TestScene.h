#ifndef __OA_TESTSCENE_H__
#define __OA_TESTSCENE_H__

#include "OA_Scene.h"

namespace onart {
	class TestScene: public Scene
	{
	public:
		void init();
	private:
		void Update();
	};
}

#endif // !__OA_TESTSCENE_H__


