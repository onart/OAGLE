#ifndef __OAGLE_SIMD_H__
#define __OAGLE_SIMD_H__

// ���Ϳ� ��� ���� �Ʒ����� �۵��ϱ� ���� SIMD ���̺귯���Դϴ�. ������ �� ������� �������� OAGLE_NOSIMD�� ������ �ּ���.
// �� �Լ����� ��� �˻縦 ���� �ʽ��ϴ�. �ε��� [3]���� ����ϹǷ�, Segmentation fault �߻��� ������ �ּ���.

namespace onart {

	/// <summary>
	/// Ư�� Ÿ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template <class T>
	inline void set4(T* vec, T val) {
		vec[0] = val; vec[1] = val; vec[2] = val; vec[3] = val;
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template <class T>
	inline void set4(T* vec, const T* val) {
		vec[0] = val[0]; vec[1] = val[1]; vec[2] = val[2]; vec[3] = val[3];
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<class T>
	inline void add4(T* vec, T val) {
		vec[0] += val; vec[1] += val; vec[2] += val; vec[3] += val;
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template <class T>
	inline void add4(T* vec, const T* val) {
		vec[0] += val[0]; vec[1] += val[1]; vec[2] += val[2]; vec[3] += val[3];
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ��</param>
	template<class T>
	inline void sub4(T* vec, T val) {
		vec[0] -= val; vec[1] -= val; vec[2] -= val; vec[3] -= val;
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template <class T>
	inline void sub4(T* vec, const T* val) {
		vec[0] -= val[0]; vec[1] -= val[1]; vec[2] -= val[2]; vec[3] -= val[3];
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<class T>
	inline void mul4(T* vec, T val) {
		vec[0] *= val; vec[1] *= val; vec[2] *= val; vec[3] *= val;
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template <class T>
	inline void mul4(T* vec, const T* val) {
		vec[0] *= val[0]; vec[1] *= val[1]; vec[2] *= val[2]; vec[3] *= val[3];
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<class T>
	inline void div4(T* vec, T val) {
		vec[0] /= val; vec[1] /= val; vec[2] /= val; vec[3] /= val;
	}

	/// <summary>
	/// Ư�� Ÿ���� �迭�� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template <class T>
	inline void div4(T* vec, const T* val) {
		vec[0] /= val[0]; vec[1] /= val[1]; vec[2] /= val[2]; vec[3] /= val[3];
	}

}

#if (defined(_M_IX86) || defined(_M_X64)) && !defined (_M_CEE_PURE) && !defined(OAGLE_NOSIMD)
	#include <emmintrin.h>
	#include <cstdint>
namespace onart {

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void set4<float>(float* vec, float val) {
		__m128 b = _mm_set_ps1(val);
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void set4<float>(float* vec, const float* val) {
		__m128 b = _mm_loadu_ps(val);
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void add4<float>(float* vec, float val) {
		__m128 b = _mm_add_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void add4<float>(float* vec, const float* val) {
		__m128 b = _mm_add_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void sub4<float>(float* vec, float val) {
		__m128 b = _mm_sub_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void sub4<float>(float* vec, const float* val) {
		__m128 b = _mm_sub_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void mul4<float>(float* vec, float val) {
		__m128 b = _mm_mul_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void mul4<float>(float* vec, const float* val) {
		__m128 b = _mm_mul_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void div4<float>(float* vec, float val) {
		__m128 b = _mm_div_ps(_mm_loadu_ps(vec), _mm_set_ps1(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// �Ǽ� �迭���� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void div4<float>(float* vec, const float* val) {
		__m128 b = _mm_div_ps(_mm_loadu_ps(vec), _mm_loadu_ps(val));
		_mm_storeu_ps(vec, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void set4<double>(double* vec, double val) {
		__m128d b = _mm_set1_pd(val);
		_mm_storeu_pd(vec, b);
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void set4<double>(double* vec, const double* val) {
		__m128d b = _mm_loadu_pd(val);
		_mm_storeu_pd(vec, b);
		b = _mm_loadu_pd(val + 2);
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void add4<double>(double* vec, double val) {
		__m128d b = _mm_add_pd(_mm_loadu_pd(vec), _mm_set1_pd(val)); 
		_mm_storeu_pd(vec, b);
		b = _mm_add_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void add4<double>(double* vec, const double* val) {
		__m128d b = _mm_add_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_add_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void sub4<double>(double* vec, double val) {
		__m128d b = _mm_sub_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_sub_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭���� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void sub4<double>(double* vec, const double* val) {
		__m128d b = _mm_sub_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_sub_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void mul4<double>(double* vec, double val) {
		__m128d b = _mm_mul_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_mul_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ���� ���մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void mul4<double>(double* vec, const double* val) {
		__m128d b = _mm_mul_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_mul_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void div4<double>(double* vec, double val) {
		__m128d b = _mm_div_pd(_mm_loadu_pd(vec), _mm_set1_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_div_pd(_mm_loadu_pd(vec + 2), _mm_set1_pd(val));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// ���� ���е� �Ǽ� �迭�� 4���� ���ϴ� ������ �����ϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void div4<double>(double* vec, const double* val) {
		__m128d b = _mm_div_pd(_mm_loadu_pd(vec), _mm_loadu_pd(val));
		_mm_storeu_pd(vec, b);
		b = _mm_div_pd(_mm_loadu_pd(vec + 2), _mm_loadu_pd(val + 2));
		_mm_storeu_pd(vec + 2, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void set4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_set1_epi32(val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void set4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_loadu_si128((__m128i*)val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void add4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void add4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void sub4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void sub4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4���� ���ϴ� ���� ���մϴ�. ��, ���� 16��Ʈ������ ���ϹǷ� �����ϼ���.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void mul4<int32_t>(int32_t* vec, int32_t val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���� �迭�� 4���� ���ϴ� ���� ���մϴ�. ��, ���� 16��Ʈ������ ���ϹǷ� �����ϼ���.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void mul4<int32_t>(int32_t* vec, const int32_t* val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void set4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_set1_epi32(val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭���� 4���� ���ϴ� ������ �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void set4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_loadu_si128((__m128i*)val);
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void add4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4���� ���ϴ� ���� �����մϴ�.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void add4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_add_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ��</param>
	template<>
	inline void sub4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4������ ���ϴ� ���� ���ϴ�.
	/// </summary>
	/// <param name="vec"> �迭</param>
	/// <param name="val">������ ����</param>
	template<>
	inline void sub4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_sub_epi32(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4���� ���ϴ� ���� ���մϴ�. ��, ���� 16��Ʈ������ ���ϹǷ� �����ϼ���.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ��</param>
	template<>
	inline void mul4<uint32_t>(uint32_t* vec, uint32_t val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_set1_epi32(val));
		_mm_storeu_si128((__m128i*)vec, b);
	}

	/// <summary>
	/// 32��Ʈ ���ȣ ���� �迭�� 4���� ���ϴ� ���� ���մϴ�. ��, ���� 16��Ʈ������ ���ϹǷ� �����ϼ���.
	/// </summary>
	/// <param name="vec">�迭</param>
	/// <param name="val">���� ����</param>
	template<>
	inline void mul4<uint32_t>(uint32_t* vec, const uint32_t* val) {
		__m128i b = _mm_mullo_epi16(_mm_loadu_si128((__m128i*)vec), _mm_loadu_si128((__m128i*)val));
		_mm_storeu_si128((__m128i*)vec, b);
	}
}
	
#endif



#endif

