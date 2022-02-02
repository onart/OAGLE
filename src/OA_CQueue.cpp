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
