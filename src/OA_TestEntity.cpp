#include "OA_TestEntity.h"
#include "OA_Anim.h"
#include "OA_Material.h"
#include "OA_Camera.h"

namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform()) {
		unsigned tex = Material::genTextureFromFile("2danim.png");
		Animation* anim1 = Animation2D::make("��������", true,
			{ Keypoint<unsigned>{0,tex} },
			// RECT ���� �κ�. ������ ���δ� 1485px, ���δ� 835px�̴�. ���� �� x��ǥ, �Ʒ� �� y��ǥ(���� ��=835�� ����), ��, ���̸� ��� (0,1)�� ����ȭ.
			{
				Keypoint<vec4>{0,vec4(253.0f / 1485,1 - 394.0f / 835,223.0f / 1485,288.0f / 835) },
				Keypoint<vec4>{0.1f,vec4(540.0f / 1485,1 - 397.0f / 835,205.0f / 1485,294.0f / 835) },
				Keypoint<vec4>{0.2f,vec4(834.0f / 1485,1 - 388.0f / 835,290.0f / 1485,300.0f / 835) },
				Keypoint<vec4>{0.3f,vec4(1139.0f / 1485,1 - 384.0f / 835,228.0f / 1485,302.0f / 835) },
				Keypoint<vec4>{0.6f,vec4(253.0f / 1485,1 - 394.0f / 835,223.0f / 1485,288.0f / 835) },
			},
			// �ǹ� �κ�. �־��� �׸�(2danim.png)�� �������� �߲�ġ�� �߽����� ���������� �̴� �̵�/ȸ��/�����ϸ��� �߽��� ��
			{
				vec2(110.0f / 223,16.0f / 288),
				vec2(119.0f / 205,12.0f / 294),
				vec2(113.0f / 290,9.0f / 300),
				vec2(110.0f / 228,15.0f / 302),
				vec2(110.0f / 223,16.0f / 288),
			});
		addAnim("��������");
		// �⺻ ���簢�� ��ü�� ���簢����. ���� �� �ȿ� ��ü�� ���鼭 ������ ���߷��� �⺻������ �̷��� �������� �����ؾ� ��.
		transform.setScale(vec3(1, 835.0f / 1485, 1));
		/* TODO: �� ������ �ڵ带 ���������� ���� �ʿ䰡 ����
		* 1. �ؽ�ó Ű����Ʈ�� �̸��� �Է¹޵��� ����. ���� ó���� unsigned��
		* 2. RECT �� �ǹ� �Է��� �ȼ� ������ �Է��ϵ��� ����. 1���� �ؽ�ó �̸��� �޾����Ƿ� ���� ���θ� �ű⼭ ������ �� ���� �� ����
		* 3. ���� �׸��� ������ �����ϱ� ���� �߰������� �ڵ带 �Է��� �ʿ䰡 ����� ���� ��(29��)
		*/
	}
	void TestEntity::Update() { if (Input::isKeyPressed(Input::KeyCode::space)) { transform.addRotation(vec3(0, 0, 1), dt); } }
}