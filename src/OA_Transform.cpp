/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Transform.h"


namespace onart {
	
	Transform::Transform(const vec3& pos, const vec3& scale, const Quaternion& rot, Transform* parent)
		:pos(pos), scale(scale), rotation(rot), parent(parent) {
		if (parent) parent->addChild(this);
		TRS();
	}

	Transform::Transform(const Transform& tr)
		: pos(tr.getLocalPosition()), rotation(tr.getLocalRotation()), scale(tr.getLocalScale()), parent(tr.getParent()) { 
		if (parent) parent->addChild(this);
		TRS();
	}

	Transform::Transform(const mat4& tr, Transform* parent)
		: model(tr), parent(parent) {
		if (parent)parent->addChild(this);
		mat2prs();
	}

	void Transform::setParent(Transform* p) {
		if (p == this) p = nullptr;
		if (parent) { parent->excludeChild(this); }
		parent = p;
		if (parent) {
			parent->addChild(this);
			model = parent->getInverseTransform() * globalModel;
		}
		else {
			model = globalModel;
		}
		mat2prs();
	}

	void Transform::mat2prs() {
		pos = model.col(4);
		scale = model.row(0);
		vec3 rsr2(model.row(1)), rsr3(model.row(2));
		scale *= scale;	rsr2 *= rsr2;	rsr3 *= rsr3;
		scale += rsr2;	scale += rsr3;
		sqrt4(scale.entry);
		mat4 rot(model * mat4::scale(vec3(1) / scale));
		float tr = rot.trace() - rot[15];		// = 2cosx + 1
		float cos2 = (tr + 1) * 0.25f;			// = (1 + cosx) / 2 = cos^2(x/2)
		float sin2 = 1 - cos2;
		float c = (tr - 1) * 0.5f;	// = cosx
		c = c > 1 ? 1 : c;
		float s = sqrtf(1 - c * c);
		if (s <= FLT_EPSILON) {
			memset(&rotation, 0, sizeof(rotation));
			rotation.c1 = 1;
		}
		else {
			rotation.ci = (rot.at(2, 1) - rot.at(1, 2));	// 2ci * sinx
			rotation.cj = (rot.at(0, 2) - rot.at(2, 0));	// 2cj * sinx
			rotation.ck = (rot.at(1, 0) - rot.at(0, 1));	// 2ck * sinx
			rotation *= 0.5f / s * sqrtf(sin2);
			rotation.c1 = sqrtf(cos2);
		}
		ready = true;
	}

	void Transform::gmat2prs() {
		pos = globalModel.col(4);
		globalScale = globalModel.row(0);
		vec3 rsr2(globalModel.row(1)), rsr3(globalModel.row(2));
		globalScale *= globalScale;	rsr2 *= rsr2;	rsr3 *= rsr3;
		globalScale += rsr2;	globalScale += rsr3;
		sqrt4(globalScale.entry);
		mat4 rot(globalModel * mat4::scale(vec3(1) / globalScale));
		float tr = rot.trace() - rot[15];		// = 2cosx + 1
		float cos2 = (tr + 1) * 0.25f;			// = (1 + cosx) / 2 = cos^2(x/2)
		float sin2 = 1 - cos2;
		float c = (tr - 1) * 0.5f;	// = cosx
		c = c > 1 ? 1 : c;
		float s = sqrtf(1 - c * c);
		if (s <= FLT_EPSILON) {
			memset(&globalRotation, 0, sizeof(globalRotation));
			globalRotation.c1 = 1;
		}
		else {
			globalRotation.ci = (rot.at(2,1) - rot.at(1,2));	// 2ci * sinx
			globalRotation.cj = (rot.at(0,2) - rot.at(2,0));	// 2cj * sinx
			globalRotation.ck = (rot.at(1,0) - rot.at(0,1));	// 2ck * sinx
			globalRotation *= 0.5f / s * sqrtf(sin2);
			globalRotation.c1 = sqrtf(cos2);
		}
	}

	const mat4& Transform::getModel() {
		if (!ready) TRS();
		else if (globalReady) return globalModel;
		globalReady = true;
		if (parent) { 
			globalModel = parent->getModel() * model;
			gmat2prs();
			return globalModel;
		}
		else { 
			return globalModel = model;
		}
	}

	void Transform::globalNotReady() {
		if (globalReady) {
			globalReady = false;
			for (Transform* ch : children) ch->globalNotReady();
		}
	}

	mat4 Transform::getInverseTransform() {
		getModel();
		return mat4::iTRS(globalPos, globalRotation, globalScale);
	}

	void Transform::setPosition(const vec3& p) {
		if (!parent)return setLocalPosition(p);
		// FACT: 부모 변환이 아핀 변환일 때 글로벌 포지션을 움직이고 자식 변환에 반영할 경우, 자식 변환 또한 위치만 변함
		getModel();
		globalModel.at(0, 3) = p[0];	globalModel.at(1, 3) = p[1];	globalModel.at(2, 3) = p[2];
		globalPos = p;
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::addPosition(const vec3& p) {
		if (!parent)return addLocalPosition(p);
		getModel();
		globalPos += p;
		globalModel.at(0, 3) = globalPos[0];	globalModel.at(1, 3) = globalPos[1];	globalModel.at(2, 3) = globalPos[2];
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::setPositionX(float x) {
		if (!parent)return setLocalPositionX(x);
		getModel();
		globalModel.at(0, 3) = x;
		globalPos[0] = x;
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::setPositionY(float y) {
		if (!parent)return setLocalPositionY(y);
		getModel();
		globalModel.at(1, 3) = y;
		globalPos[1] = y;
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::setPositionZ(float z) {
		if (!parent)return setLocalPositionZ(z);
		getModel();
		globalModel.at(2, 3) = z;
		globalPos[2] = z;
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::addPositionX(float x) {
		if (!parent)return addLocalPositionX(x);
		getModel();
		globalPos[0] += x;
		globalModel.at(0, 3) = globalPos[0];
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::addPositionY(float y) {
		if (!parent)return addLocalPositionX(y);
		getModel();
		globalPos[1] += y;
		globalModel.at(1, 3) = globalPos[1];
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}

	void Transform::addPositionZ(float z) {
		if (!parent)return addLocalPositionZ(z);
		getModel();
		globalPos[2] += z;
		globalModel.at(2, 3) = globalPos[2];
		pos = parent->getInverseTransform() * vec4(globalPos, 1);
		model.at(0, 3) = pos[0];	model.at(1, 3) = pos[1];	model.at(2, 3) = pos[2];
		globalNotReady();
		globalReady = true;
	}
}