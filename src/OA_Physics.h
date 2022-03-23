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
		/// ���� �������� ���ӵ��� �����մϴ�.
		/// </summary>
		/// <returns></returns>
		vec3 getAcceleration();
		/// <summary>
		/// �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocity(const vec3& v) { velocity = v; }
		/// <summary>
		/// x�� �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocityX(float x) { velocity.x = x; }
		/// <summary>
		/// y�� �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocityY(float y) { velocity.y = y; }
		/// <summary>
		/// z�� �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocityZ(float z) { velocity.z = z; }
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
		inline const vec3& getPosition() { return tr.getGlobalPosition(); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPosition(const vec3& p) { tr.setPosition(p); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� x�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPositionX(float x) { tr.setPositionX(x); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� y�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPositionY(float y) { tr.setPositionY(y); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� z�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPositionZ(float z) { tr.setPositionZ(z); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� ��ġ�� ������ ��ȭ��ŵ�ϴ�.
		/// </summary>
		inline void addPosition(const vec3& p) { tr.addPosition(p); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� x�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void addPositionX(float x) { tr.addPositionX(x); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� y�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void addPositionY(float y) { tr.addPositionY(y); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� z�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void addPositionZ(float z) { tr.addPositionZ(z); }
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
		/// ���� �������� ���ӵ��� �����մϴ�.
		/// </summary>
		vec2 getAcceleration();
		/// <summary>
		/// �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocity(const vec2& v) { velocity = v; }
		/// <summary>
		/// x�� �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocityX(float x) { velocity.x = x; }
		/// <summary>
		/// y�� �ӵ��� ������ �����մϴ�.
		/// </summary>
		inline void setVelocityY(float y) { velocity.y = y; }
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
		/// <summary>
		/// �ӵ��� ������ ä�� ���� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPosition(const vec2& p) { tr.setPosition(p); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� x�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPositionX(float x) { tr.setPositionX(x); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� y�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void setPositionY(float y) { tr.setPositionY(y); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� ��ġ�� ������ ��ȭ��ŵ�ϴ�.
		/// </summary>
		inline void addPosition(const vec3& p) { tr.addPosition(p); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� x�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void addPositionX(float x) { tr.addPositionX(x); }
		/// <summary>
		/// �ӵ��� ������ ä�� ���� y�� ��ġ�� ������ �����մϴ�.
		/// </summary>
		inline void addPositionY(float y) { tr.addPositionY(y); }
	protected:
		vec2 velocity;
		vec2 acceleration;
		vec2 netForce;
		float inverseMass;
		float damp;
	private:
		Transform& tr;
	};
}

#endif // !__OA_PHYSICS_H__

