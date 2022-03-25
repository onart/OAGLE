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
#include "OA_PhysicalSystem.h"
#include "OA_Contact.h"
#include "OA_PointMass.h"

#include <algorithm>

namespace onart {
	std::vector<PointMass*> PointMassSystem::indiv;
	std::vector<PointMass2D*> PointMassSystem2D::indiv;
	std::vector<ForceGenerator*> PointMassSystem::forces;
	std::vector<ContactGenerator*> PointMassSystem::contacts;
	std::vector<ForceGenerator2D*> PointMassSystem2D::forces;
	std::vector<ContactGenerator2D*> PointMassSystem2D::contacts;

	template <class T>
	void insort(std::vector<T*>& v, T* e) {
		v.insert(std::lower_bound(v.begin(), v.end(), e), e);
	}

	template <class T>
	void removeFromSorted(std::vector<T*>& v, T* e) {
		auto it = std::lower_bound(v.begin(), v.end(), e);
		if (*it == e)v.erase(it);
	}

	void PointMassSystem::addIndividual(PointMass* p) { insort(indiv, p); }
	void PointMassSystem::removeIndividual(PointMass* p) { removeFromSorted(indiv, p); }
	void PointMassSystem::addContactGenerator(ContactGenerator* p) { insort(contacts, p); }
	void PointMassSystem::removeContactGenerator(ContactGenerator* p) { removeFromSorted(contacts, p); }
	void PointMassSystem::addForceGenerator(ForceGenerator* p) { insort(forces, p); }
	void PointMassSystem::removeForceGenerator(ForceGenerator* p) { removeFromSorted(forces, p); }

	void PointMassSystem2D::addIndividual(PointMass2D* p) { insort(indiv, p); }
	void PointMassSystem2D::removeIndividual(PointMass2D* p) { removeFromSorted(indiv, p); }
	void PointMassSystem2D::addContactGenerator(ContactGenerator2D* p) { insort(contacts, p); }
	void PointMassSystem2D::removeContactGenerator(ContactGenerator2D* p) { removeFromSorted(contacts, p); }
	void PointMassSystem2D::addForceGenerator(ForceGenerator2D* p) { insort(forces, p); }
	void PointMassSystem2D::removeForceGenerator(ForceGenerator2D* p) { removeFromSorted(forces, p); }

	
}