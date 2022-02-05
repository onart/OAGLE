#include "OA_TestScene.h"
#include "OA_Audio.h"
#include "oagle_simd.h"
#include <cstdio>
#include <filesystem>

namespace onart {
	void TestScene::Update() {  }
	void TestScene::init() {
		auto size = std::filesystem::file_size("000.ogg");
		void* buf = malloc(size);
		FILE* fp;
		fopen_s(&fp, "000.ogg", "rb");
		if (fp) { fread(buf, 1, size, fp); }
		Audio::Source::load(buf, size, "0")->play(true);
	}
}