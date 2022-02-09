#include "OA_TestScene.h"
#include "OA_TestEntity.h"

namespace onart {
	void TestScene::Update() {  }
	void TestScene::init() { entities.push_back(new TestEntity()); }
}