#include "OA_TestScene.h"
#include "OA_Audio.h"
#include "oagle_simd.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() { static int x = 0; if (x++ == 200) { Audio::Source::load("000.ogg")->play(false); } }
	void TestScene::init() { auto s = Audio::Source::load("arb.wav"); s->play(true); }
}