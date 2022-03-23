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
	/// �� ��ü�� ���� ������ ���� �߻��� ��� ������ ó���մϴ�.
	/// �����ϱ� ���� setMaximumRepCount()�� �� ���� ȣ���ؾ� �մϴ�.
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
	/// 2�� ��ü�� ���� �δ� ����ü�Դϴ�.
	/// </summary>
	class MatterLink :public ContactGenerator {
	public:
		PointMass* matter[2];
		/// <summary>
		/// �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����ϸ�, �ƴ� ��� 0�� �����մϴ�. �־��� contact�� ���� ä��ϴ�.
		/// </summary>
		virtual unsigned addContact(Contact* contact, unsigned limit) = 0;
	protected:
		float currentLength();
	};

	/// <summary>
	/// 2�� ��ü�� ���� �δ� ����ü�Դϴ�.
	/// </summary>
	class MatterLink2D : public ContactGenerator2D {
	public:
		PointMass2D* matter[2];
		/// <summary>
		/// �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����ϸ�, �ƴ� ��� 0�� �����մϴ�. �־��� contact�� ���� ä��ϴ�.
		/// </summary>
		virtual unsigned addContact(Contact2D* contact, unsigned limit) = 0;
	protected:
		float currentLength();
	};

	/// <summary>
	/// 2�� ��ü�� ���� ������ �����մϴ�.
	/// </summary>
	class MatterCable : public MatterLink {
	public:
		const float maxLen;
		const float restitution;
		/// <summary>
		/// �� ������ �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����մϴ�.
		/// </summary>
		unsigned addContact(Contact* contact, unsigned limit);
	};

	/// <summary>
	/// 2�� ��ü�� ���� ����� �����մϴ�.
	/// </summary>
	class MatterRod :public MatterLink {
	public:
		const float length;
		/// <summary>
		/// �� ������ �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����մϴ�.
		/// </summary>
		unsigned addContact(Contact* contact, unsigned limit);
	};

	/// <summary>
	/// 2�� ��ü�� ���� ������ �����մϴ�.
	/// </summary>
	class MatterCable2D : public MatterLink2D {
	public:
		const float maxLen;
		const float restitution;
		/// <summary>
		/// �� ������ �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����մϴ�.
		/// </summary>
		unsigned addContact(Contact2D* contact, unsigned limit);
	};

	/// <summary>
	/// 2�� ��ü�� ���� ����� �����մϴ�.
	/// </summary>
	class MatterRod2D :public MatterLink2D {
	public:
		const float length;
		/// <summary>
		/// �� ������ �ʿ��� ��� ������ �����մϴ�. ������ ������ ��� 1�� �����մϴ�.
		/// </summary>
		unsigned addContact(Contact2D* contact, unsigned limit);
	};
}

#endif // !_OA_CONTACT_H__

