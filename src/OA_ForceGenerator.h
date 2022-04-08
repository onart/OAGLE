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

#include <vector>
#include <map>
#include "oaglem.h"

namespace onart {
	class PointMass;
	class PointMass2D;
	class RigidBody;
	class RigidBody2D;
	/// <summary>
	/// 매 프레임 힘을 지속적으로 가합니다. 중력 같은 거의 상수인 힘이 아닌 항력 등에 대하여 사용합니다.
	/// </summary>
	class ForceGenerator {
	public:
		virtual void generate(PointMass* pm) {}
		virtual void generate(RigidBody* rb) {}
		inline virtual void generate(PointMass* pm, float dt) {}
	};

	/// <summary>
	/// 매 프레임 힘을 지속적으로 가합니다. 중력 같은 거의 상수인 힘이 아닌 항력 등에 대하여 사용합니다.
	/// </summary>
	class ForceGenerator2D {
	public:
		virtual void generate(PointMass2D* pm) {}
		virtual void generator(RigidBody2D* rb) {}
		inline virtual void generate(PointMass2D* pm, float dt) {}
	};

	/// <summary>
	/// 이것은 힘 발생기의 한 예입니다. 계수 2개를 미리 정해 두고 항력을 적용합니다.
	/// </summary>
	class DragGenerator :public ForceGenerator {
	public:
		inline DragGenerator(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass* pm);
		void generate(RigidBody* rb);
	private:
		const float k1, k2;
	};

	/// <summary>
	/// 계수 2개를 미리 정해 두고 항력을 적용합니다.
	/// </summary>
	class DragGenerator2D :public ForceGenerator2D {
	public:
		inline DragGenerator2D(float k1, float k2) :k1(k1), k2(k2) {}
		void generate(PointMass2D* pm);
	private:
		const float k1, k2;
	};

	/// <summary>
	/// 훅 법칙을 따르는 힘을 작용합니다. 2개 물체에 각각 연결해야 합니다. 이에 따라, 안전한 사용을 위해서는 wrapper가 필수불가결합니다.
	/// 한쪽을 고정하려면 무한대 질량을 사용합니다.
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
	/// 훅 법칙을 따르는 힘을 작용합니다. 2개 물체에 각각 연결해야 합니다. 이에 따라, 안전한 사용을 위해서는 wrapper가 필수불가결합니다.
	/// 한쪽을 고정하려면 무한대 질량을 사용합니다.
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
	/// 훅 법칙에서 당기는 힘만 작용합니다. 2개 물체에 각각 연결해야 합니다. 이에 따라, 안전한 사용을 위해서는 wrapper가 필수불가결합니다.
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

	class HookeSpringRigid :public ForceGenerator {
	private:
		vec3 attached;
		vec3 otherAttached;
		RigidBody* other;
		float k;
		float restLength;
	public:
		inline HookeSpringRigid(const vec3& attached, RigidBody* other, const vec3& otherAttached, float k, float restLength) :attached(attached), other(other), k(k), restLength(restLength), otherAttached(otherAttached) {}
		void generate(RigidBody* rb);
	};

	/// <summary>
	/// 3차원 힘 발생기와 강체의 릴레이션입니다.
	/// </summary>
	class R_ForceRigid {
	protected:
		static std::map<RigidBody*, std::vector<ForceGenerator*>> rbreg;
		static std::map<ForceGenerator*, std::vector<RigidBody*>> fgreg;
	public:
		/// <summary>
		/// 현재 프레임에 가할 힘이 있으면 가합니다.
		/// </summary>
		static void Update();
		/// <summary>
		/// 힘 발생기와 질점의 연계를 추가합니다.
		/// </summary>
		static void add(RigidBody* pm, ForceGenerator* fg);
		/// <summary>
		/// 힘 발생기와 질점의 연계를 제거합니다.
		/// </summary>
		static void remove(RigidBody* pm, ForceGenerator* fg);
		/// <summary>
		/// 주어진 질점에 대한 모든 힘 발생기를 제거합니다.
		/// </summary>
		static void cascade(RigidBody* pm);
		/// <summary>
		/// 주어진 힘 발생기에 대한 모든 질점을 제거합니다.
		/// </summary>
		static void cascade(ForceGenerator* fg);
		/// <summary>
		/// 모든 힘 발생기와 질점의 관계를 제거합니다.
		/// </summary>
		static void clear();
	};

	/// <summary>
	/// 3차원 힘 발생기와 질점의 릴레이션입니다.
	/// </summary>
	class R_ForcePoint {
	protected:
		static std::map<PointMass*, std::vector<ForceGenerator*>> pmreg;
		static std::map<ForceGenerator*, std::vector<PointMass*>> fgreg;
	public:
		/// <summary>
		/// 현재 프레임에 가할 힘이 있으면 가합니다.
		/// </summary>
		static void Update();
		/// <summary>
		/// 힘 발생기와 질점의 연계를 추가합니다.
		/// </summary>
		static void add(PointMass* pm, ForceGenerator* fg);
		/// <summary>
		/// 힘 발생기와 질점의 연계를 제거합니다.
		/// </summary>
		static void remove(PointMass* pm, ForceGenerator* fg);
		/// <summary>
		/// 주어진 질점에 대한 모든 힘 발생기를 제거합니다.
		/// </summary>
		static void cascade(PointMass* pm);
		/// <summary>
		/// 주어진 힘 발생기에 대한 모든 질점을 제거합니다.
		/// </summary>
		static void cascade(ForceGenerator* fg);
		/// <summary>
		/// 모든 힘 발생기와 질점의 관계를 제거합니다.
		/// </summary>
		static void clear();
	};

	/// <summary>
	/// 2차원 힘 발생기와 질점의 릴레이션입니다.
	/// </summary>
	class R_ForcePoint2D {
	protected:
		static std::map<PointMass2D*, std::vector<ForceGenerator2D*>> pmreg;
		static std::map<ForceGenerator2D*, std::vector<PointMass2D*>> fgreg;
	public:
		/// <summary>
		/// 현재 프레임에 가할 힘이 있으면 가합니다.
		/// </summary>
		static void Update();
		/// <summary>
		/// 힘 발생기와 질점의 연계를 추가합니다.
		/// </summary>
		static void add(PointMass2D* pm, ForceGenerator2D* fg);
		/// <summary>
		/// 주어진 질점에 대한 모든 힘 발생기를 제거합니다.
		/// </summary>
		static void remove(PointMass2D* pm, ForceGenerator2D* fg);
		/// <summary>
		/// 주어진 힘 발생기에 대한 모든 질점을 제거합니다.
		/// </summary>
		static void cascade(PointMass2D* pm);
		/// <summary>
		/// 주어진 힘 발생기에 대한 모든 질점을 제거합니다.
		/// </summary>
		static void cascade(ForceGenerator2D* fg);
		/// <summary>
		/// 모든 힘 발생기와 질점의 관계를 제거합니다.
		/// </summary>
		static void clear();
	};
}

#endif // !__OA_FORCEGENERATOR_H__
