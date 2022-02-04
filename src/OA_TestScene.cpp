#include "OA_TestScene.h"
#include "OA_Audio.h"
#include "oagle_simd.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() { static int x = 0; if (x++ == 60) { Audio::Source::load("000.ogg")->play(false); Audio::Source::get("000.ogg")->setVolume(0.5f); Audio::setMasterVolume(0.3f); } }
	void TestScene::init() { auto s = Audio::Source::load("arb.wav"); s->play(true); }
}