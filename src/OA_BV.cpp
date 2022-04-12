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
#include "OA_BV.h"

namespace onart {
	template<class BV> bool BVHNode<BV>::overlaps(const BVHNode* other)const {
		return volume->overlaps(other->volume);
	}

	template<class BV> unsigned BVHNode<BV>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const {
		if (isLeaf() || limit == 0) return 0;
		return children[0]->getPotentialContactsWith(children[1], contacts, limit);
	}

	template<class BV> unsigned BVHNode<BV>::getPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, unsigned limit) const {
		if (overlaps(other) || limit == 0)return 0;
		if (isLeaf() && other->isLeaf()) {
			contacts->body[0] = body;
			contacts->body[1] = other->body;
			return 1;
		}
		if (other->isLeaf() || (!isLeaf() && volume->getSize() >= other->volume->getSize())) {
			unsigned count = children[0]->getPotentialContactsWith(other, contacts, limit);
			if (limit > count) {
				return count - children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
			}
			else {
				return count;
			}
		}
		else {
			unsigned count = getPotentialContactsWith(other->children[0], contacts, limit);
			if (limit > count) {
				return count + getPotentialContactsWith(other->children[1], contacts + count, limit - count);
			}
			else {
				return count;
			}
		}
	}

	template<class BV> void BVHNode<BV>::insert(RigidBody* newBody, const BV& newVolume) {
		if (isLeaf()) {
			children[0] = new BVHNode(this, volume, body);
			children[1] = new BVHNode(this, newVolume, newBody);
			this->body = nullptr;
			recalculateBoundingVolume();
		}
		else {
			if (children[0]->volume.getGrowth(newVolume) < children[1]->volume.getGrowth(newVolume)) {
				children[0]->insert(newBody, newVolume);
			}
			else {
				children[1]->insert(newBody, newVolume);
			}
		}
	}

	template<class BV> BVHNode<BV>::~BVHNode() {
		if (parent) {
			BVHNode* sibling;
			if (parent->children[0] == this)sibling = parent->children[1];
			else sibling = parent->children[0];
			parent->volume = sibling->volume;
			parent->body = sibling->body;
			parent->children[0] = sibling->children[0];
			parent->children[1] = sibling->children[1];
			sibling->parent = nullptr;
			sibling->body = nullptr;
			sibling->children[0] = nullptr;
			sibling->children[1] = nullptr;
			delete sibling;
			parent->recalculateBoundingVolume();
		}
		if (children[0]) {
			children[0]->parent = nullptr;
			delete children[0];
		}
		if (children[1]) {
			children[1]->parent = nullptr;
			delete children[1];
		}
	}

	template<class BV> void BVHNode<BV>::recalculateBoundingVolume(bool recurse) {
		if (isLeaf()) return;
		volume = std::move(BV(children[0]->volume, children[1]->volume));
		if (parent) parent->recalculateBoundingVolume(true);
	}

	BoundingSphere::BoundingSphere(const BoundingSphere& _1, const BoundingSphere& _2) {
		vec3 centerOffset = _2.center - _1.center;
		float distance = centerOffset.length2();
		float radDiff = _2.radius - _1.radius;
		if (radDiff * radDiff >= distance) {	// Æ÷ÇÔ
			if (_1.radius > _2.radius) {
				center = _1.center;
				radius = _1.radius;
			}
			else {
				center = _2.center;
				radius = _2.radius;
			}
		}
		else {
			distance = sqrtf(distance);
			radius = (distance + _1.radius + _2.radius) * 0.5f;
			center = _1.center;
			center += centerOffset * ((radius - _1.radius) / distance);
		}
	}

	bool BoundingSphere::overlaps(const BoundingSphere* other) const {
		float rpr = (radius + other->radius);
		return center.distance2(other->center) < rpr * rpr;
	}

	float BoundingSphere::getGrowth(const BoundingSphere& other)const {
		BoundingSphere newSphere(*this, other);
		return newSphere.radius * newSphere.radius - radius * radius;
	}
}