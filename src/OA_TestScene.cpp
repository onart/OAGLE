#include "OA_TestScene.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() { auto mp=Input::relativeCursorPos(); printf("%f %f\r", mp.x,mp.y); }
	void TestScene::init() {  }
}