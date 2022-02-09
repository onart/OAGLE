#include "OA_TestEntity.h"
#include "OA_Anim.h"
#include "OA_Material.h"
#include "OA_Camera.h"

namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform()) {
		unsigned tex = Material::genTextureFromFile("2danim.png");
		Animation* anim1 = Animation2D::make("돌려차기", true,
			{ Keypoint<unsigned>{0,tex} },
			// RECT 영역 부분. 사진의 가로는 1485px, 세로는 835px이다. 좌측 끝 x좌표, 아래 끝 y좌표(가장 위=835로 가정), 폭, 높이를 모두 (0,1)로 정규화.
			{
				Keypoint<vec4>{0,vec4(253.0f / 1485,1 - 394.0f / 835,223.0f / 1485,288.0f / 835) },
				Keypoint<vec4>{0.1f,vec4(540.0f / 1485,1 - 397.0f / 835,205.0f / 1485,294.0f / 835) },
				Keypoint<vec4>{0.2f,vec4(834.0f / 1485,1 - 388.0f / 835,290.0f / 1485,300.0f / 835) },
				Keypoint<vec4>{0.3f,vec4(1139.0f / 1485,1 - 384.0f / 835,228.0f / 1485,302.0f / 835) },
				Keypoint<vec4>{0.6f,vec4(253.0f / 1485,1 - 394.0f / 835,223.0f / 1485,288.0f / 835) },
			},
			// 피벗 부분. 주어진 그림(2danim.png)를 기준으로 발꿈치를 중심으로 설정했으며 이는 이동/회전/스케일링의 중심이 됨
			{
				vec2(110.0f / 223,16.0f / 288),
				vec2(119.0f / 205,12.0f / 294),
				vec2(113.0f / 290,9.0f / 300),
				vec2(110.0f / 228,15.0f / 302),
				vec2(110.0f / 223,16.0f / 288),
			});
		addAnim("돌려차기");
		// 기본 직사각형 객체는 정사각형임. 따라서 그 안에 전체가 들어가면서 비율을 맞추려면 기본적으로 이렇게 스케일을 조정해야 함.
		transform.setScale(vec3(1, 835.0f / 1485, 1));
		/* TODO: 위 과정의 코드를 직관적으로 만들 필요가 있음
		* 1. 텍스처 키포인트는 이름을 입력받도록 변경. 내부 처리는 unsigned로
		* 2. RECT 및 피벗 입력은 픽셀 단위로 입력하도록 변경. 1에서 텍스처 이름을 받았으므로 가로 세로를 거기서 나누는 게 나을 것 같음
		* 3. 원본 그림의 비율을 유지하기 위해 추가적으로 코드를 입력할 필요가 없어야 좋을 것(29행)
		*/
	}
	void TestEntity::Update() { if (Input::isKeyPressed(Input::KeyCode::space)) { transform.addRotation(vec3(0, 0, 1), dt); } }
}