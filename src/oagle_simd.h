/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OAGLE_SIMD_H__
#define __OAGLE_SIMD_H__

// 벡터와 행렬 연산 아래에서 작동하기 위한 SIMD 라이브러리입니다. 컴파일 시 사용하지 않으려면 OAGLE_NOSIMD를 정의해 주세요.
// 이 함수들은 경계 검사를 하지 않습니다. 인덱스 [3]까지 사용하므로, Segmentation fault 발생에 유의해 주세요.

#include <cstring>

namespace onart {

	/// <summary>
	/// 특정 타입의 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값</param>
	template <class T>
	inline void set4(T* vec, T val) {
		vec[0] = val; vec[1] = val; vec[2] = val; vec[3] = val;
	}

	/// <summary>
	/// 특정 타입의 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값들</param>
	template <class T>
	inline void set4(T* vec, const T* val) {
		vec[0] = val[0]; vec[1] = val[1]; vec[2] = val[2]; vec[3] = val[3];
	}

	/// <summary>
	/// 특정 타입의 배열에서 원하는 만큼 원하는 값으로 초기화합니다.
	/// </summary>
	template<class T>
	inline void setAll(T* vec, T val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			set4<T>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] = val;
		}
	}

	/// <summary>
	/// 특정 타입의 배열에서 원하는 만큼 원하는 값으로 초기화합니다. memcpy()를 그대로 사용합니다. 매개변수 size는 바이트 단위가 아닌 원소의 수임에 주의하세요.
	/// </summary>
	template<class T>
	inline void setAll(T* vec, const T* val, size_t size) {
		memcpy(vec, val, size * sizeof(T));
	}

	/// <summary>
	/// 특정 타입의 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값</param>
	template<class T>
	inline void add4(T* vec, T val) {
		vec[0] += val; vec[1] += val; vec[2] += val; vec[3] += val;
	}

	/// <summary>
	/// 특정 타입의 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	template <class T>
	inline void add4(T* vec, const T* val) {
		vec[0] += val[0]; vec[1] += val[1]; vec[2] += val[2]; vec[3] += val[3];
	}

	/// <summary>
	/// 특정 타입의 배열에 원하는 만큼 원하는 값을 더합니다.
	/// </summary>
	template<class T>
	inline void addAll(T* vec, T val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			add4<T>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] += val;
		}
	}

	/// <summary>
	/// 특정 타입의 배열에 다른 배열을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	/// <param name="size">배열 크기</param>
	template<class T>
	inline void addAll(T* vec, const T* val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			add4<T>(vec + (i - 4), val + (i - 4));
		}
		for (i -= 4; i < size; i++) {
			vec[i] += val[i];
		}
	}	

	/// <summary>
	/// 특정 타입의 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값</param>
	template<class T>
	inline void sub4(T* vec, T val) {
		vec[0] -= val; vec[1] -= val; vec[2] -= val; vec[3] -= val;
	}

	/// <summary>
	/// 특정 타입의 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">감산할 값들</param>
	template <class T>
	inline void sub4(T* vec, const T* val) {
		vec[0] -= val[0]; vec[1] -= val[1]; vec[2] -= val[2]; vec[3] -= val[3];
	}

	/// <summary>
	/// 특정 타입의 배열에서 원하는 만큼 원하는 값을 뺍니다.
	/// </summary>
	template<class T>
	inline void subAll(T* vec, T val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			sub4<T>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] -= val;
		}
	}

	/// <summary>
	/// 특정 타입의 배열에서 다른 배열을 뺍니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">뺄 값들</param>
	/// <param name="size">배열 크기</param>
	template<class T>
	inline void subAll(T* vec, const T* val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			sub4<T>(vec + (i - 4), val + (i - 4));
		}
		for (i -= 4; i < size; i++) {
			vec[i] -= val[i];
		}
	}

	/// <summary>
	/// 특정 타입의 배열의 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값</param>
	template<class T>
	inline void mul4(T* vec, T val) {
		vec[0] *= val; vec[1] *= val; vec[2] *= val; vec[3] *= val;
	}

	/// <summary>
	/// 특정 타입의 배열의 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	template <class T>
	inline void mul4(T* vec, const T* val) {
		vec[0] *= val[0]; vec[1] *= val[1]; vec[2] *= val[2]; vec[3] *= val[3];
	}

	/// <summary>
	/// 특정 타입의 배열에 원하는 만큼 원하는 값을 곱합니다.
	/// </summary>
	template<class T>
	inline void mulAll(T* vec, T val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			mul4<T>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] *= val;
		}
	}

	/// <summary>
	/// 특정 타입의 배열에 다른 배열을 성분별로 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	/// <param name="size">배열 크기</param>
	template<class T>
	inline void mulAll(T* vec, const T* val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			mul4<T>(vec + (i - 4), val + (i - 4));
		}
		for (i -= 4; i < size; i++) {
			vec[i] *= val[i];
		}
	}

	/// <summary>
	/// 특정 타입의 배열의 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값</param>
	template<class T>
	inline void div4(T* vec, T val) {
		vec[0] /= val; vec[1] /= val; vec[2] /= val; vec[3] /= val;
	}

	/// <summary>
	/// 특정 타입의 배열의 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값들</param>
	template <class T>
	inline void div4(T* vec, const T* val) {
		vec[0] /= val[0]; vec[1] /= val[1]; vec[2] /= val[2]; vec[3] /= val[3];
	}

	/// <summary>
	/// 특정 타입의 배열을 원하는 만큼 원하는 값으로 나눕니다.
	/// </summary>
	template<class T>
	inline void divAll(T* vec, T val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			div4<T>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] /= val;
		}
	}

	/// <summary>
	/// 특정 타입의 배열을 다른 배열의 성분별로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값들</param>
	/// <param name="size">배열 크기</param>
	template<class T>
	inline void divAll(T* vec, const T* val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			div4<T>(vec + (i - 4), val + (i - 4));
		}
		for (i -= 4; i < size; i++) {
			vec[i] /= val[i];
		}
	}

	/// <summary>
	/// 특정 타입의 배열의 4개의 값을 자릅니다.
	/// </summary>
	template <class T>
	inline void clamp4(T* vec, T min, T max) {
		vec[0] = vec[0] > min ? vec[0] : min; vec[0] = vec[0] > max ? max : vec[0];
		vec[1] = vec[1] > min ? vec[1] : min; vec[1] = vec[1] > max ? max : vec[1];
		vec[2] = vec[2] > min ? vec[2] : min; vec[2] = vec[2] > max ? max : vec[2];
		vec[3] = vec[3] > min ? vec[3] : min; vec[3] = vec[3] > max ? max : vec[3];
	}

	template<class T>
	inline void clampAll(T* vec, T min, T max, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			clamp4<T>(vec + (i - 4), min, max);
		}
		for (i -= 4; i < size; i++) {
			vec[i] = vec[i] > min ? vec[i] : min;
			vec[i] = vec[i] < max ? vec[i] : max;
		}
	}

}

