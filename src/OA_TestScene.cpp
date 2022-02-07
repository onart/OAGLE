#include "OA_TestScene.h"
#include "OA_Audio.h"
#include "oagle_simd.h"
#include <cstdio>
#include <filesystem>

namespace onart {
	void TestScene::Update() {  }
	void TestScene::init() { Audio::Source::load("lt2.mp3")->play(true); }
}