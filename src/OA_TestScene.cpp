#include "OA_TestScene.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() { print(Input::relativeCursorPos(),"cursor"); }
	void TestScene::init() {  }
}