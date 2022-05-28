﻿#include "OA_TestScene.h"
#include "OA_TestEntity.h"
#include "OA_TestEntity2.h"
#include "OA_Camera.h"
#include "OA_UI.h"
#include "OA_Random.h"
#include "OA_Audio2.h"

namespace onart {
	pSafeAudioStream p;
	void TestScene::Update() { if (Input::isKeyPressedNow(Input::KeyCode::A)) { auto x = Audio2::Source::get("004.ogg"); if(x)x->play(); } }
	void TestScene::init() {
		Font::load("ariblk.ttf", { 'A','B','C','l',u'가' }, 480, "arial");
		auto x=Audio2::Source::load("004.ogg");
		x->play();
		//addEntity(new TestEntity());
		//addEntity(new TestEntity2());
		oastring basee = u"\aa2.00가가\nAC\bff0000ffC\bffffffffCCC";
		addEntity(new UI::Text("te2", Font::get("arial"), basee, vec2(0.f), 2000, AlignH::LEFT, AlignV::TOP, 0.3f));
		addEntity(new UI::Text("te3", Font::get("arial"), basee, vec4(0, 0, 1, 1), 2000, false, AlignH::LEFT, 1.0f));
		//addEntity(new UI::GaugeV("gv", vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-0.5f, -0.2f, 0.1f, 1), vec2(2), 1, nullptr));
		//addEntity(new UI::GaugeH("gh", vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1.0f / 1024, -1.0f / 1024, 0, 0), vec4(-1, -0.5f, 1, 0.1f), vec2(2), 10, nullptr));
	}
}
