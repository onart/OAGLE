/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_CQUEUE_H__
#define __OA_CQUEUE_H__

namespace onart {
	/// <summary>
	/// Ư���� ������ ���� ����� �� �ִ� ���� ť�Դϴ�. �⺻������ ��� ��Ʈ�� 0���� �� �ֽ��ϴ�.
	/// </summary>
	template <class T, unsigned L>
	class CircularQueue
	{
	private:
		int out = 0;
		T body[L];
	public:
		CircularQueue();
		/// <summary>
		/// ť�� ���Ҹ� �����Ͽ� �����մϴ�.
		/// </summary>
		void push(const T&);
		/// <summary>
		/// ť�� ���ϴ� ����ŭ�� ���Ҹ� �����Ͽ� �����մϴ�.
		/// </summary>
		/// <param name="src">���� ���</param>
		/// <param name="length">������ �����Դϴ�. ť ���̺��� ū ��� ť ���̸�ŭ�� �����մϴ�.</param>
		void push(const T* src, unsigned length = L);
		/// <summary>
		/// ť���� ���� �� ���Ҹ� ���� �����մϴ�.
		/// </summary>
		T pop();
		/// <summary>
		/// ť�� ������ ���Լ�������� �����մϴ�. ������ ���̸� ������� �ʴ� ��� ��ü�� �����մϴ�.
		/// </summary>
		/// <param name="dst">�����ؼ� �ٿ����� ��</param>
		/// <param name="length">������ �����Դϴ�. ť ���̺��� ū ��� ť ��ü�� �����մϴ�.</param>
		void copy(T* dst, unsigned length = L);
	};

	/// <summary>
	/// Ư���� ������ ���� ����� �� �ִ� ���� ť�Դϴ�. �⺻������ ��� ��Ʈ�� 0���� �� ������ ���̸� �ٲپ� ��Ȱ���� �� �ֽ��ϴ�.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class CircularQueue<T, 0> {
	private:
		unsigned length;
		int out = 0;
		T* body;
	public:
		/// <summary>
		/// ť�� ���̸� �ٲߴϴ�. ������ ���Ҵ� �������� �ʽ��ϴ�.
		/// </summary>
		void resize(unsigned length);
		/// <summary>
		/// ���ϴ� ������ ť�� �������� �Ҵ��Ͽ� �����մϴ�.
		/// </summary>
		CircularQueue(unsigned length);
		~CircularQueue();
	};
	
}

#endif // !__OA_CQUEUE_H__


