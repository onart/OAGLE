#include "OA_ForceGenerator.h"
#include "OA_Physics.h"

namespace onart {
	void DragGenerator::generate(PointMass* pm) {
		const vec3& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void DragGenerator2D::generate(PointMass2D* pm) {
		const vec2& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void HookeSpring::generate(PointMass* pm) {
		vec3 f(pm->getPosition()); f -= other->getPosition();
		float mag = fabs(1 - restLength / f.length()) * k;
		f *= -mag;
		pm->addForce(f);
	}

	void HookeSpring2D::generate(PointMass2D* pm) {
		vec2 f(pm->getPosition()); f -= other->getPosition();
		float mag = fabs(1 - restLength / f.length()) * k;
		f *= -mag;
		pm->addForce(f);
	}

	void HookeBungee::generate(PointMass* pm) {
		vec3 f(pm->getPosition()); f -= other->getPosition();
		float mag = (1 - restLength / f.length()) * k;
		if (mag <= 0) return;
		f *= mag;
		pm->addForce(f);
	}

	void HookeBungee2D::generate(PointMass2D* pm) {
		vec2 f(pm->getPosition()); f -= other->getPosition();
		float mag = (1 - restLength / f.length()) * k;
		if (mag <= 0) return;
		f *= mag;
		pm->addForce(f);
	}
}