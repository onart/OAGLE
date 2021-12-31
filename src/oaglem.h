#ifndef __OAGLEM_H__
#define __OAGLEM_H__

/*
* 수학 함수
* 구성:
* nvec, mat2, mat3, mat4, Quaternion
* 다른 모듈로 대체하는 법:
*	namespace onart{
*		using mat4 =(다른모듈 4x4행렬);
*	}
*/

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cassert>

constexpr float PI = 3.14159265358979323846f;

/// <summary>
/// 라디안을 입력하면 도 단위로 변경합니다.
/// </summary>
constexpr float deg(float rad) { return rad * 180 / PI; }
/// <summary>
/// 도 단위를 입력하면 라디안으로 변경합니다.
/// </summary>
constexpr float rad(float deg) { return deg / 180 * PI; }

namespace onart {
	struct Quaternion;
	/// <summary>
	/// N차원 벡터입니다. 길이에 관계없이 상호 변환이 가능합니다.
	/// </summary>
	/// <typeparam name="T">벡터 성분의 타입입니다. 사칙 연산 및 부호 반전이 가능하여야 합니다.</typeparam>
	template <unsigned D, class T = float> union nvec {
		T entry[D];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };

		/// <summary>
		/// 영벡터를 생성합니다.
		/// </summary>
		nvec() { for (int i = 0; i < D; i++) entry[i] = 0; }

		/// <summary>
		/// 벡터의 모든 값을 하나의 값으로 초기화합니다.
		/// </summary>
		nvec(T a) { for (int i = 0; i < D; i++) entry[i] = a; }

