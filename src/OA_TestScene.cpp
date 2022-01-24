#include "OA_TestScene.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() { print(slerp(Quaternion(1, 2, 3, 4), Quaternion(5, 6, 7, 8), .5f)); }
	void TestScene::init() {  }
}