#include "OA_TestEntity.h"
#include "OA_Anim.h"
#include "OA_Material.h"
#include "OA_Camera.h"
#include "OA_Model.h"
#include  "OA_Shader.h"
extern onart::Camera mainCamera;
extern onart::Shader program3;
namespace onart {
	TestEntity::TestEntity() :Entity("2danim", Transform()) {
		//Animation* anim1 = Animation3D::load("vam", "dvd2.dae", true, {});
		setModel(Model::load("head.obj"));
		
		mainCamera.ratio.setProjMatrix3D(PI / 4, 1, 1000);
		mainCamera.fix(vec3(0, -1, 100), 0);
	}
	void TestEntity::Update() { if (Input::isKeyPressed(Input::KeyCode::space)) { transform.addRotation(vec3(0, 0, 1), dt); } }
}