﻿/********************************************************************************
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
	/// 특수한 목적을 위해 사용할 수 있는 원형 큐입니다. 기본적으로 모든 비트가 0으로 차 있습니다.
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
		/// 큐에 원소를 복사하여 삽입합니다.
		/// </summary>
		void push(const T&);
		/// <summary>
		/// 큐에 원하는 수만큼의 원소를 복사하여 삽입합니다.
		/// </summary>
		/// <param name="src">복사 대상</param>
		/// <param name="length">복사할 길이입니다. 큐 길이보다 큰 경우 큐 길이만큼만 복사합니다.</param>
		void push(const T* src, unsigned length = L);
		/// <summary>
		/// 큐에서 가장 앞 원소를 빼서 리턴합니다.
		/// </summary>
		T pop();
		/// <summary>
		/// 큐의 내용을 선입선출식으로 복사합니다. 복사할 길이를 명시하지 않는 경우 전체를 복사합니다.
		/// </summary>
		/// <param name="dst">복사해서 붙여넣을 곳</param>
		/// <param name="length">복사할 길이입니다. 큐 길이보다 큰 경우 큐 전체를 복사합니다.</param>
		void copy(T* dst, unsigned length = L);
	};

	/// <summary>
	/// 특수한 목적을 위해 사용할 수 있는 원형 큐입니다. 기본적으로 모든 비트가 0으로 차 있으며 길이를 바꾸어 재활용할 수 있습니다.
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
		/// 큐의 길이를 바꿉니다. 기존의 원소는 유지하지 않습니다.
		/// </summary>
		void resize(unsigned length);
		/// <summary>
		/// 원하는 길이의 큐를 동적으로 할당하여 생성합니다.
		/// </summary>
		CircularQueue(unsigned length);
		~CircularQueue();
	};
	
}

#endif // !__OA_CQUEUE_H__


