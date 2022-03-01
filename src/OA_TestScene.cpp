#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_Camera.h"
#include "OA_UI.h"
#include "OA_Random.h"
#include "OA_Audio.h"
namespace onart {
	pSafeAudioStream p;
	void TestScene::Update() { static int i = 0; if (++i % 100 == 0) { printf("P\n"); p.pause(); } else if (i % 50 == 0) { p.resume(); printf("R\n"); } }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'°¡' }, 480, "arial"); 
		entities.push_back(new TestEntity());
		entities.push_back(new UI::Text("te2", Font::get("arial"), u"°¡°¡\nACCCCC", vec2(0), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
		p = Audio::Source::load("040.ogg")->playSafe();
	}
}