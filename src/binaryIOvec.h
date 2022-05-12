#ifndef __BINARY_IO_VEC_H__
#define __BINARY_IO_VEC_H__

#include <vector>
#include <algorithm>

template <class T>
inline void insort(std::vector<T*>& v, T* e) {
	v.insert(std::lower_bound(v.begin(), v.end(), e), e);
}

template <class T>
inline void removeFromSorted(std::vector<T*>& v, T* e) {
	auto it = std::lower_bound(v.begin(), v.end(), e);
	if (*it == e)v.erase(it);
}

#endif // !__BINARY_IO_VEC_H__

