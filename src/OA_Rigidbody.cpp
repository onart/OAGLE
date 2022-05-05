#include "OA_Rigidbody.h"
#include "OA_Transform.h"

extern float dt;

namespace onart{

	void Rigidbody2D::Update() {
		velocity += acceleration * dt;
		velocity += netForce * inverseMass * dt;
		transform->addPosition(vec3(velocity, 0));
	}
}