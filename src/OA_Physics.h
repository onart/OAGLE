/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_PHYSICS_H__
#define __OA_PHYSICS_H__

#include "oaglem.h"
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// �Ϻ� ���� ��Ģ�� ����޴� �����Դϴ�. Ʈ�������� � ��Ģ�� �����ϴ� ���°� �Ǵµ�, ���ɻ� ��Ʈ Ʈ���������� �����ϴ� ���� ���ϰ� �����մϴ�.
	/// </summary>
	class PointMass {
	public:
		/// <summary>
		/// ������ �����մϴ�.
		/// </summary>
		/// <param name="tr">���������� �����̰� �� Ʈ������ ��ü�Դϴ�. null�̸� �ǹ̰� �����Ƿ� �����ڸ� ����մϴ�.</param>
		PointMass(Transform& tr);
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�.
		/// </summary>
		void Update();
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�. �̰��� ��ü�� ���� �ð� �帧�� ���� �غ�� �Լ��Դϴ�. �Ű������� ���� Update()�� �̰� �� �ϳ����� �����մϴ�.
		/// </summary>
		void Update(float dt);
		/// <summary>
		/// ��ü�� ���� ���ӽ�ŵ�ϴ�. �ӵ� ��ȭ���� ������ �ݺ���մϴ�.
		/// ���� �ð� � ����ϴ� ������ �� ���ϱ�� addForce �Լ��� �̿��ϼ���.
		/// </summary>
		void impulse(const vec3&);
		/// <summary>
		/// ��ü�� ���� ���ӽ�ŵ�ϴ�. �ӵ� ��ȭ���� ������ ���� ���� �־��� ��ŭ�Դϴ�.
		/// ���� �ð� � ����ϴ� ������ �� ���ϱ�� addForce �Լ��� �̿��ϼ���.
		/// </summary>
		void accelerate(const vec3&);
		/// <summary>
		/// ���� �����ӿ� ���� ���մϴ�.
		/// </summary>
		void addForce(const vec3&);
		/// <summary>
		/// ���������� ���� ���մϴ�. ���ӵ��� ������ �ݺ���մϴ�.
		/// �ְ� �ִ� ���� ���ַ��� ������ �Լ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�. �׷°� ���� ����� �ƴ� ���� ���ؼ��� �� �߻��⸦ ����մϴ�.
		/// </summary>
		void addConstantForce(const vec3&);
		/// <summary>
		/// ���������� ���� ���մϴ�. ���ӵ��� ������ ���� ���� �־��� ��ŭ�Դϴ�. ���� ��� �߷��� �̷��� �����մϴ�.
		/// �ְ� �ִ� ���� ���ַ��� ������ �Լ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�.
		/// </summary>
		void addAbsoluteForce(const vec3&);
		/// <summary>
		/// ���� �ӵ��� �����մϴ�.
		/// </summary>
		inline const vec3& getVelocity() const { return velocity; }
		/// <summary>
		/// �������� �����մϴ�.
		/// </summary>
		inline float getInverseMass() const { return inverseMass; }
		/// <summary>
		/// ���� ��ġ�� �����մϴ�.
		/// </summary>
		inline const vec3& getPosition() const { return tr.getGlobalPosition(); }
	protected:
		vec3 velocity;
		vec3 acceleration;
		vec3 netForce;
		float inverseMass;
		float damp;
	private:
		Transform& tr;
	};

	/// <summary>
	/// �Ϻ� ���� ��Ģ�� ����޴� �����Դϴ�. Ʈ�������� � ��Ģ�� �����ϴ� ���°� �Ǵµ�, ���ɻ� ��Ʈ Ʈ���������� �����ϴ� ���� ���ϰ� �����մϴ�.
	/// </summary>
	class PointMass2D {
	public:
		/// <summary>
		/// ������ �����մϴ�.
		/// </summary>
		/// <param name="tr">���������� �����̰� �� Ʈ������ ��ü�Դϴ�. null�̸� �ǹ̰� �����Ƿ� �����ڸ� ����մϴ�.</param>
		PointMass2D(Transform& tr);
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�.
		/// </summary>
		void Update();
		/// <summary>
		/// �� ������ ���� ��Ģ�� �����մϴ�. �̰��� ��ü�� ���� �ð� �帧�� ���� �غ�� �Լ��Դϴ�. �Ű������� ���� Update()�� �̰� �� �ϳ����� �����մϴ�.
		/// </summary>
		void Update(float dt);
		/// <summary>
		/// ��ü�� ���� ���ӽ�ŵ�ϴ�. �ӵ� ��ȭ���� ������ �ݺ���մϴ�.
		/// ���� �ð� � ����ϴ� ������ �� ���ϱ�� addForce �Լ��� �̿��ϼ���.
		/// </summary>
		void impulse(const vec2&);
		/// <summary>
		/// ��ü�� ���� ���ӽ�ŵ�ϴ�. �ӵ� ��ȭ���� ������ ���� ���� �־��� ��ŭ�Դϴ�.
		/// ���� �ð� � ����ϴ� ������ �� ���ϱ�� addForce �Լ��� �̿��ϼ���.
		/// </summary>
		void accelerate(const vec2&);
		/// <summary>
		/// ���� �����ӿ� ���� ���մϴ�.
		/// </summary>
		void addForce(const vec2&);
		/// <summary>
		/// ���������� ���� ���մϴ�. ���ӵ��� ������ �ݺ���մϴ�.
		/// �ְ� �ִ� ���� ���ַ��� ������ �Լ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�. �׷°� ���� ����� �ƴ� ���� ���ؼ��� �� �߻��⸦ ����մϴ�.
		/// </summary>
		void addConstantForce(const vec2&);
		/// <summary>
		/// ���������� ���� ���մϴ�. ���ӵ��� ������ ���� ���� �־��� ��ŭ�Դϴ�. ���� ��� �߷��� �̷��� �����մϴ�.
		/// �ְ� �ִ� ���� ���ַ��� ������ �Լ��� �ݴ� ������ ���͸� �ָ� �˴ϴ�.
		/// </summary>
		void addAbsoluteForce(const vec2&);
		/// <summary>
		/// ���� �ӵ��� �����մϴ�.
		/// </summary>
		inline const vec2& getVelocity() const { return velocity; }
		/// <summary>
		/// �������� �����մϴ�.
		/// </summary>
		inline float getInverseMass() const { return inverseMass; }
		/// <summary>
		/// ���� ��ġ�� �����մϴ�. 2D���� vec3 Ÿ���Դϴ�.
		/// </summary>
		inline const vec3& getPosition() const { return tr.getGlobalPosition(); }
	protected:
		vec2 velocity;
		vec2 acceleration;
		vec2 netForce;
		float inverseMass;
		float damp;
	private:
		Transform& tr;
	};

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
		float k1, k2;
	};

	/// <summary>
	/// ��� 2���� �̸� ���� �ΰ� �׷��� �����մϴ�.
	/// </summary>
	class DragGenerator2D :public ForceGenerator2D {
	public:
		inline DragGenerator2D(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass2D* pm);
	private:
		float k1, k2;
	};
}

#endif // !__OA_PHYSICS_H__

