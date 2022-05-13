﻿#include "OA_TestEntity.h"
#include "OA_Material.h"
#include "OA_Camera.h"
#include "OA_Window.h"
namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform(vec2(0.02f,1)), false, false, true), rb(1, 1000, transform), bc(this, 0.1f, vec2(0.0f), &rb) {
		pAnimation anim1 = Animation2D::make("돌려차기", true,
			{ Keypoint<pTexture>{0,Material::genTextureFromFile("2danim.png")} },
			{
				Keypoint<vec4>{0,vec4(253,441,223,288) },
				Keypoint<vec4>{0.1f,vec4(540,438,205,294) },
				Keypoint<vec4>{0.2f,vec4(836,446,300,298) },
				Keypoint<vec4>{0.3f,vec4(1136,462,227,294) },
				Keypoint<vec4>{0.6f,vec4(253,441,223,288) },
			},
			// 피벗 부분. 주어진 그림(2danim.png)를 기준으로 발꿈치를 중심으로 설정했으며 이는 이동/회전/스케일링의 중심이 됨
			{
				vec2(110,16),
				vec2(119,12),
				vec2(107,12),
				vec2(112,8),
				vec2(110,16),
			});
		addAnim(anim1);
		pAnimation anim2 = UIAnimation::make("u돌", true,
			{ Keypoint<pTexture>{0,Material::genTextureFromFile("2danim.png")} },
			{
				Keypoint<vec4>{0,vec4(253,441,223,288) },
				Keypoint<vec4>{0.1f,vec4(540,438,205,294) },
				Keypoint<vec4>{0.2f,vec4(836,446,300,298) },
				Keypoint<vec4>{0.3f,vec4(1136,462,227,294) },
				Keypoint<vec4>{0.6f,vec4(253,441,223,288) },
			},
			// 피벗 부분. 주어진 그림(2danim.png)를 기준으로 발꿈치를 중심으로 설정했으며 이는 이동/회전/스케일링의 중심이 됨
			{
				vec2(110,16),
				vec2(119,12),
				vec2(107,12),
				vec2(112,8),
				vec2(110,16),
			}
			);
		window::setCursorImage(anim2, vec2(0.4f));
		rb.addConstantAcceleration(vec2(0, -1.0f));
	}
	void TestEntity::Update() { if (Input::isKeyPressedNow(Input::KeyCode::space)) { window::setCursorImage(); } }

	void TestEntity::Act(int kp, float progress) {  }
}