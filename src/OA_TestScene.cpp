#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"

namespace onart {
	void TestScene::Update() { auto x = Entity::get<UI::Text>("te2"); static int i = 0; if (Input::isKeyPressed(Input::KeyCode::C)) { x->setRotation(Quaternion::rotation(vec3(0, 0, 1), tp)); } }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'°¡' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"°¡°¡\nACCCCC", vec2(0), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
	}
}