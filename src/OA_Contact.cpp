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
#include "OA_Contact.h"
#include "OA_Physics.h"

namespace onart {
	
	unsigned ContactResolver::iterations;

	void Contact::resolve(float duration) {
		resolveVelocity(duration);
		resolvePenetration(duration);
	}

	float Contact::afterSpeed() const {
		vec3 rel(matter[0]->getVelocity());
		if (matter[1]) rel -= matter[1]->getVelocity();
		return rel.dot(contactNormal);
	}

	void Contact::resolveVelocity(float duration) {
		float after = afterSpeed();
		if (after > 0) return;
		float after2 = -after * restitution;
		// Á¤Áö Á¢ÃË
		vec3 frameAcc = matter[0]->getAcceleration();
		if (matter[1])frameAcc -= matter[1]->getAcceleration();
		float afterByAcc = frameAcc.dot(contactNormal) * duration;
		if (afterByAcc < 0) {
			after += restitution * afterByAcc;
			if (after < 0)after = 0;
		}
		// Á¤Áö Á¢ÃË
		float delta = after2 - after;

		float totalInverseMass = matter[0]->getInverseMass();
		if (matter[1])totalInverseMass += matter[1]->getInverseMass();
		if (totalInverseMass <= 0) return;
		float impulse = delta / totalInverseMass;
		vec3 impulsePerIMass(contactNormal * impulse);
		matter[0]->impulse(impulsePerIMass);
		if (matter[1]) matter[1]->impulse(-impulsePerIMass);
	}

	void Contact::resolvePenetration(float duration) {
		if (penetration <= 0) return;
		float totalInvsereMass = matter[0]->getInverseMass();
		if (matter[1]) totalInvsereMass += matter[1]->getInverseMass();
		if (totalInvsereMass <= 0) return;
		vec3 movePerIMass = contactNormal * (penetration / totalInvsereMass);
		matter[0]->addPosition(movePerIMass * matter[0]->getInverseMass());
		if (matter[1])matter[1]->addPosition(movePerIMass * matter[1]->getInverseMass());
	}

	void Contact2D::resolve(float duration) {
		resolveVelocity(duration);
		resolvePenetration(duration);
	}

	float Contact2D::afterSpeed() const {
		vec2 rel(matter[0]->getVelocity());
		if (matter[1]) rel -= matter[1]->getVelocity();
		return rel.dot(contactNormal);
	}

	void Contact2D::resolveVelocity(float duration) {
		float after = afterSpeed();
		if (after > 0) return;
		float after2 = -after * restitution;
		float delta = after2 - after;
		// Á¤Áö Á¢ÃË
		vec3 frameAcc = matter[0]->getAcceleration();
		if (matter[1])frameAcc -= matter[1]->getAcceleration();
		float afterByAcc = frameAcc.dot(contactNormal) * duration;
		if (afterByAcc < 0) {
			after += restitution * afterByAcc;
			if (after < 0)after = 0;
		}
		// Á¤Áö Á¢ÃË
		float totalInverseMass = matter[0]->getInverseMass();
		if (matter[1])totalInverseMass += matter[1]->getInverseMass();
		if (totalInverseMass <= 0) return;
		float impulse = delta / totalInverseMass;
		vec2 impulsePerIMass(contactNormal * impulse);
		matter[0]->impulse(impulsePerIMass);
		if (matter[1]) matter[1]->impulse(-impulsePerIMass);
	}

	void Contact2D::resolvePenetration(float duration) {
		if (penetration <= 0) return;
		float totalInvsereMass = matter[0]->getInverseMass();
		if (matter[1]) totalInvsereMass += matter[1]->getInverseMass();
		if (totalInvsereMass <= 0) return;
		vec2 movePerIMass = contactNormal * (penetration / totalInvsereMass);
		matter[0]->addPosition(movePerIMass * matter[0]->getInverseMass());
		if (matter[1])matter[1]->addPosition(movePerIMass * matter[1]->getInverseMass());
	}

	void ContactResolver::resolve(Contact* arr, unsigned numContacts, float duration) {
		unsigned i;
		unsigned iterationsUsed = 0;
		while (iterationsUsed < iterations) {
			float min = INF;
			unsigned mi = numContacts;
			for (i = 0; i < numContacts; i++) {
				float sep = arr[i].afterSpeed();
				if (sep < min && (sep < 0 || arr[i].penetration>0)) {
					min = sep;
					mi = i;
				}
			}
			if (mi == numContacts) break;
			arr[mi].resolve(duration);
			iterationsUsed++;
		}
	}

	void ContactResolver::resolve(Contact2D* arr, unsigned numContacts, float duration) {
		unsigned i;
		unsigned iterationsUsed = 0;
		while (iterationsUsed < iterations) {
			float min = INF;
			unsigned mi = numContacts;
			for (i = 0; i < numContacts; i++) {
				float sep = arr[i].afterSpeed();
				if (sep < min && (sep < 0 || arr[i].penetration>0)) {
					min = sep;
					mi = i;
				}
			}
			if (mi == numContacts) break;
			arr[mi].resolve(duration);
			iterationsUsed++;
		}
	}

	unsigned MatterCable::addContact(Contact* contact, unsigned limit) {
		float length = currentLength();
		if (length < maxLen) return 0;
		contact->matter[0] = matter[0];
		contact->matter[1] = matter[1];
		vec3 norm((matter[1]->getPosition() - matter[0]->getPosition()).normalize());
		contact->contactNormal = norm;
		contact->penetration = length - maxLen;
		contact->restitution = restitution;
		return 1;
	}

	unsigned MatterRod::addContact(Contact* contact, unsigned limit) {
		float dist = currentLength();
		if (dist == length) return 0;
		contact->matter[0] = matter[0];
		contact->matter[1] = matter[1];
		vec3 norm((matter[1]->getPosition() - matter[0]->getPosition()).normalize());
		if (dist > length) {
			contact->contactNormal = norm;
			contact->penetration = dist - length;
		}
		else {
			contact->contactNormal = -norm;
			contact->penetration = length - dist;
		}
		contact->restitution = 0;
		return 1;
	}

	float MatterLink::currentLength() { 
		return matter[0]->getPosition().distance(matter[1]->getPosition()); 
	}
	
	float MatterLink2D::currentLength() {
		return vec2(matter[0]->getPosition()).distance(vec2(matter[1]->getPosition()));
	}

	unsigned MatterCable2D::addContact(Contact2D* contact, unsigned limit) {
		float length = currentLength();
		if (length < maxLen) return 0;
		contact->matter[0] = matter[0];
		contact->matter[1] = matter[1];
		vec2 norm((matter[1]->getPosition() - matter[0]->getPosition()).normalize());
		contact->contactNormal = norm;
		contact->penetration = length - maxLen;
		contact->restitution = restitution;
		return 1;
	}

	unsigned MatterRod2D::addContact(Contact2D* contact, unsigned limit) {
		float dist = currentLength();
		if (dist == length) return 0;
		contact->matter[0] = matter[0];
		contact->matter[1] = matter[1];
		vec2 norm((matter[1]->getPosition() - matter[0]->getPosition()).normalize());
		if (dist > length) {
			contact->contactNormal = norm;
			contact->penetration = dist - length;
		}
		else {
			contact->contactNormal = -norm;
			contact->penetration = length - dist;
		}
		contact->restitution = 0;
		return 1;
	}
}