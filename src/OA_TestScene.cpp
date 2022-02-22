#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"

namespace onart {
	void TestScene::Update() { Entity::get<UI::Text>("te2")->setRotation(Quaternion::euler(tp, 0, 0)); Entity::get<UI::Text>("te2")->setColor(fabs(sinf(tp))); }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'°¡' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("tet", Font::get("arial"), u"A\bffff00ffB\ax3.00C", vec2()));
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"°¡°¡\nA", vec4(-0.5f, 0, 1, 1)));
	}
}