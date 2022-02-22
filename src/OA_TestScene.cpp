#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"

namespace onart {
	void TestScene::Update() {  }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'가' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"가가가가가가가가가가가\nA", vec4(-0.5f, -0.5f, 1, 1), 1000));
	}
}