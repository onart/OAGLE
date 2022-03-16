#include "OA_TestEntity.h"
#include "OA_Material.h"
#include "OA_Camera.h"
#include "OA_Window.h"
namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform()) {
		pAnimation anim1 = Animation2D::make("��������", true,
			{ Keypoint<pTexture>{0,Material::genTextureFromFile("2danim.png")} },
			{
				Keypoint<vec4>{0,vec4(253,441,223,288) },
				Keypoint<vec4>{0.1f,vec4(540,438,205,294) },
				Keypoint<vec4>{0.2f,vec4(836,446,300,298) },
				Keypoint<vec4>{0.3f,vec4(1136,462,227,294) },
				Keypoint<vec4>{0.6f,vec4(253,441,223,288) },
			},
			// �ǹ� �κ�. �־��� �׸�(2danim.png)�� �������� �߲�ġ�� �߽����� ���������� �̴� �̵�/ȸ��/�����ϸ��� �߽��� ��
			{
				vec2(110,16),
				vec2(119,12),
				vec2(107,12),
				vec2(112,8),
				vec2(110,16),
			});
		addAnim(anim1);
		pAnimation anim2 = UIAnimation::make("u��", true,
			{ Keypoint<pTexture>{0,Material::genTextureFromFile("2danim.png")} },
			{
				Keypoint<vec4>{0,vec4(253,441,223,288) },
				Keypoint<vec4>{0.1f,vec4(540,438,205,294) },
				Keypoint<vec4>{0.2f,vec4(836,446,300,298) },
				Keypoint<vec4>{0.3f,vec4(1136,462,227,294) },
				Keypoint<vec4>{0.6f,vec4(253,441,223,288) },
			},
			// �ǹ� �κ�. �־��� �׸�(2danim.png)�� �������� �߲�ġ�� �߽����� ���������� �̴� �̵�/ȸ��/�����ϸ��� �߽��� ��
			{
				vec2(110,16),
				vec2(119,12),
				vec2(107,12),
				vec2(112,8),
				vec2(110,16),
			}
			);
		window::setCursorImage(anim2, vec2(0.4f));
	}
	void TestEntity::Update() { if (Input::isKeyPressedNow(Input::KeyCode::space)) { window::setCursorImage(); } }

	void TestEntity::Act(int kp, float progress) {  }
}