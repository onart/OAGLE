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
#ifndef __OA_FORCEGENERATOR_H__
#define __OA_FORCEGENERATOR_H__

namespace onart {
	class PointMass;
	class PointMass2D;
	/// <summary>
	/// �� ������ ���� ���������� ���մϴ�. �߷� ���� ���� ����� ���� �ƴ� �׷� � ���Ͽ� ����մϴ�.
	/// </summary>
	class ForceGenerator {
	public:
		virtual void generate(PointMass* pm) = 0;
		inline virtual void generate(PointMass* pm, float dt) {}
	};

	/// <summary>
	/// �� ������ ���� ���������� ���մϴ�. �߷� ���� ���� ����� ���� �ƴ� �׷� � ���Ͽ� ����մϴ�.
	/// </summary>
	class ForceGenerator2D {
	public:
		virtual void generate(PointMass2D* pm) = 0;
		inline virtual void generate(PointMass2D* pm, float dt) {}
	};

	/// <summary>
	/// �̰��� �� �߻����� �� ���Դϴ�. ��� 2���� �̸� ���� �ΰ� �׷��� �����մϴ�.
	/// </summary>
	class DragGenerator :public ForceGenerator {
	public:
		inline DragGenerator(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass* pm);
	private:
		const float k1, k2;
	};

	/// <summary>
	/// ��� 2���� �̸� ���� �ΰ� �׷��� �����մϴ�.
	/// </summary>
	class DragGenerator2D :public ForceGenerator2D {
	public:
		inline DragGenerator2D(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass2D* pm);
	private:
		const float k1, k2;
	};

	/// <summary>
	/// �� ��Ģ�� ������ ���� �ۿ��մϴ�. 2�� ��ü�� ���� �����ؾ� �մϴ�. �̿� ����, ������ ����� ���ؼ��� wrapper�� �ʼ��Ұ����մϴ�.
	/// ������ �����Ϸ��� ���Ѵ� ������ ����մϴ�.
	/// </summary>
	class HookeSpring : public ForceGenerator {
	public:
		inline HookeSpring(PointMass* other, float k, float restLength) :other(other), k(k), restLength(restLength) {}
		void generate(PointMass* pm);
	private:
		PointMass* other;
		const float k;
		const float restLength;
	};

	/// <summary>
	/// �� ��Ģ�� ������ ���� �ۿ��մϴ�. 2�� ��ü�� ���� �����ؾ� �մϴ�. �̿� ����, ������ ����� ���ؼ��� wrapper�� �ʼ��Ұ����մϴ�.
	/// ������ �����Ϸ��� ���Ѵ� ������ ����մϴ�.
	/// </summary>
	class HookeSpring2D :public ForceGenerator2D {
	public:
		inline HookeSpring2D(PointMass2D* other, float k, float restLength) :other(other), k(k), restLength(restLength) {}
		void generate(PointMass2D* pm);
	private:
		PointMass2D* other;
		const float k;
		const float restLength;
	};

	/// <summary>
	/// �� ��Ģ���� ���� ���� �ۿ��մϴ�. 2�� ��ü�� ���� �����ؾ� �մϴ�. �̿� ����, ������ ����� ���ؼ��� wrapper�� �ʼ��Ұ����մϴ�.
	/// </summary>
	class HookeBungee: public ForceGenerator {
	public:
		inline HookeBungee(PointMass* other, float k, float restLength) :other(other), k(k), restLength(restLength) {}
		void generate(PointMass* pm);
	private:
		PointMass* other;
		const float k;
		const float restLength;
	};

	class HookeBungee2D : public ForceGenerator2D {
	public:
		inline HookeBungee2D(PointMass2D* other, float k, float restLength) :other(other), k(k), restLength(restLength) {}
		void generate(PointMass2D* pm);
	private:
		PointMass2D* other;
		const float k;
		const float restLength;
	};
}

#endif // !__OA_FORCEGENERATOR_H__
