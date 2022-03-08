#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"
#include "OA_Random.h"
#include "OA_Audio.h"
namespace onart {
	pSafeAudioStream p;
	void TestScene::Update() { Entity::get<UI::ToggleButton>("button1")->move(vec4(cosf(tp), sinf(tp), 1, 1)); }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'°¡' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"°¡°¡\nACCCCC", vec2(0), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
		entities.push_back(new UI::ToggleButton("button1", vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-0.5f, -0.5f, 1, 1), nullptr));
	}
}