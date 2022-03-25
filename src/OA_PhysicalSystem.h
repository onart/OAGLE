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
	class ForceGenerator;
	class ForceGenerator2D;
	class ContactGenerator;
	class ContactGenerator2D;
	/// <summary>
	/// �������� ������ �����Դϴ�. ���� �ܰ迡�� �ٷ������ �ʽ��ϴ�.
	/// </summary>
	class PointMassSystem
	{
	public:
		static void Update();
		static void addIndividual(PointMass* m);
		static void removeIndividual(PointMass* m);
		static void addForceGenerator(ForceGenerator* g);
		static void removeForceGenerator(ForceGenerator* g);
		static void addContactGenerator(ContactGenerator* g);
		static void removeContactGenerator(ContactGenerator* g);
	private:
		static std::vector<PointMass*> indiv;
		static std::vector<ForceGenerator*> forces;
		static std::vector<ContactGenerator*> contacts;
	};

	/// <summary>
	/// 2D �������� ������ �����Դϴ�. ���� �ܰ迡�� �ٷ������ �ʽ��ϴ�.
	/// </summary>
	class PointMassSystem2D {
	public:
		static void Update();
		static void addIndividual(PointMass2D* m);
		static void removeIndividual(PointMass2D* m);
		static void addForceGenerator(ForceGenerator2D* g);
		static void removeForceGenerator(ForceGenerator2D* g);
		static void addContactGenerator(ContactGenerator2D* g);
		static void removeContactGenerator(ContactGenerator2D* g);
	private:
		static std::vector<PointMass2D*> indiv;
		static std::vector<ForceGenerator2D*> forces;
		static std::vector<ContactGenerator2D*> contacts;
	};
}
#endif // !_OA_PHYSICALSYSTEM_H__
