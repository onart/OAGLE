#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"

namespace onart {
	void TestScene::Update() { auto x = Entity::get<UI::Text>("te2"); static int i = 0; if (Input::isKeyPressedNow(Input::KeyCode::C)) { x->setContent(u"C"); } if (Input::isKeyPressedNow(Input::KeyCode::D)) { x->append(u"가"); } }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'가' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"가가\nACCCCC", vec2(0), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
	}
}