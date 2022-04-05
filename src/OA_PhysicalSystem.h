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
#ifndef _OA_PHYSICALSYSTEM_H__
#define _OA_PHYSICALSYSTEM_H__

#include <vector>

namespace onart {
	class PointMass;
	class PointMass2D;
	class ContactGenerator;
	class ContactGenerator2D;
	class Contact;
	class Contact2D;

	constexpr size_t maxContact = 60;

	/// <summary>
	/// 질점으로 구성된 세계입니다. 응용 단계에서 다루어지지 않습니다.
	/// </summary>
	class PointMassSystem
	{
	public:
		static void Update();
		static void addIndividual(PointMass* m);
		static void removeIndividual(PointMass* m);
		static void addContactGenerator(ContactGenerator* g);
		static void removeContactGenerator(ContactGenerator* g);
		static unsigned generateContacts();
	private:
		static std::vector<PointMass*> indiv;
		static std::vector<ContactGenerator*> contacts;
		static Contact* contactsInThisFrame;
	};

	/// <summary>
	/// 2D 질점으로 구성된 세계입니다. 응용 단계에서 다루어지지 않습니다.
	/// </summary>
	class PointMassSystem2D {
	public:
		static void Update();
		static void addIndividual(PointMass2D* m);
		static void removeIndividual(PointMass2D* m);
		static void addContactGenerator(ContactGenerator2D* g);
		static void removeContactGenerator(ContactGenerator2D* g);
		static unsigned generateContacts();
	private:
		static std::vector<PointMass2D*> indiv;
		static std::vector<ContactGenerator2D*> contacts;
		static Contact2D* contactsInThisFrame;
	};
}
#endif // !_OA_PHYSICALSYSTEM_H__
