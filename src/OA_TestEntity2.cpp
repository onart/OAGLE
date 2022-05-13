#include "OA_TestEntity2.h"

namespace onart {
	TestEntity2::TestEntity2() :Entity("2danim", Transform(vec3(0, -0.5f, 0.1f), vec3(1)), false, false, true), rb(100000, 1000, transform), bc(this, 0.2f, vec2(0, 0), &rb) {
		pAnimation anim1 = Animation2D::get("돌려차기");
		addAnim(anim1);
	}

	void TestEntity2::Update() {

	}
}