/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
* Cyclone
* Copyright (c) 2003-2009 Ian Millington
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef _OA_CONTACT_H__
#define _OA_CONTACT_H__

#include "oaglem.h"

namespace onart {
	class PointMass;
	class PointMass2D;

	class Contact
	{
		friend class ContactResolver;
	public:
		PointMass* matter[2];
		vec3 contactNormal;
		float restitution;
		float penetration;
	protected:
		void resolve(float duration);
		float afterSpeed() const;
	private:
		void resolveVelocity(float duration);
		void resolvePenetration(float duration);
	};

	class Contact2D {
	public:
		friend class ContactResolver;
		PointMass2D* matter[2];
		vec2 contactNormal;
		float restitution;
		float penetration;
	protected:
		void resolve(float duration);
		float afterSpeed() const;
	private:
		void resolveVelocity(float duration);
		void resolvePenetration(float duration);
	};

	/// <summary>
	/// 두 물체의 여러 접촉이 동시 발생한 경우 적절히 처리합니다.
	/// 시작하기 전에 setMaximumRepCount()를 한 번은 호출해야 합니다.
	/// </summary>
	class ContactResolver {
	protected:
		static unsigned iterations;
	public:
		inline static void setMaximumRepCount(unsigned u) { iterations = u; }
		static void resolve(Contact* arr, unsigned numContacts, float duration);
		static void resolve(Contact2D* arr, unsigned numContacts, float duration);
	};

	class ContactGenerator {
	public:
		virtual unsigned addContact(Contact* contact, unsigned limit) = 0;
	};

	class ContactGenerator2D {
	public:
		virtual unsigned addContact(Contact2D* contact, unsigned limit) = 0;
	};

	/// <summary>
	/// 2개 물체를 묶어 두는 연결체입니다.
	/// </summary>
	class MatterLink :public ContactGenerator {
	public:
		PointMass* matter[2];
		/// <summary>
		/// 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴하며, 아닌 경우 0을 리턴합니다. 주어진 contact에 값을 채웁니다.
		/// </summary>
		virtual unsigned addContact(Contact* contact, unsigned limit) = 0;
	protected:
		float currentLength();
	};

	/// <summary>
	/// 2개 물체를 묶어 두는 연결체입니다.
	/// </summary>
	class MatterLink2D : public ContactGenerator2D {
	public:
		PointMass2D* matter[2];
		/// <summary>
		/// 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴하며, 아닌 경우 0을 리턴합니다. 주어진 contact에 값을 채웁니다.
		/// </summary>
		virtual unsigned addContact(Contact2D* contact, unsigned limit) = 0;
	protected:
		float currentLength();
	};

	/// <summary>
	/// 2개 물체를 강성 끈으로 연결합니다.
	/// </summary>
	class MatterCable : public MatterLink {
	public:
		const float maxLen;
		const float restitution;
		/// <summary>
		/// 끈 유지에 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴합니다.
		/// </summary>
		unsigned addContact(Contact* contact, unsigned limit);
	};

	/// <summary>
	/// 2개 물체를 강성 막대로 연결합니다.
	/// </summary>
	class MatterRod :public MatterLink {
	public:
		const float length;
		/// <summary>
		/// 끈 유지에 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴합니다.
		/// </summary>
		unsigned addContact(Contact* contact, unsigned limit);
	};

	/// <summary>
	/// 2개 물체를 강성 끈으로 연결합니다.
	/// </summary>
	class MatterCable2D : public MatterLink2D {
	public:
		const float maxLen;
		const float restitution;
		/// <summary>
		/// 끈 유지에 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴합니다.
		/// </summary>
		unsigned addContact(Contact2D* contact, unsigned limit);
	};

	/// <summary>
	/// 2개 물체를 강성 막대로 연결합니다.
	/// </summary>
	class MatterRod2D :public MatterLink2D {
	public:
		const float length;
		/// <summary>
		/// 끈 유지에 필요한 경우 접촉을 생성합니다. 접촉이 생성된 경우 1을 리턴합니다.
		/// </summary>
		unsigned addContact(Contact2D* contact, unsigned limit);
	};
}

#endif // !_OA_CONTACT_H__