		/// <summary>
		/// 벡터의 값 중 앞 2~4개를 초기화합니다.
		/// </summary>
		nvec(T x, T y, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {  }

		/// <summary>
		/// 복사 생성자입니다.
		/// </summary>
		nvec(const nvec& v) { for (int i = 0; i < D; i++) entry[i] = v.entry[i]; }

		/// <summary>
		/// 다른 차원의 벡터를 사용하는 복사 생성자입니다. 가급적 차원 축소에만 사용하는 것이 좋습니다.
		/// </summary>
		template <unsigned E> nvec(const nvec<E, T>& v) { unsigned min = D > E ? E : D; int i = 0; for (; i < min; i++) entry[i] = v.entry[i]; for (; i < D; i++) entry[i] = 0; }

		/// <summary>
		/// 벡터의 모든 성분을 하나의 값으로 초기화합니다. operator=과 동일합니다.
		/// </summary>
		inline void set(T a) { for (int i = 0; i < D; i++) entry[i] = a; }
		
		/// <summary>
		/// 다른 벡터의 값을 복사해 옵니다. operator=과 동일합니다.
		/// </summary>
		template <unsigned E> inline void set(const nvec<E, T>& v) {
			unsigned min = D > E ? E : D;
			for (unsigned i = 0; i < min; i++) {
				entry[i] = v.entry[i];
			}
		}

		/// <summary>
		/// 벡터의 모든 성분을 하나의 값으로 초기화합니다. set()과 동일합니다.
		/// </summary>
		inline nvec& operator=(T a) { set(a); return *this; }

		/// <summary>
		/// 다른 벡터의 값을 복사해 옵니다. set()과 동일합니다.
		/// </summary>
		template <unsigned E> inline nvec& operator=(const nvec<E, T>& v) { set(v); return *this; }

		/// <summary>
		/// 다른 벡터와 성분별 연산을 차원수가 낮은 쪽을 기준으로 합니다.
		/// </summary>
		template <unsigned E> inline nvec& operator+=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] += v[i]; }
		template <unsigned E> inline nvec& operator-=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] -= v[i]; }
		template <unsigned E> inline nvec& operator*=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] *= v[i]; }
		template <unsigned E> inline nvec& operator/=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] /= v[i]; }

		/// <summary>
		/// 벡터의 모든 성분에 대하여 주어진 값과 연산합니다.
		/// </summary>
		inline nvec& operator+=(T a) { for (int i = 0; i < D; i++) entry[i] += a; return *this; }
		inline nvec& operator-=(T a) { for (int i = 0; i < D; i++) entry[i] -= a; return *this; }
		inline nvec& operator*=(T a) { for (int i = 0; i < D; i++) entry[i] *= a; return *this; }
		inline nvec& operator/=(T a) { for (int i = 0; i < D; i++) entry[i] /= a; return *this; }
		inline nvec operator+(T a) const { auto r(*this); r += a; return r; }
		inline nvec operator-(T a) const { auto r(*this); r -= a; return r; }
		inline nvec operator*(T a) const { auto r(*this); r *= a; return r; }
		inline nvec operator/(T a) const { auto r(*this); r /= a; return r; }

		/// <summary>
		/// 벡터의 모든 성분이 동일한 경우 참을 리턴합니다. 다른 크기의 벡터와의 비교를 지원하지 않습니다.
		/// </summary>
		inline bool operator==(const nvec& v) const { for (int i = 0; i < D; i++)if (entry[i] != v[i]) return false; return true; }
		inline bool operator!=(const nvec& v) const { for (int i = 0; i < D; i++)if (entry[i] != v[i]) return true; return false; }

		/// <summary>
		/// 다른 벡터와 성분별 연산을 차원수가 낮은 쪽을 기준으로 합니다. 리턴 차원수는 항상 좌변값과 동일합니다.
		/// </summary>
		template <unsigned E> inline nvec operator+(const nvec<E, T>& v) const { auto r(*this); r += v; return r; }
		template <unsigned E> inline nvec operator-(const nvec<E, T>& v) const { auto r(*this); r -= v; return r; }
		template <unsigned E> inline nvec operator*(const nvec<E, T>& v) const { auto r(*this); r *= v; return r; }
		template <unsigned E> inline nvec operator/(const nvec<E, T>& v) const { auto r(*this); r /= v; return r; }

		/// <summary>
		/// T형 배열로 사용할 수 있도록 포인터를 리턴합니다.
		/// </summary>
		inline operator T* () { return &x; }
		inline operator const T* () const { return &x; }

		/// <summary>
		/// 명시적 캐스트가 가능한 타입이라면 벡터도 명시적으로 캐스트가 가능합니다.
		/// 성분별로 캐스트해서 새로 만드는 것과 비교하여 특별히 성능적 면에서 나을 부분은 없으며
		/// 유연한 코드만을 위해 추가하였습니다.
		/// </summary>
		template <class T2> inline operator nvec<D, T2>() { nvec<D, T2> n; for (unsigned i = 0; i < D; i++) { n[i] = (T2)entry[i]; } return n; }

		/// <summary>
		/// 부호를 반전시켜 리턴합니다.
		/// </summary>
		inline nvec operator-() const { return (*this) * -1; }

		/// <summary>
		/// 인덱스 연산자
		/// </summary>
		inline T& operator[](ptrdiff_t i) { assert(i < D); return entry[i]; }
		inline const T& operator[](ptrdiff_t i) const { assert(i < D); return entry[i]; }

		/// <summary>
		/// 벡터의 방향을 유지하고 길이를 1로 맞춥니다. 정수 벡터에서는 사용할 수 없습니다.
		/// </summary>
		inline nvec normalize() const { return (*this) / length(); }

		/// <summary>
		/// 벡터 길이의 제곱을 리턴합니다.
		/// </summary>
		inline float length2() const { float s = 0; for (int i = 0; i < D; i++) s += entry[i] * entry[i]; return s; }

		/// <summary>
		/// 벡터 길이를 리턴합니다.
		/// </summary>
		inline float length() const { return sqrtf(length2()); }

		/// <summary>
		/// 다른 벡터와의 거리 제곱을 리턴합니다.
		/// </summary>
		inline float distance2(const nvec& v) const { return (*this - v).length2(); }

		/// <summary>
		/// 다른 벡터와의 거리를 리턴합니다.
		/// </summary>
		inline float distance(const nvec& v) const { return sqrtf(distance(v)); }

		/// <summary>
		/// 다른 벡터와의 내적을 리턴합니다. 다른 차원과의 연산을 지원하지 않습니다.
		/// </summary>
		inline T dot(const nvec& v) const { T s = 0; for (int i = 0; i < D; i++)s += entry[i] * v[i]; return s; }
	};

	using vec2 = nvec<2>;					using vec3 = nvec<3>;					using vec4 = nvec<4>;
	using ivec2 = nvec<2, int>;				using ivec3 = nvec<3, int>;				using ivec4 = nvec<4, int>;
	using uvec2 = nvec<2, unsigned>;		using uvec3 = nvec<3, unsigned>;		using uvec4 = nvec<4, unsigned>;
	using dvec2 = nvec<2, double>;			using dvec3 = nvec<3, double>;			using dvec4 = nvec<4, double>;

	/// <summary>
	/// 2개 3차원 실수 벡터의 외적을 계산합니다.
	/// </summary>
	inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	/// <summary>
	/// 2개의 벡터를 선형 보간합니다.
	/// </summary>
	/// <param name="a">선형 보간 대상 1(t=0에 가까울수록 이 벡터에 가깝습니다.)</param>
	/// <param name="b">선형 보간 대상 2(t=1에 가까울수록 이 벡터에 가깝습니다.)</param>
	/// <param name="t">선형 보간 값</param>	
	template <unsigned D, class T> inline nvec<D, T> lerp(const nvec<D, T>& a, const nvec<D, T>& b, const nvec<D, float>& t) { return a * (1 - t) + b * t; }

	/// <summary>
	/// 2차원 이미지의 회전연산을 위한 2x2 행렬입니다. 단, 3차원 연산의 z축을 0으로 고정하는 것이 더 일반적인 방법입니다.
	/// </summary>
	struct mat2 {
		union { float a[4]; struct { float _11, _12, _21, _22; }; };

		/// <summary>
		/// 단위행렬을 생성합니다.
		/// </summary>
		inline mat2() { _11 = _22 = 1; _12 = _21 = 0; }

		/// <summary>
		/// 행 우선 순서로 매개변수를 주어 행렬을 생성합니다.
		/// </summary>
		inline mat2(float _11, float _12, float _21, float _22) :_11(_11), _12(_12), _21(_21), _22(_22) { }
		
		/// <summary>
		/// 인덱스 연산자
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// 복사 생성
		/// </summary>
		inline mat2(const mat2& m) { for (int i = 0; i < 4; i++) { a[i] = m[i]; } }

		/// <summary>
		/// 행렬을 단위행렬로 바꿉니다.
		/// </summary>
		inline void toI() { _11 = _22 = 1; _12 = _21 = 0; }

		/// <summary>
		/// 다른 행렬과 성분별로 더하거나 뺍니다.
		/// </summary>
		inline mat2& operator+=(const mat2& m) { for (int i = 0; i < 4; i++)a[i] += m[i]; return *this; }
		inline mat2& operator-=(const mat2& m) { for (int i = 0; i < 4; i++)a[i] -= m[i]; return *this; }
		inline mat2 operator+(const mat2& m) const { auto r = mat2(*this); r += m; return r; }
		inline mat2 operator-(const mat2& m) const { auto r = mat2(*this); r -= m; return r; }

		/// <summary>
		/// 행렬곱을 수행합니다.
		/// </summary>
		inline mat2 operator*(const mat2& m) const {
			return mat2(
				_11 * m._11 + _12 * m._21,
				_11 * m._12 + _12 * m._22,

				_21 * m._11 + _22 * m._21,
				_21 * m._12 + _22 * m._22
			);
		}
		inline mat2& operator*=(const mat2& m) { return *this = operator*(m); }

		/// <summary>
		/// 벡터에 선형변환을 적용하여 리턴합니다.
		/// </summary>
		inline vec2 operator*(const vec2& v) const { return vec2(_11 * v.x + _12 * v.y, _21 * v.x + _22 * v.y); }

		/// <summary>
		/// 행렬에 실수배를 합니다.
		/// </summary>
		inline mat2& operator*=(float f) { for (int i = 0; i < 4; i++) a[i] *= f; }
		inline mat2 operator*(float f) const { mat2 r(*this); r *= f; return r; }
		inline mat2& operator/=(float f) { for (int i = 0; i < 4; i++) a[i] /= f; }
		inline mat2 operator/(float f) const { mat2 r(*this); r /= f; return r; }

		/// <summary>
		/// 행렬식을 반환합니다.
		/// </summary>
		inline float det() const { return _11 * _22 - _12 * _21; }

		/// <summary>
		/// 역행렬을 반환합니다.
		/// </summary>
		inline mat2 inverse() const {
			float d = det();
			if (d == 0) printf("%p: 이 행렬은 역행렬이 없거나 매우 큰 성분을 가집니다. NaN에 의해 예기치 못한 동작이 발생할 수 있습니다.\n", this);
			return mat2(_22, -_12, -_21, _11) / d;
		}

		/// <summary>
		/// 전치 행렬을 반환합니다.
		/// </summary>
		inline mat2 transpose() const { return mat2(_11, _21, _12, _22); }

		/// <summary>
		/// 행 우선 순서로 된 배열을 리턴합니다.
		/// </summary>
		inline operator float* () { return a; }
	};

	/// <summary>
	/// 3차원 모델의 회전연산 혹은 2차원 이미지의 아핀 변환을 위한 3x3 행렬입니다.
	/// </summary>
	struct mat3 {
		union { float a[9]; struct { float _11, _12, _13, _21, _22, _23, _31, _32, _33; }; };

		/// <summary>
		/// 단위행렬을 생성합니다.
		/// </summary>
		inline mat3() { _11 = _22 = _33 = 1; _12 = _13 = _21 = _23 = _31 = _32 = 0; }

		/// <summary>
		/// 행 우선 순서로 매개변수를 주어 행렬을 생성합니다.
		/// </summary>
		inline mat3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33) :_11(_11), _12(_12), _13(_13), _21(_21), _22(_22), _23(_23), _31(_31), _32(_32), _33(_33) { }

		/// <summary>
		/// 인덱스 연산자
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// 복사 생성
		/// </summary>
		inline mat3(const mat3& m) { for (int i = 0; i < 9; i++) { a[i] = m[i]; } }

		/// <summary>
		/// 행렬을 단위행렬로 바꿉니다.
		/// </summary>
		inline void toI() { _11 = _22 = _33 = 1; _12 = _13 = _21 = _23 = _31 = _32 = 0; }

		/// <summary>
		/// 다른 행렬과 성분별로 더하거나 뺍니다.
		/// </summary>
		inline mat3& operator+=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] += m[i]; return *this; }
		inline mat3& operator-=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] -= m[i]; return *this; }
		inline mat3 operator+(const mat3& m) const { auto r = mat3(*this); r += m; return r; }
		inline mat3 operator-(const mat3& m) const { auto r = mat3(*this); r -= m; return r; }

		/// <summary>
		/// 행렬곱을 수행합니다.
		/// </summary>
		inline mat3 operator*(const mat3& m) const {
			return mat3(
				_11 * m._11 + _12 * m._21 + _13 * m._31,
				_11 * m._12 + _12 * m._22 + _13 * m._32,
				_11 * m._13 + _12 * m._23 + _13 * m._33,

				_21 * m._11 + _22 * m._21 + _23 * m._31,
				_21 * m._12 + _22 * m._22 + _23 * m._32,
				_21 * m._13 + _22 * m._23 + _23 * m._33,

				_31 * m._11 + _32 * m._21 + _33 * m._31,
				_31 * m._12 + _32 * m._22 + _33 * m._32,
				_31 * m._13 + _32 * m._23 + _33 * m._33
			);
		}
		inline mat3& operator*=(const mat3& m) { return *this = operator*(m); }

		/// <summary>
		/// 벡터에 선형변환을 적용하여 리턴합니다.
		/// </summary>
		inline vec3 operator*(const vec3& v) const { return vec3(_11 * v.x + _12 * v.y + _13 * v.z, _21 * v.x + _22 * v.y + _23 * v.z, _31 * v.x + _32 * v.y + _33 * v.z); }

		/// <summary>
		/// 행렬에 실수배를 합니다.
		/// </summary>
		inline mat3& operator*=(float f) { for (int i = 0; i < 9; i++) a[i] *= f; }
		inline mat3 operator*(float f) const { mat3 r(*this); r *= f; return r; }
		inline mat3& operator/=(float f) { for (int i = 0; i < 9; i++) a[i] /= f; }
		inline mat3 operator/(float f) const { mat3 r(*this); r /= f; return r; }

		/// <summary>
		/// 행렬식을 반환합니다.
		/// </summary>
		inline float det() const { return _11 * (_22 * _33 - _23 * _32) + _12 * (_23 * _31 - _21 * _33) + _13 * (_21 * _32 - _22 * _31); }

		/// <summary>
		/// 역행렬을 반환합니다.
		/// </summary>
		inline mat3 inverse() const {
			float d = det();
			if (d == 0) printf("%p: 이 행렬은 역행렬이 없거나 매우 큰 성분을 가집니다. NaN에 의해 예기치 못한 동작이 발생할 수 있습니다.\n", this);
			return mat3((_22 * _33 - _32 * _23), (_13 * _32 - _12 * _33), (_12 * _23 - _13 * _22), (_23 * _31 - _21 * _33), (_11 * _33 - _13 * _31), (_21 * _13 - _11 * _23), (_21 * _32 - _31 * _22), (_31 * _12 - _11 * _32), (_11 * _22 - _21 * _12)) / d;
		}

		/// <summary>
		/// 전치 행렬을 반환합니다.
		/// </summary>
		inline mat3 transpose() const { return mat3(_11, _21, _31, _12, _22, _32, _13, _23, _33); }

		/// <summary>
		/// 행 우선 순서로 된 배열을 리턴합니다.
		/// </summary>
		inline operator float* () { return a; }

		/// <summary>
		/// 좌측 상단 2x2 행렬로 캐스트합니다.
		/// </summary>
		inline operator mat2() { return mat2(_11, _12, _21, _22); }

		/// <summary>
		/// 2차원 병진 행렬을 계산합니다.
		/// </summary>
		inline static mat3 translate(const vec2& t) {
			return mat3(
				1, 0, t.x,
				0, 1, t.y,
				0, 0, 1
			);
		}
		/// <summary>
		/// 2차원 병진 행렬을 계산합니다.
		/// </summary>
		inline static mat3 translate(float x, float y) {
			return mat3(
				1, 0, x,
				0, 1, y,
				0, 0, 1
			);
		}

		/// <summary>
		/// 2차원 크기 변환 행렬을 계산합니다.
		/// </summary>
		inline static mat3 scale(const vec2& t) {
			return mat3(
				t.x, 0, 0,
				0, t.y, 0,
				0, 0, 1
			);
		}
		/// <summary>
		/// 2차원 크기 변환 행렬을 계산합니다.
		/// </summary>
		inline static mat3 scale(float x, float y) {
			return mat3(
				x, 0, 0,
				0, y, 0,
				0, 0, 1
			);
		}

		/// <summary>
		/// Z축 기준의 2차원 회전을 리턴합니다. X, Y 축 방향을 원하는 경우 3x3 아핀 변환이 아닌 4x4 아핀 변환을 사용해야 합니다.
		/// </summary>
		inline static mat3 rotate(float z) {
			return mat3(
				cosf(z), -sinf(z), 0,
				sinf(z), cosf(z), 0,
				0, 0, 1
			);
		}

		/// <summary>
		/// Z축 기준의 3차원 회전을 리턴합니다.
		/// </summary>
		/// <param name="roll">roll(X축 방향 회전)</param>
		/// <param name="pitch">pitch(Y축 방향 회전)</param>
		/// <param name="yaw">yaw(Z축 방향 회전)</param>
		inline static mat3 rotate(float roll, float pitch, float yaw);
	};

	/// <summary>
	/// 3차원 모델의 아핀 변환을 위한 4x4 행렬입니다.
	/// </summary>
	struct mat4 {
		union { float a[16]; struct { float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; }; };

		/// <summary>
		/// 단위행렬을 생성합니다.
		/// </summary>
		inline mat4() { _11 = _22 = _33 = _44 = 1; _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0; }

		/// <summary>
		/// 행 우선 순서로 매개변수를 주어 행렬을 생성합니다.
		/// </summary>
		inline mat4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44) :_11(_11), _12(_12), _13(_13), _14(_14), _21(_21), _22(_22), _23(_23), _24(_24), _31(_31), _32(_32), _33(_33), _34(_34), _41(_41), _42(_42), _43(_43), _44(_44) { }

		/// <summary>
		/// 인덱스 연산자
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// 복사 생성
		/// </summary>
		inline mat4(const mat4& m) { for (int i = 0; i < 16; i++) { a[i] = m[i]; } }

		/// <summary>
		/// 행렬을 단위행렬로 바꿉니다.
		/// </summary>
		inline void toI() { _11 = _22 = _33 = _44 = 1; _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0; }

		/// <summary>
		/// 다른 행렬과 성분별로 더하거나 뺍니다.
		/// </summary>
		inline mat4& operator+=(const mat4& m) { for (int i = 0; i < 16; i++)a[i] += m[i]; return *this; }
		inline mat4& operator-=(const mat4& m) { for (int i = 0; i < 16; i++)a[i] -= m[i]; return *this; }
		inline mat4 operator+(const mat4& m) const { auto r = mat4(*this); r += m; return r; }
		inline mat4 operator-(const mat4& m) const { auto r = mat4(*this); r -= m; return r; }

		/// <summary>
		/// 행렬곱을 수행합니다.
		/// </summary>
		inline mat4 operator*(const mat4& m) const {
			return mat4(
				_11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41,
				_11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42,
				_11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43,
				_11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44,

				_21* m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41,
				_21* m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42,
				_21* m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43,
				_21* m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44,

				_31* m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41,
				_31* m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42,
				_31* m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43,
				_31* m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44,

				_41* m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41,
				_41* m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42,
				_41* m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43,
				_41* m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44
			);
		}
		inline mat4& operator*=(const mat4& m) { return *this = operator*(m); }

		/// <summary>
		/// 벡터에 선형변환을 적용하여 리턴합니다.
		/// </summary>
		inline vec4 operator*(const vec4& v) const { return vec4(_11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w, _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w, _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w, _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w); }

		/// <summary>
		/// 행렬에 실수배를 합니다.
		/// </summary>
		inline mat4& operator*=(float f) { for (int i = 0; i < 16; i++) a[i] *= f; }
		inline mat4 operator*(float f) const { mat4 r(*this); r *= f; return r; }
		inline mat4& operator/=(float f) { for (int i = 0; i < 16; i++) a[i] /= f; }
		inline mat4 operator/(float f) const { mat4 r(*this); r /= f; return r; }

		/// <summary>
		/// 행렬식을 반환합니다.
		/// </summary>
		inline float det() const {
			return 
				_41 * _32 * _23 * _14 - _31 * _42 * _23 * _14 - _41 * _22 * _33 * _14 + _21 * _42 * _33 * _14 +
				_31 * _22 * _43 * _14 - _21 * _32 * _43 * _14 - _41 * _32 * _13 * _24 + _31 * _42 * _13 * _24 +
				_41 * _12 * _33 * _24 - _11 * _42 * _33 * _24 - _31 * _12 * _43 * _24 + _11 * _32 * _43 * _24 +
				_41 * _22 * _13 * _34 - _21 * _42 * _13 * _34 - _41 * _12 * _23 * _34 + _11 * _42 * _23 * _34 +
				_21 * _12 * _43 * _34 - _11 * _22 * _43 * _34 - _31 * _22 * _13 * _44 + _21 * _32 * _13 * _44 +
				_31 * _12 * _23 * _44 - _11 * _32 * _23 * _44 - _21 * _12 * _33 * _44 + _11 * _22 * _33 * _44;
		}

		/// <summary>
		/// 역행렬을 반환합니다.
		/// </summary>
		inline mat4 inverse() const {
			float d = det();
			if (d == 0) printf("%p: 이 행렬은 역행렬이 없거나 매우 큰 성분을 가집니다. NaN에 의해 예기치 못한 동작이 발생할 수 있습니다.\n", this);
			return mat4((_32 * _43 * _24 - _42 * _33 * _24 + _42 * _23 * _34 - _22 * _43 * _34 - _32 * _23 * _44 + _22 * _33 * _44),
				(_42 * _33 * _14 - _32 * _43 * _14 - _42 * _13 * _34 + _12 * _43 * _34 + _32 * _13 * _44 - _12 * _33 * _44),
				(_22 * _43 * _14 - _42 * _23 * _14 + _42 * _13 * _24 - _12 * _43 * _24 - _22 * _13 * _44 + _12 * _23 * _44),
				(_32 * _23 * _14 - _22 * _33 * _14 - _32 * _13 * _24 + _12 * _33 * _24 + _22 * _13 * _34 - _12 * _23 * _34),

				(_41 * _33 * _24 - _31 * _43 * _24 - _41 * _23 * _34 + _21 * _43 * _34 + _31 * _23 * _44 - _21 * _33 * _44),
				(_31 * _43 * _14 - _41 * _33 * _14 + _41 * _13 * _34 - _11 * _43 * _34 - _31 * _13 * _44 + _11 * _33 * _44),
				(_41 * _23 * _14 - _21 * _43 * _14 - _41 * _13 * _24 + _11 * _43 * _24 + _21 * _13 * _44 - _11 * _23 * _44),
				(_21 * _33 * _14 - _31 * _23 * _14 + _31 * _13 * _24 - _11 * _33 * _24 - _21 * _13 * _34 + _11 * _23 * _34),

				(_31 * _42 * _24 - _41 * _32 * _24 + _41 * _22 * _34 - _21 * _42 * _34 - _31 * _22 * _44 + _21 * _32 * _44),
				(_41 * _32 * _14 - _31 * _42 * _14 - _41 * _12 * _34 + _11 * _42 * _34 + _31 * _12 * _44 - _11 * _32 * _44),
				(_21 * _42 * _14 - _41 * _22 * _14 + _41 * _12 * _24 - _11 * _42 * _24 - _21 * _12 * _44 + _11 * _22 * _44),
				(_31 * _22 * _14 - _21 * _32 * _14 - _31 * _12 * _24 + _11 * _32 * _24 + _21 * _12 * _34 - _11 * _22 * _34),

				(_41 * _32 * _23 - _31 * _42 * _23 - _41 * _22 * _33 + _21 * _42 * _33 + _31 * _22 * _43 - _21 * _32 * _43),
				(_31 * _42 * _13 - _41 * _32 * _13 + _41 * _12 * _33 - _11 * _42 * _33 - _31 * _12 * _43 + _11 * _32 * _43),
				(_41 * _22 * _13 - _21 * _42 * _13 - _41 * _12 * _23 + _11 * _42 * _23 + _21 * _12 * _43 - _11 * _22 * _43),
				(_21 * _32 * _13 - _31 * _22 * _13 + _31 * _12 * _23 - _11 * _32 * _23 - _21 * _12 * _33 + _11 * _22 * _33)) / d;
		}

		/// <summary>
		/// 전치 행렬을 반환합니다.
		/// </summary>
		inline mat4 transpose() const { return mat4(_11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44); }

		/// <summary>
		/// 행 우선 순서로 된 배열을 리턴합니다.
		/// </summary>
		inline operator float* () { return a; }
		inline operator const float* () const { return a; }

		/// <summary>
		/// 좌측 상단 3x3 행렬로 캐스트합니다.
		/// </summary>
		inline operator mat3() { return mat3(_11, _12, _13, _21, _22, _23, _31, _32, _33); }

		/// <summary>
		/// 3차원 병진 행렬을 계산합니다.
		/// </summary>
		inline static mat4 translate(const vec3& t) {
			return mat4(
				1, 0, 0, t.x,
				0, 1, 0, t.y,
				0, 0, 1, t.z,
				0, 0, 0, 1
			);
		}
		/// <summary>
		/// 3차원 병진 행렬을 계산합니다.
		/// </summary>
		inline static mat4 translate(float x, float y, float z) {
			return mat4(
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1
			);
		}

		/// <summary>
		/// 3차원 크기 변환 행렬을 계산합니다.
		/// </summary>
		inline static mat4 scale(const vec3& t) {
			return mat4(
				t.x, 0, 0, 0,
				0, t.y, 0, 0,
				0, 0, t.z, 0,
				0, 0, 0, 1
			);
		}
		/// <summary>
		/// 3차원 크기 변환 행렬을 계산합니다.
		/// </summary>
		inline static mat4 scale(float x, float y, float z) {
			return mat4(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			);
		}

		/// <summary>
		/// 3차원 회전 행렬을 계산합니다.
		/// </summary>
		inline static mat4 rotate(const vec3& axis, float angle);

		/// <summary>
		/// 3차원 회전 행렬을 계산합니다. cf) 오른손 법칙
		/// </summary>
		/// <param name="roll">roll(X축 방향 회전)</param>
		/// <param name="pitch">pitch(Y축 방향 회전)</param>
		/// <param name="yaw">yaw(Z축 방향 회전)</param>
		inline static mat4 rotate(float roll, float pitch, float yaw);

		/// <summary>
		/// lookAt 형식의 뷰 행렬을 계산합니다.
		/// </summary>
		/// <param name="eye">눈의 위치</param>
		/// <param name="at">피사체 위치</param>
		/// <param name="up">위쪽 방향: 화면 상에서 위쪽 방향이 이 벡터의 방향과 비슷해집니다.</param>
		inline static mat4 lookAt(const vec3& eye, const vec3& at, const vec3& up) {
			vec3 n = (eye - at).normalize();
			vec3 u = cross(up, n).normalize();
			vec3 v = cross(n, u).normalize();
			return mat4(
					u.x, u.y, u.z, -(u.dot(eye)),
					v.x, v.y, v.z, -(v.dot(eye)),
					n.x, n.y, n.z, -(n.dot(eye)),
					0, 0, 0, 1
				);
		}
		/// <summary>
		/// 병진, 회전, 배율 행렬 T, R, S를 구한 후 곱하는 것보다 조금 더 빠르게 계산합니다.
		/// </summary>
		/// <param name="translation">병진</param>
		/// <param name="rotation">회전</param>
		/// <param name="scale">배율</param>
		inline static mat4 TRS(const vec3& translation, const Quaternion& rotation, const vec3& scale);
		/// <summary>
		/// 표준 뷰 볼륨 직육면체에 들어올 대상 뿔대(절두체)를 조절하는 투사 행렬을 계산합니다.
		/// 순수 2D 게임을 만드는 경우, 단위 행렬에 aspect만 적용하면 됩니다.
		/// </summary>
		/// <param name="fovy">field of view Y: 뿔대의 Y축 방향(화면 기준 세로) 라디안 각도입니다.</param>
		/// <param name="aspect">표시 뷰포트 비율(가로/세로)입니다.</param>
		/// <param name="dnear">뿔대에서 가장 가까운 거리입니다. 이 이하의 거리는 보는 눈보다 뒤에 있는 것으로 칩니다.</param>
		/// <param name="dfar">뿔대에서 가장 먼 거리입니다. 이 이상의 거리는 보이지 않습니다.</param>
		inline static mat4 perspective(float fovy, float aspect, float dnear, float dfar) {
			mat4 r(
				1, 0,					0, 0,
				0, 1 / tanf(fovy / 2),	0, 0,
				0, 0,	(dnear + dfar) / (dnear - dfar), 0,
				0, 0,					-1, 0
			);
			r._11 = r._22 / aspect;
			return r;
		}
	};

	/// <summary>
	/// 3차원 회전 등을 표현하는 사원수입니다. 1, i, j, k 부분에 해당하는 c1, ci, cj, ck 멤버를 가집니다.
	/// </summary>
	struct Quaternion {
		float c1 = 1, ci = 0, cj = 0, ck = 0;

		/// <summary>
		/// 사원수를 생성합니다.
		/// </summary>
		/// <param name="o">실수부분</param>
		/// <param name="i">i부분</param>
		/// <param name="j">j부분</param>
		/// <param name="k">k부분</param>
		Quaternion(float o = 1, float i = 0, float j = 0, float k = 0) :c1(o), ci(i), cj(j), ck(k) {};

		/// <summary>
		/// 사원수 크기의 제곱을 리턴합니다.
		/// </summary>
		inline float abs2() const { return c1 * c1 + ci * ci + cj * cj + ck * ck; }

		/// <summary>
		/// 사원수 크기를 리턴합니다.
		/// </summary
		inline float abs() const { return sqrtf(abs2()); }

		/// <summary>
		/// 켤레(공액)사원수를 리턴합니다.
		/// </summary>
		inline Quaternion conjugate() const { return Quaternion(c1, -ci, -cj, -ck); }

		/// <summary>
		/// 사원수의 우측에 곱하면 1이 되는 값을 리턴합니다.
		/// </summary>
		inline Quaternion inverse() const { return conjugate() / abs2(); }

		/// <summary>
		/// 사원수 곱셈 연산자입니다.
		/// </summary>
		inline Quaternion operator*(const Quaternion& q) const {
			return Quaternion(
				c1 * q.c1 - ci * q.ci - cj * q.cj - ck * q.ck,
				c1 * q.ci + ci * q.c1 + cj * q.ck - ck * q.cj,
				c1 * q.cj - ci * q.ck + cj * q.c1 + ck * q.ci,
				c1 * q.ck + ci * q.cj - cj * q.ci + ck * q.c1
			);
		}

		/// <summary>
		/// 사원수 간 사칙 연산입니다. 모든 연산은 이 사원수를 기준으로 우측에 적용됩니다.
		/// </summary>
		inline Quaternion& operator+=(const Quaternion& q) { c1 += q.c1; ci += q.ci; cj += q.cj; ck += q.ck; return *this; }
		inline Quaternion& operator-=(const Quaternion& q) { c1 -= q.c1; ci -= q.ci; cj -= q.cj; ck -= q.ck; return *this; }
		inline Quaternion& operator*=(const Quaternion& q) { *this = *this * q; return *this; }	// multiplying on the left is more commonly used operation
		inline Quaternion& operator/=(const Quaternion& q) { *this = *this * q.inverse(); return *this; }
		inline Quaternion operator*(float f) const { return Quaternion(c1 * f, ci * f, cj * f, ck * f); }
		inline Quaternion operator/(float f) const { return Quaternion(c1 / f, ci / f, cj / f, ck / f); }
		inline Quaternion operator+(const Quaternion& q) const { Quaternion r(*this); r += q; return r; }
		inline Quaternion operator-(const Quaternion& q) const { Quaternion r(*this); r -= q; return r; }
		inline Quaternion operator/(const Quaternion& q) const { Quaternion r(*this); r /= q; return r; }

		/// <summary>
		/// 사원수의 부호를 반대로 합니다. 180도 뒤집은 것과 동일합니다.
		/// </summary>
		inline Quaternion operator-() const { return Quaternion(-c1, -ci, -cj, -ck); }


		/// <summary>
		/// 사원수 회전을 합칩니다. 기존 사원수가 먼저 적용되며 크기 1임을 확인하지 않습니다.
		/// </summary>
		inline void compound(const Quaternion& q) { *this = q * (*this); }

		/// <summary>
		/// 사원수 회전을 합칩니다. 기존 사원수가 먼저 적용됩니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void compound(const vec3& axis, float angle) { auto q = rotation(axis, angle); compound(q); }

		/// <summary>
		/// 사원수를 회전 행렬로 변형합니다.
		/// </summary>
		inline mat4 toMat4() const {
			return mat4(
				1 - 2 * (cj * cj + ck * ck), 2 * (ci * cj - c1 * ck), 2 * (ci * ck + c1 * cj), 0,
				2 * (ci * cj + c1 * ck), 1 - 2 * (ci * ci + ck * ck), 2 * (cj * ck - c1 * ci), 0,
				2 * (ci * ck - c1 * cj), 2 * (cj * ck + c1 * ci), 1 - 2 * (ci * ci + cj * cj), 0,
				0, 0, 0, 1
			);
		}

		/// <summary>
		/// 축과 각이 주어졌을 때 회전값을 리턴합니다. 회전축은 정규화가 이루어집니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각(라디안)</param>
		inline static Quaternion rotation(const vec3& axis, float angle) {
			float c = cosf(angle / 2), s = sinf(angle / 2);
			auto nv = axis.normalize() * s;
			return Quaternion(c, nv.x, nv.y, nv.z);
		}

		/// <summary>
		/// 오일러 회전에 해당하는 사원수를 생성합니다. cf) 오른손 법칙
		/// <para>https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles</para>
		/// </summary>
		/// <param name="yaw">yaw(Z축 방향 회전)</param>
		/// <param name="pitch">pitch(Y축 방향 회전)</param>
		/// <param name="roll">roll(X축 방향 회전)</param>
		inline static Quaternion euler(float yaw, float pitch, float roll) {
			float cy = cosf(yaw / 2);	float sy = sinf(yaw / 2);
			float cp = cosf(pitch / 2);	float sp = sinf(pitch / 2);
			float cr = cosf(roll / 2);	float sr = sinf(roll / 2);
			
			return Quaternion(cr * cp * cy + sr * sp * sy, sr * cp * cy - cr * sp * sy, cr * sp * cy + sr * cp * sy, cr * cr * sy - sr * sp * cy);
		}
		
	};

	/// <summary>
	/// 사원수의 선형 보간을 리턴합니다.
	/// </summary>
	/// <param name="q1">선형 보간 대상 1(t=0에 가까울수록 이것에 가깝습니다.)</param>
	/// <param name="q2">선형 보간 대상 2(t=1에 가까울수록 이것에 가깝습니다.)</param>
	/// <param name="t">선형 보간 값</param>
	inline Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t) {
		Quaternion ret = q1 * (1 - t) + q2 * t;
		return ret / ret.abs();
	}

	/// <summary>
	/// 사원수의 구면 선형 보간을 리턴합니다.
	/// </summary>
	/// <param name="q1">선형 보간 대상 1(t=0에 가까울수록 이것에 가깝습니다.)</param>
	/// <param name="q2">선형 보간 대상 2(t=1에 가까울수록 이것에 가깝습니다.)</param>
	/// <param name="t">선형 보간 값</param>
	inline Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
		float Wa, Wb;
		float costh = (q1.c1 * q2.c1 + q1.ci * q2.ci + q1.cj * q2.cj + q1.ck * q2.ck) / q1.abs() / q2.abs();
		if (costh > 1) costh = 1;
		else if (costh < -1) costh = -1;
		float theta = acos(costh);
		float sn = sin(theta);

		if (sn <= FLT_EPSILON) return q1;
		Wa = sin((1 - t) * theta) / sn;
		Wb = sin(t * theta) / sn;

		Quaternion r = q1 * Wa + q2 * Wb;
		return r / r.abs();
	}

	inline mat3 mat3::rotate(float roll, float pitch, float yaw) { return mat3(mat4::rotate(roll, pitch, yaw)); }
	inline mat4 mat4::rotate(const vec3& axis, float angle) { return Quaternion::rotation(axis, angle).toMat4(); }
	inline mat4 mat4::rotate(float roll, float pitch, float yaw) { return Quaternion::euler(yaw, pitch, roll).toMat4(); }
	inline mat4 mat4::TRS(const vec3& translation, const Quaternion& rotation, const vec3& scale) {
		// 곱 36회/합 6회, T*R*S 따로 하는 경우 곱 155회/합 102회
		mat4 r = rotation.toMat4();
		r[0] *= scale.x;	r[1] *= scale.y;	r[2] *= scale.z;	r[3] = translation.x;
		r[4] *= scale.x;	r[5] *= scale.y;	r[6] *= scale.z;	r[4] = translation.y;
		r[8] *= scale.x;	r[9] *= scale.y;	r[10] *= scale.z;	r[11] = translation.z;
		return r;
	}

	/// <summary>
	/// 편리한 디버그를 위한 값 출력 함수입니다.
	/// </summary>
	/// <param name="v">표시할 변수</param>
	/// <param name="tag">이름</param>
	inline void print(const vec2& v, const char* tag = "", char end = '\n') { printf("%s: %f %f%c", tag, v.x, v.y, end); }
	inline void print(const ivec2& v, const char* tag = "", char end = '\n') { printf("%s: %d %d%c", tag, v.x, v.y, end); }
	inline void print(const uvec2& v, const char* tag = "", char end = '\n') { printf("%s: %u %u%c", tag, v.x, v.y, end); }
	inline void print(const dvec2& v, const char* tag = "", char end = '\n') { printf("%s: %f %f%c", tag, v.x, v.y, end); }
	inline void print(const vec3& v, const char* tag = "", char end = '\n') { printf("%s: %f %f %f%c", tag, v.x, v.y, v.z, end); }
	inline void print(const ivec3& v, const char* tag = "", char end = '\n') { printf("%s: %d %d %d%c", tag, v.x, v.y, v.z, end); }
	inline void print(const uvec3& v, const char* tag = "", char end = '\n') { printf("%s: %u %u %u%c", tag, v.x, v.y, v.z, end); }
	inline void print(const dvec3& v, const char* tag = "", char end = '\n') { printf("%s: %f %f %f%c", tag, v.x, v.y, v.z, end); }
	inline void print(const vec4& v, const char* tag = "", char end = '\n') { printf("%s: %f %f %f %f%c", tag, v.x, v.y, v.z, v.w, end); }
	inline void print(const ivec4& v, const char* tag = "", char end = '\n') { printf("%s: %d %d %d %d%c", tag, v.x, v.y, v.z, v.w, end); }
	inline void print(const uvec4& v, const char* tag = "", char end = '\n') { printf("%s: %u %u %u %u%c", tag, v.x, v.y, v.z, v.w, end); }
	inline void print(const dvec4& v, const char* tag = "", char end = '\n') { printf("%s: %f %f %f %f%c", tag, v.x, v.y, v.z, v.w, end); }
	inline void print(const Quaternion& q, const char* tag = "", char end = '\n') { printf("%s: %f + %fi + %fj + %fk%c", tag, q.c1, q.ci, q.cj, q.ck, end); }
	inline void print(const mat4& m, const char* tag = "") { printf("%s:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", tag, m._11, m._12, m._13, m._14, m._21, m._22, m._23, m._24, m._31, m._32, m._33, m._34, m._41, m._42, m._43, m._44); }
	
}

#endif // !__OAGLEM_H__
