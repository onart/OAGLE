#include "OA_TestScene.h"
#include "OA_Audio.h"
#include "oagle_simd.h"
#include <cstdio>

namespace onart {
	void TestScene::Update() {  }
	void TestScene::init() { auto s = Audio::Source::load("lt2.mp3"); s->play(false); }
}