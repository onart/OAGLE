#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Text.h"
#include "OA_Camera.h"

namespace onart {
	void TestScene::Update() { print(Input::cursorPos()); }
	void TestScene::init() { Font::load("ariblk.ttf", { 'A','B','C','l',u'°¡' }, 480, "arial"); entities.push_back(new TestEntity()); }
}