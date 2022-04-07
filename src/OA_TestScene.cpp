#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"
#include "OA_Random.h"
#include "OA_Audio.h"

namespace onart {
	pSafeAudioStream p;
	void TestScene::Update() {  }
	void TestScene::init() {
		
		Font::load("ariblk.ttf", { 'A','B','C','l',u'가' }, 480, "arial");
		addEntity(new TestEntity());
		addEntity(new UI::Text("te2", Font::get("arial"), u"가가\nACCCCC", vec2(0.f), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
		addEntity(new UI::GaugeV("gv", vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-0.5f, -0.2f, 0.1f, 1), vec2(2), 1, nullptr));
		addEntity(new UI::GaugeH("gh", vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1, -0.5f, 1, 0.1f), vec2(2), 10, nullptr));
	}
}