#if (defined(_M_IX86) || defined(_M_X64)) && !defined (_M_CEE_PURE) && !defined(OAGLE_NOSIMD)
	#include <emmintrin.h>
	#include <cstdint>
namespace onart {
	/// <summary>
	/// 실수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값</param>
	template<>
	inline void set4<float>(float* vec, float val) {
		__m128 b = _mm_set_ps1(val);
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값들</param>
	template<>
	inline void set4<float>(float* vec, const float* val) {
		__m128 b = _mm_loadu_ps(val);
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열을 원하는 값으로 초기화합니다.
	/// </summary>
	template<>
	inline void setAll<float>(float* vec, float val, size_t size) {
		size_t i = 4;
		for (; i <= size; i += 4) {
			set4<float>(vec + (i - 4), val);
		}
		for (i -= 4; i < size; i++) {
			vec[i] = val;
		}
	}

	/// <summary>
	/// 실수 배열에서 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값</param>
	template<>
	inline void add4<float>(float* vec, float val) {
		__m128 b = _mm_add_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	template<>
	inline void add4<float>(float* vec, const float* val) {
		__m128 b = _mm_add_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">감산할 값</param>
	template<>
	inline void sub4<float>(float* vec, float val) {
		__m128 b = _mm_sub_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">감산할 값들</param>
	template<>
	inline void sub4<float>(float* vec, const float* val) {
		__m128 b = _mm_sub_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값</param>
	template<>
	inline void mul4<float>(float* vec, float val) {
		__m128 b = _mm_mul_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	template<>
	inline void mul4<float>(float* vec, const float* val) {
		__m128 b = _mm_mul_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값</param>
	template<>
	inline void div4<float>(float* vec, float val) {
		__m128 b = _mm_div_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값들</param>
	template<>
	inline void div4<float>(float* vec, const float* val) {
		__m128 b = _mm_div_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// 실수 배열에서 앞 4개를 원하는 값으로 자릅니다.
	/// </summary>
	template<>
	inline void clamp4<float>(float* vec, float min, float max) {
		__m128 m = _mm_set_ps1(min); __m128 M = _mm_set_ps1(max);
		__m128 v = _mm_loadu_ps(vec);
		v = _mm_min_ps(_mm_max_ps(v, m), M);
		_mm_storeu_ps(vec, v);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값</param>
	template<>
	inline void set4<double>(double* vec, double val) {
		__m128d b = _mm_set1_pd(val);
		_mm_storeu_pd(vec, b);
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값들</param>
	template<>
	inline void set4<double>(double* vec, const double* val) {
		__m128d b = _mm_loadu_pd(val);
		_mm_storeu_pd(vec, b);
		b = _mm_loadu_pd(val + 2);
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값</param>
	template<>
	inline void add4<double>(double* vec, double val) {
		__m128d b = _mm_add_pd(_mm_loadu_pd(vec), _mm_set1_pd(val)); 
		_mm_storeu_pd(vec, b);
		b = _mm_add_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	template<>
	inline void add4<double>(double* vec, const double* val) {
		__m128d b = _mm_add_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_add_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값</param>
	template<>
	inline void sub4<double>(double* vec, double val) {
		__m128d b = _mm_sub_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_sub_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에서 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값들</param>
	template<>
	inline void sub4<double>(double* vec, const double* val) {
		__m128d b = _mm_sub_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_sub_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열의 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값</param>
	template<>
	inline void mul4<double>(double* vec, double val) {
		__m128d b = _mm_mul_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_mul_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열의 4개에 원하는 값을 곱합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	template<>
	inline void mul4<double>(double* vec, const double* val) {
		__m128d b = _mm_mul_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_mul_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열의 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값</param>
	template<>
	inline void div4<double>(double* vec, double val) {
		__m128d b = _mm_div_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_div_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열의 4개를 원하는 값으로 나눕니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">나누는 값들</param>
	template<>
	inline void div4<double>(double* vec, const double* val) {
		__m128d b = _mm_div_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_div_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 이중 정밀도 실수 배열에서 앞 4개를 원하는 값으로 자릅니다.
	/// </summary>
	template<>
	inline void clamp4<double>(double* vec, double min, double max) {
		__m128d m = _mm_set1_pd(min); __m128d M = _mm_set1_pd(max);
		__m128d v1 = _mm_loadu_pd(vec);
		__m128d v2 = _mm_loadu_pd(vec + 2);
		v1 = _mm_min_pd(_mm_max_pd(v1, m), M);
		v2 = _mm_min_pd(_mm_max_pd(v2, m), M);
		_mm_storeu_pd(vec, v1);
		_mm_storeu_pd(vec + 2, v2);
	}

	/// <summary>
	/// 32비트 정수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값</param>
	template<>
	inline void set4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_set1_epi32(val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값들</param>
	template<>
	inline void set4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_loadu_si128((__m128i*)val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값</param>
	template<>
	inline void add4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	template<>
	inline void add4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열의 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값</param>
	template<>
	inline void sub4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열의 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값들</param>
	template<>
	inline void sub4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열의 4개에 원하는 값을 곱합니다. 단, 하위 16비트끼리만 곱하므로 주의하세요.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값</param>
	template<>
	inline void mul4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 정수 배열의 4개에 원하는 값을 곱합니다. 단, 하위 16비트끼리만 곱하므로 주의하세요.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	template<>
	inline void mul4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값</param>
	template<>
	inline void set4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_set1_epi32(val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열에서 4개를 원하는 값으로 초기화합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">넣을 값들</param>
	template<>
	inline void set4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_loadu_si128((__m128i*)val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값</param>
	template<>
	inline void add4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열에 4개에 원하는 값을 누적합니다.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">누적할 값들</param>
	template<>
	inline void add4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열의 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값</param>
	template<>
	inline void sub4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열의 4개에서 원하는 값을 뺍니다.
	/// </summary>
	/// <param name="vec"> 배열</param>
	/// <param name="val">감산할 값들</param>
	template<>
	inline void sub4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열의 4개에 원하는 값을 곱합니다. 단, 하위 16비트끼리만 곱하므로 주의하세요.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값</param>
	template<>
	inline void mul4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32비트 비부호 정수 배열의 4개에 원하는 값을 곱합니다. 단, 하위 16비트끼리만 곱하므로 주의하세요.
	/// </summary>
	/// <param name="vec">배열</param>
	/// <param name="val">곱할 값들</param>
	template<>
	inline void mul4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}
}
	
#endif



#endif

