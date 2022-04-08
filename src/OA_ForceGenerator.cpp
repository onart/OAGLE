/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Copyright (c) 2003-2009 Ian Millington
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_ForceGenerator.h"
#include "OA_PointMass.h"
#include <algorithm>

namespace onart {

	std::map<PointMass*, std::vector<ForceGenerator*>> R_ForcePoint::pmreg;
	std::map<ForceGenerator*, std::vector<PointMass*>> R_ForcePoint::fgreg;
	std::map<PointMass2D*, std::vector<ForceGenerator2D*>> R_ForcePoint2D::pmreg;
	std::map<ForceGenerator2D*, std::vector<PointMass2D*>> R_ForcePoint2D::fgreg;
	std::map<RigidBody*, std::vector<ForceGenerator*>> R_ForceRigid::rbreg;
	std::map<ForceGenerator*, std::vector<RigidBody*>> R_ForceRigid::fgreg;

	void DragGenerator::generate(PointMass* pm) {
		const vec3& v = pm->getVelocity();
		pm->addForce(-v * (k1 + k2 * v.length()));
	}

	void DragGenerator::generate(RigidBody* rb) {
		const vec3& v = rb->getVelocity();
		rb->addForce(-v * (k1 + k2 * v.length()));
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

	void HookeSpringRigid::generate(RigidBody* rb) {
		vec3 lws = rb->getPosition() + attached;
		vec3 ows = other->getPosition() + otherAttached;
		vec3 f = lws - ows;
		float mag = f.length();
		mag = fabs(mag - restLength) * k;
		f.normalize();
		f *= mag;
		rb->addForceAtPoint(f, lws);
	}

	void R_ForcePoint::add(PointMass* pm, ForceGenerator* fg) {
		pmreg[pm].push_back(fg);
		fgreg[fg].push_back(pm);
	}

	void R_ForcePoint::cascade(PointMass* pm) {
		auto vec = pmreg.find(pm);
		if (vec == pmreg.end()) return;
		for (ForceGenerator* fg : vec->second) {
			auto& fgv = fgreg[fg];
			fgv.erase(std::remove(fgv.begin(), fgv.end(), pm), fgv.end());
		}
		pmreg.erase(pm);
	}

	void R_ForcePoint::cascade(ForceGenerator* fg) {
		auto vec = fgreg.find(fg);
		if (vec == fgreg.end()) return;
		for (PointMass* pm : vec->second) {
			auto& pmv = pmreg[pm];
			pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		}
		fgreg.erase(fg);
	}

	void R_ForcePoint::remove(PointMass* pm, ForceGenerator* fg) {
		auto& pmv = pmreg[pm];
		pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		auto& fgv = fgreg[fg];
		fgv.erase(std::remove(fgv.begin(), fgv.end(), pm), fgv.end());
	}

	void R_ForcePoint::clear() {
		pmreg.clear();
		fgreg.clear();
	}

	void R_ForcePoint::Update() {
		for (auto& p : fgreg) {
			ForceGenerator* fg = p.first;
			for (PointMass* pm : p.second) {
				fg->generate(pm);
			}
		}
	}

	void R_ForcePoint2D::add(PointMass2D* pm, ForceGenerator2D* fg) {
		pmreg[pm].push_back(fg);
		fgreg[fg].push_back(pm);
	}

	void R_ForcePoint2D::cascade(PointMass2D* pm) {
		auto vec = pmreg.find(pm);
		if (vec == pmreg.end()) return;
		for (ForceGenerator2D* fg : vec->second) {
			auto& fgv = fgreg[fg];
			fgv.erase(std::remove(fgv.begin(), fgv.end(), pm), fgv.end());
		}
		pmreg.erase(pm);
	}

	void R_ForcePoint2D::cascade(ForceGenerator2D* fg) {
		auto vec = fgreg.find(fg);
		if (vec == fgreg.end()) return;
		for (PointMass2D* pm : vec->second) {
			auto& pmv = pmreg[pm];
			pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		}
		fgreg.erase(fg);
	}

	void R_ForcePoint2D::remove(PointMass2D* pm, ForceGenerator2D* fg) {
		auto& pmv = pmreg[pm];
		pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		auto& fgv = fgreg[fg];
		fgv.erase(std::remove(fgv.begin(), fgv.end(), pm), fgv.end());
	}

	void R_ForcePoint2D::clear() {
		pmreg.clear();
		fgreg.clear();
	}

	void R_ForcePoint2D::Update() {
		for (auto& p : fgreg) {
			ForceGenerator2D* fg = p.first;
			for (PointMass2D* pm : p.second) {
				fg->generate(pm);
			}
		}
	}

	void R_ForceRigid::Update() {
		for (auto& p : fgreg) {
			ForceGenerator* fg = p.first;
			for (RigidBody* rb : p.second) {
				fg->generate(rb);
			}
		}
	}

	void R_ForceRigid::add(RigidBody* rb, ForceGenerator* fg) {
		rbreg[rb].push_back(fg);
		fgreg[fg].push_back(rb);
	}

	void R_ForceRigid::cascade(ForceGenerator* fg) {
		auto vec = fgreg.find(fg);
		if (vec == fgreg.end()) return;
		for (RigidBody* pm : vec->second) {
			auto& pmv = rbreg[pm];
			pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		}
		fgreg.erase(fg);
	}

	void R_ForceRigid::cascade(RigidBody* rb) {
		auto vec = rbreg.find(rb);
		if (vec == rbreg.end()) return;
		for (ForceGenerator* fg : vec->second) {
			auto& fgv = fgreg[fg];
			fgv.erase(std::remove(fgv.begin(), fgv.end(), rb), fgv.end());
		}
		rbreg.erase(rb);
	}

	void R_ForceRigid::remove(RigidBody* rb, ForceGenerator* fg) {
		auto& pmv = rbreg[rb];
		pmv.erase(std::remove(pmv.begin(), pmv.end(), fg), pmv.end());
		auto& fgv = fgreg[fg];
		fgv.erase(std::remove(fgv.begin(), fgv.end(), rb), fgv.end());
	}

	void R_ForceRigid::clear() {
		rbreg.clear();
		fgreg.clear();
	}
}