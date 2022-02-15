#include "OA_TestEntity.h"
#include "OA_Anim.h"
#include "OA_Material.h"
#include "OA_Camera.h"

namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform()) {
		Animation* anim1 = Animation2D::make("��������", true,
			{ Keypoint<Texture>{0,Material::genTextureFromFile("2danim.png")} },
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
		addAnim("��������");
	}
	void TestEntity::Update() { if (Input::isKeyPressed(Input::KeyCode::space)) { transform.addRotation(vec3(0, 1, 0), dt); } }
}