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
#ifndef __CONTACTDETECTOR_H__
#define __CONTACTDETECTOR_H__

#include "oaglem.h"

namespace onart {

	class RigidBody;

	struct PotentialContact {
		RigidBody* body[2];
	};

	struct BoundingSphere
	{
		vec3 center;
		float radius;
	public:
		inline BoundingSphere(const vec3& c, float r) :center(c), radius(r) {}
		BoundingSphere(const BoundingSphere& _1, const BoundingSphere& _2);
		bool overlaps(const BoundingSphere* other)const;
		float getGrowth(const BoundingSphere& other) const;
	};

	template <class BV>
	class BVHNode {
	public:
		~BVHNode();
		BVHNode* parent = nullptr;
		BVHNode* children[2];			// 트리 자식
		BV volume;						// 구 or 직육면체 등
		RigidBody* body = nullptr;		// 리프 노드일 때 강체
		inline bool isLeaf() const { return body != nullptr; }
		bool overlaps(const BVHNode* other) const;
		void recalculateBoundingVolume(bool recurse = true);
		unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const;
		unsigned getPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, unsigned limit) const;
		void insert(RigidBody* newBody, const BV& volume);
	};
}

#endif // !__CONTACTDETECTOR_H__

