/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_CQueue.h"
#include <cstdlib>
#include <cstring>

namespace onart {

	template<class T, unsigned L>
	CircularQueue<T, L>::CircularQueue() {
		memset(body, 0, sizeof(body));
	}

	template<class T, unsigned L>
	void CircularQueue<T, L>::push(const T& elem) {
		body[out++] = elem;
		if (out == L)out = 0;
		
	}

	template<class T, unsigned L>
	void CircularQueue<T, L>::push(const T* src, unsigned length) {
		unsigned l1 = L - out > length ? length : L - out;
		memcpy(body + out, src, l1);
		if (length > l1)memcpy(body, src + l1, out);
	}

	template<class T, unsigned L>
	T CircularQueue<T, L>::pop() {
		T ret = body[out];
		body[out++] = 0;
		if (out == L)out = 0;
		return ret;
	}

	template<class T, unsigned L>
	void CircularQueue<T, L>::copy(T* dst, unsigned length) {
		unsigned l1 = L - out > length ? length : L - out;
		memcpy(dst, body + out, l1);
		if (length > l1)memcpy(dst + l1, body, out);
	}


	template<class T>
	void CircularQueue<T, 0>::resize(unsigned length) {
		this->length = length;
		body = realloc(sizeof(T) * length);
		out = 0;
	}



	template<class T>
	CircularQueue<T, 0>::CircularQueue(unsigned length) :length(length) {
		body = malloc(sizeof(T) * length);
	}

	template<class T>
	CircularQueue<T, 0>::~CircularQueue() {
		free(body);
	}
}
