#ifndef __OAGLEM_H__
#define __OAGLEM_H__

/*
* ���� �Լ�
* ����:
* nvec, mat2, mat3, mat4, Quaternion
* �ٸ� ���� ��ü�ϴ� ��:
*	namespace onart{
*		using mat4 =(�ٸ���� 4x4���);
*	}
*/

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cassert>

constexpr float PI = 3.14159265358979323846f;

/// <summary>
/// ������ �Է��ϸ� �� ������ �����մϴ�.
/// </summary>
constexpr float deg(float rad) { return rad * 180 / PI; }
/// <summary>
/// �� ������ �Է��ϸ� �������� �����մϴ�.
/// </summary>
constexpr float rad(float deg) { return deg / 180 * PI; }

namespace onart {
	struct Quaternion;
	/// <summary>
	/// N���� �����Դϴ�. ���̿� ������� ��ȣ ��ȯ�� �����մϴ�.
	/// </summary>
	/// <typeparam name="T">���� ������ Ÿ���Դϴ�. ��Ģ ���� �� ��ȣ ������ �����Ͽ��� �մϴ�.</typeparam>
	template <unsigned D, class T = float> union nvec {
		T entry[D];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };

		/// <summary>
		/// �����͸� �����մϴ�.
		/// </summary>
		nvec() { for (int i = 0; i < D; i++) entry[i] = 0; }

		/// <summary>
		/// ������ ��� ���� �ϳ��� ������ �ʱ�ȭ�մϴ�.
		/// </summary>
		nvec(T a) { for (int i = 0; i < D; i++) entry[i] = a; }

		/// <summary>
		/// ������ �� �� �� 2~4���� �ʱ�ȭ�մϴ�.
		/// </summary>
		nvec(T x, T y, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {  }

		/// <summary>
		/// ���� �������Դϴ�.
		/// </summary>
		nvec(const nvec& v) { for (int i = 0; i < D; i++) entry[i] = v.entry[i]; }

		/// <summary>
		/// �ٸ� ������ ���͸� ����ϴ� ���� �������Դϴ�. ������ ���� ��ҿ��� ����ϴ� ���� �����ϴ�.
		/// </summary>
		template <unsigned E> nvec(const nvec<E, T>& v) { unsigned min = D > E ? E : D; int i = 0; for (; i < min; i++) entry[i] = v.entry[i]; for (; i < D; i++) entry[i] = 0; }

		/// <summary>
		/// ������ ��� ������ �ϳ��� ������ �ʱ�ȭ�մϴ�. operator=�� �����մϴ�.
		/// </summary>
		inline void set(T a) { for (int i = 0; i < D; i++) entry[i] = a; }
		
		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. operator=�� �����մϴ�.
		/// </summary>
		template <unsigned E> inline void set(const nvec<E, T>& v) {
			unsigned min = D > E ? E : D;
			for (unsigned i = 0; i < min; i++) {
				entry[i] = v.entry[i];
			}
		}

		/// <summary>
		/// ������ ��� ������ �ϳ��� ������ �ʱ�ȭ�մϴ�. set()�� �����մϴ�.
		/// </summary>
		inline nvec& operator=(T a) { set(a); return *this; }

		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. set()�� �����մϴ�.
		/// </summary>
		template <unsigned E> inline nvec& operator=(const nvec<E, T>& v) { set(v); return *this; }

		/// <summary>
		/// �ٸ� ���Ϳ� ���к� ������ �������� ���� ���� �������� �մϴ�.
		/// </summary>
		template <unsigned E> inline nvec& operator+=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] += v[i]; }
		template <unsigned E> inline nvec& operator-=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] -= v[i]; }
		template <unsigned E> inline nvec& operator*=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] *= v[i]; }
		template <unsigned E> inline nvec& operator/=(const nvec<E, T>& v) { unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] /= v[i]; }

		/// <summary>
		/// ������ ��� ���п� ���Ͽ� �־��� ���� �����մϴ�.
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
		/// ������ ��� ������ ������ ��� ���� �����մϴ�. �ٸ� ũ���� ���Ϳ��� �񱳸� �������� �ʽ��ϴ�.
		/// </summary>
		inline bool operator==(const nvec& v) const { for (int i = 0; i < D; i++)if (entry[i] != v[i]) return false; return true; }
		inline bool operator!=(const nvec& v) const { for (int i = 0; i < D; i++)if (entry[i] != v[i]) return true; return false; }

		/// <summary>
		/// �ٸ� ���Ϳ� ���к� ������ �������� ���� ���� �������� �մϴ�. ���� �������� �׻� �º����� �����մϴ�.
		/// </summary>
		template <unsigned E> inline nvec operator+(const nvec<E, T>& v) const { auto r(*this); r += v; return r; }
		template <unsigned E> inline nvec operator-(const nvec<E, T>& v) const { auto r(*this); r -= v; return r; }
		template <unsigned E> inline nvec operator*(const nvec<E, T>& v) const { auto r(*this); r *= v; return r; }
		template <unsigned E> inline nvec operator/(const nvec<E, T>& v) const { auto r(*this); r /= v; return r; }

		/// <summary>
		/// T�� �迭�� ����� �� �ֵ��� �����͸� �����մϴ�.
		/// </summary>
		inline operator T* () { return &x; }
		inline operator const T* () const { return &x; }

		/// <summary>
		/// ����� ĳ��Ʈ�� ������ Ÿ���̶�� ���͵� ��������� ĳ��Ʈ�� �����մϴ�.
		/// ���к��� ĳ��Ʈ�ؼ� ���� ����� �Ͱ� ���Ͽ� Ư���� ������ �鿡�� ���� �κ��� ������
		/// ������ �ڵ常�� ���� �߰��Ͽ����ϴ�.
		/// </summary>
		template <class T2> inline operator nvec<D, T2>() { nvec<D, T2> n; for (unsigned i = 0; i < D; i++) { n[i] = (T2)entry[i]; } return n; }

		/// <summary>
		/// ��ȣ�� �������� �����մϴ�.
		/// </summary>
		inline nvec operator-() const { return (*this) * -1; }

		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline T& operator[](ptrdiff_t i) { assert(i < D); return entry[i]; }
		inline const T& operator[](ptrdiff_t i) const { assert(i < D); return entry[i]; }

		/// <summary>
		/// ������ ������ �����ϰ� ���̸� 1�� ����ϴ�. ���� ���Ϳ����� ����� �� �����ϴ�.
		/// </summary>
		inline nvec normalize() const { return (*this) / length(); }

		/// <summary>
		/// ���� ������ ������ �����մϴ�.
		/// </summary>
		inline float length2() const { float s = 0; for (int i = 0; i < D; i++) s += entry[i] * entry[i]; return s; }

		/// <summary>
		/// ���� ���̸� �����մϴ�.
		/// </summary>
		inline float length() const { return sqrtf(length2()); }

		/// <summary>
		/// �ٸ� ���Ϳ��� �Ÿ� ������ �����մϴ�.
		/// </summary>
		inline float distance2(const nvec& v) const { return (*this - v).length2(); }

		/// <summary>
		/// �ٸ� ���Ϳ��� �Ÿ��� �����մϴ�.
		/// </summary>
		inline float distance(const nvec& v) const { return sqrtf(distance(v)); }

		/// <summary>
		/// �ٸ� ���Ϳ��� ������ �����մϴ�. �ٸ� �������� ������ �������� �ʽ��ϴ�.
		/// </summary>
		inline T dot(const nvec& v) const { T s = 0; for (int i = 0; i < D; i++)s += entry[i] * v[i]; return s; }
	};

	using vec2 = nvec<2>;					using vec3 = nvec<3>;					using vec4 = nvec<4>;
	using ivec2 = nvec<2, int>;				using ivec3 = nvec<3, int>;				using ivec4 = nvec<4, int>;
	using uvec2 = nvec<2, unsigned>;		using uvec3 = nvec<3, unsigned>;		using uvec4 = nvec<4, unsigned>;
	using dvec2 = nvec<2, double>;			using dvec3 = nvec<3, double>;			using dvec4 = nvec<4, double>;

	/// <summary>
	/// 2�� 3���� �Ǽ� ������ ������ ����մϴ�.
	/// </summary>
	inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	/// <summary>
	/// 2���� ���͸� ���� �����մϴ�.
	/// </summary>
	/// <param name="a">���� ���� ��� 1(t=0�� �������� �� ���Ϳ� �������ϴ�.)</param>
	/// <param name="b">���� ���� ��� 2(t=1�� �������� �� ���Ϳ� �������ϴ�.)</param>
	/// <param name="t">���� ���� ��</param>	
	template <unsigned D, class T> inline nvec<D, T> lerp(const nvec<D, T>& a, const nvec<D, T>& b, const nvec<D, float>& t) { return a * (1 - t) + b * t; }

	/// <summary>
	/// 2���� �̹����� ȸ�������� ���� 2x2 ����Դϴ�. ��, 3���� ������ z���� 0���� �����ϴ� ���� �� �Ϲ����� ����Դϴ�.
	/// </summary>
	struct mat2 {
		union { float a[4]; struct { float _11, _12, _21, _22; }; };

		/// <summary>
		/// ��������� �����մϴ�.
		/// </summary>
		inline mat2() { _11 = _22 = 1; _12 = _21 = 0; }

		/// <summary>
		/// �� �켱 ������ �Ű������� �־� ����� �����մϴ�.
		/// </summary>
		inline mat2(float _11, float _12, float _21, float _22) :_11(_11), _12(_12), _21(_21), _22(_22) { }
		
		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// ���� ����
		/// </summary>
		inline mat2(const mat2& m) { for (int i = 0; i < 4; i++) { a[i] = m[i]; } }

		/// <summary>
		/// ����� ������ķ� �ٲߴϴ�.
		/// </summary>
		inline void toI() { _11 = _22 = 1; _12 = _21 = 0; }

		/// <summary>
		/// �ٸ� ��İ� ���к��� ���ϰų� ���ϴ�.
		/// </summary>
		inline mat2& operator+=(const mat2& m) { for (int i = 0; i < 4; i++)a[i] += m[i]; return *this; }
		inline mat2& operator-=(const mat2& m) { for (int i = 0; i < 4; i++)a[i] -= m[i]; return *this; }
		inline mat2 operator+(const mat2& m) const { auto r = mat2(*this); r += m; return r; }
		inline mat2 operator-(const mat2& m) const { auto r = mat2(*this); r -= m; return r; }

		/// <summary>
		/// ��İ��� �����մϴ�.
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
		/// ���Ϳ� ������ȯ�� �����Ͽ� �����մϴ�.
		/// </summary>
		inline vec2 operator*(const vec2& v) const { return vec2(_11 * v.x + _12 * v.y, _21 * v.x + _22 * v.y); }

		/// <summary>
		/// ��Ŀ� �Ǽ��踦 �մϴ�.
		/// </summary>
		inline mat2& operator*=(float f) { for (int i = 0; i < 4; i++) a[i] *= f; }
		inline mat2 operator*(float f) const { mat2 r(*this); r *= f; return r; }
		inline mat2& operator/=(float f) { for (int i = 0; i < 4; i++) a[i] /= f; }
		inline mat2 operator/(float f) const { mat2 r(*this); r /= f; return r; }

		/// <summary>
		/// ��Ľ��� ��ȯ�մϴ�.
		/// </summary>
		inline float det() const { return _11 * _22 - _12 * _21; }

		/// <summary>
		/// ������� ��ȯ�մϴ�.
		/// </summary>
		inline mat2 inverse() const {
			float d = det();
			if (d == 0) printf("%p: �� ����� ������� ���ų� �ſ� ū ������ �����ϴ�. NaN�� ���� ����ġ ���� ������ �߻��� �� �ֽ��ϴ�.\n", this);
			return mat2(_22, -_12, -_21, _11) / d;
		}

		/// <summary>
		/// ��ġ ����� ��ȯ�մϴ�.
		/// </summary>
		inline mat2 transpose() const { return mat2(_11, _21, _12, _22); }

		/// <summary>
		/// �� �켱 ������ �� �迭�� �����մϴ�.
		/// </summary>
		inline operator float* () { return a; }
	};

	/// <summary>
	/// 3���� ���� ȸ������ Ȥ�� 2���� �̹����� ���� ��ȯ�� ���� 3x3 ����Դϴ�.
	/// </summary>
	struct mat3 {
		union { float a[9]; struct { float _11, _12, _13, _21, _22, _23, _31, _32, _33; }; };

		/// <summary>
		/// ��������� �����մϴ�.
		/// </summary>
		inline mat3() { _11 = _22 = _33 = 1; _12 = _13 = _21 = _23 = _31 = _32 = 0; }

		/// <summary>
		/// �� �켱 ������ �Ű������� �־� ����� �����մϴ�.
		/// </summary>
		inline mat3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33) :_11(_11), _12(_12), _13(_13), _21(_21), _22(_22), _23(_23), _31(_31), _32(_32), _33(_33) { }

		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// ���� ����
		/// </summary>
		inline mat3(const mat3& m) { for (int i = 0; i < 9; i++) { a[i] = m[i]; } }

		/// <summary>
		/// ����� ������ķ� �ٲߴϴ�.
		/// </summary>
		inline void toI() { _11 = _22 = _33 = 1; _12 = _13 = _21 = _23 = _31 = _32 = 0; }

		/// <summary>
		/// �ٸ� ��İ� ���к��� ���ϰų� ���ϴ�.
		/// </summary>
		inline mat3& operator+=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] += m[i]; return *this; }
		inline mat3& operator-=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] -= m[i]; return *this; }
		inline mat3 operator+(const mat3& m) const { auto r = mat3(*this); r += m; return r; }
		inline mat3 operator-(const mat3& m) const { auto r = mat3(*this); r -= m; return r; }

		/// <summary>
		/// ��İ��� �����մϴ�.
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
		/// ���Ϳ� ������ȯ�� �����Ͽ� �����մϴ�.
		/// </summary>
		inline vec3 operator*(const vec3& v) const { return vec3(_11 * v.x + _12 * v.y + _13 * v.z, _21 * v.x + _22 * v.y + _23 * v.z, _31 * v.x + _32 * v.y + _33 * v.z); }

		/// <summary>
		/// ��Ŀ� �Ǽ��踦 �մϴ�.
		/// </summary>
		inline mat3& operator*=(float f) { for (int i = 0; i < 9; i++) a[i] *= f; }
		inline mat3 operator*(float f) const { mat3 r(*this); r *= f; return r; }
		inline mat3& operator/=(float f) { for (int i = 0; i < 9; i++) a[i] /= f; }
		inline mat3 operator/(float f) const { mat3 r(*this); r /= f; return r; }

		/// <summary>
		/// ��Ľ��� ��ȯ�մϴ�.
		/// </summary>
		inline float det() const { return _11 * (_22 * _33 - _23 * _32) + _12 * (_23 * _31 - _21 * _33) + _13 * (_21 * _32 - _22 * _31); }

		/// <summary>
		/// ������� ��ȯ�մϴ�.
		/// </summary>
		inline mat3 inverse() const {
			float d = det();
			if (d == 0) printf("%p: �� ����� ������� ���ų� �ſ� ū ������ �����ϴ�. NaN�� ���� ����ġ ���� ������ �߻��� �� �ֽ��ϴ�.\n", this);
			return mat3((_22 * _33 - _32 * _23), (_13 * _32 - _12 * _33), (_12 * _23 - _13 * _22), (_23 * _31 - _21 * _33), (_11 * _33 - _13 * _31), (_21 * _13 - _11 * _23), (_21 * _32 - _31 * _22), (_31 * _12 - _11 * _32), (_11 * _22 - _21 * _12)) / d;
		}

		/// <summary>
		/// ��ġ ����� ��ȯ�մϴ�.
		/// </summary>
		inline mat3 transpose() const { return mat3(_11, _21, _31, _12, _22, _32, _13, _23, _33); }

		/// <summary>
		/// �� �켱 ������ �� �迭�� �����մϴ�.
		/// </summary>
		inline operator float* () { return a; }

		/// <summary>
		/// ���� ��� 2x2 ��ķ� ĳ��Ʈ�մϴ�.
		/// </summary>
		inline operator mat2() { return mat2(_11, _12, _21, _22); }

		/// <summary>
		/// 2���� ���� ����� ����մϴ�.
		/// </summary>
		inline static mat3 translate(const vec2& t) {
			return mat3(
				1, 0, t.x,
				0, 1, t.y,
				0, 0, 1
			);
		}
		/// <summary>
		/// 2���� ���� ����� ����մϴ�.
		/// </summary>
		inline static mat3 translate(float x, float y) {
			return mat3(
				1, 0, x,
				0, 1, y,
				0, 0, 1
			);
		}

		/// <summary>
		/// 2���� ũ�� ��ȯ ����� ����մϴ�.
		/// </summary>
		inline static mat3 scale(const vec2& t) {
			return mat3(
				t.x, 0, 0,
				0, t.y, 0,
				0, 0, 1
			);
		}
		/// <summary>
		/// 2���� ũ�� ��ȯ ����� ����մϴ�.
		/// </summary>
		inline static mat3 scale(float x, float y) {
			return mat3(
				x, 0, 0,
				0, y, 0,
				0, 0, 1
			);
		}

		/// <summary>
		/// Z�� ������ 2���� ȸ���� �����մϴ�. X, Y �� ������ ���ϴ� ��� 3x3 ���� ��ȯ�� �ƴ� 4x4 ���� ��ȯ�� ����ؾ� �մϴ�.
		/// </summary>
		inline static mat3 rotate(float z) {
			return mat3(
				cosf(z), -sinf(z), 0,
				sinf(z), cosf(z), 0,
				0, 0, 1
			);
		}

		/// <summary>
		/// Z�� ������ 3���� ȸ���� �����մϴ�.
		/// </summary>
		/// <param name="roll">roll(X�� ���� ȸ��)</param>
		/// <param name="pitch">pitch(Y�� ���� ȸ��)</param>
		/// <param name="yaw">yaw(Z�� ���� ȸ��)</param>
		inline static mat3 rotate(float roll, float pitch, float yaw);
	};

	/// <summary>
	/// 3���� ���� ���� ��ȯ�� ���� 4x4 ����Դϴ�.
	/// </summary>
	struct mat4 {
		union { float a[16]; struct { float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; }; };

		/// <summary>
		/// ��������� �����մϴ�.
		/// </summary>
		inline mat4() { _11 = _22 = _33 = _44 = 1; _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0; }

		/// <summary>
		/// �� �켱 ������ �Ű������� �־� ����� �����մϴ�.
		/// </summary>
		inline mat4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44) :_11(_11), _12(_12), _13(_13), _14(_14), _21(_21), _22(_22), _23(_23), _24(_24), _31(_31), _32(_32), _33(_33), _34(_34), _41(_41), _42(_42), _43(_43), _44(_44) { }

		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// ���� ����
		/// </summary>
		inline mat4(const mat4& m) { for (int i = 0; i < 16; i++) { a[i] = m[i]; } }

		/// <summary>
		/// ����� ������ķ� �ٲߴϴ�.
		/// </summary>
		inline void toI() { _11 = _22 = _33 = _44 = 1; _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0; }

		/// <summary>
		/// �ٸ� ��İ� ���к��� ���ϰų� ���ϴ�.
		/// </summary>
		inline mat4& operator+=(const mat4& m) { for (int i = 0; i < 16; i++)a[i] += m[i]; return *this; }
		inline mat4& operator-=(const mat4& m) { for (int i = 0; i < 16; i++)a[i] -= m[i]; return *this; }
		inline mat4 operator+(const mat4& m) const { auto r = mat4(*this); r += m; return r; }
		inline mat4 operator-(const mat4& m) const { auto r = mat4(*this); r -= m; return r; }

		/// <summary>
		/// ��İ��� �����մϴ�.
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
		/// ���Ϳ� ������ȯ�� �����Ͽ� �����մϴ�.
		/// </summary>
		inline vec4 operator*(const vec4& v) const { return vec4(_11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w, _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w, _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w, _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w); }

		/// <summary>
		/// ��Ŀ� �Ǽ��踦 �մϴ�.
		/// </summary>
		inline mat4& operator*=(float f) { for (int i = 0; i < 16; i++) a[i] *= f; }
		inline mat4 operator*(float f) const { mat4 r(*this); r *= f; return r; }
		inline mat4& operator/=(float f) { for (int i = 0; i < 16; i++) a[i] /= f; }
		inline mat4 operator/(float f) const { mat4 r(*this); r /= f; return r; }

		/// <summary>
		/// ��Ľ��� ��ȯ�մϴ�.
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
		/// ������� ��ȯ�մϴ�.
		/// </summary>
		inline mat4 inverse() const {
			float d = det();
			if (d == 0) printf("%p: �� ����� ������� ���ų� �ſ� ū ������ �����ϴ�. NaN�� ���� ����ġ ���� ������ �߻��� �� �ֽ��ϴ�.\n", this);
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
		/// ��ġ ����� ��ȯ�մϴ�.
		/// </summary>
		inline mat4 transpose() const { return mat4(_11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44); }

		/// <summary>
		/// �� �켱 ������ �� �迭�� �����մϴ�.
		/// </summary>
		inline operator float* () { return a; }
		inline operator const float* () const { return a; }

		/// <summary>
		/// ���� ��� 3x3 ��ķ� ĳ��Ʈ�մϴ�.
		/// </summary>
		inline operator mat3() { return mat3(_11, _12, _13, _21, _22, _23, _31, _32, _33); }

		/// <summary>
		/// 3���� ���� ����� ����մϴ�.
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
		/// 3���� ���� ����� ����մϴ�.
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
		/// 3���� ũ�� ��ȯ ����� ����մϴ�.
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
		/// 3���� ũ�� ��ȯ ����� ����մϴ�.
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
		/// 3���� ȸ�� ����� ����մϴ�.
		/// </summary>
		inline static mat4 rotate(const vec3& axis, float angle);

		/// <summary>
		/// 3���� ȸ�� ����� ����մϴ�. cf) ������ ��Ģ
		/// </summary>
		/// <param name="roll">roll(X�� ���� ȸ��)</param>
		/// <param name="pitch">pitch(Y�� ���� ȸ��)</param>
		/// <param name="yaw">yaw(Z�� ���� ȸ��)</param>
		inline static mat4 rotate(float roll, float pitch, float yaw);

		/// <summary>
		/// lookAt ������ �� ����� ����մϴ�.
		/// </summary>
		/// <param name="eye">���� ��ġ</param>
		/// <param name="at">�ǻ�ü ��ġ</param>
		/// <param name="up">���� ����: ȭ�� �󿡼� ���� ������ �� ������ ����� ��������ϴ�.</param>
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
		/// ����, ȸ��, ���� ��� T, R, S�� ���� �� ���ϴ� �ͺ��� ���� �� ������ ����մϴ�.
		/// </summary>
		/// <param name="translation">����</param>
		/// <param name="rotation">ȸ��</param>
		/// <param name="scale">����</param>
		inline static mat4 TRS(const vec3& translation, const Quaternion& rotation, const vec3& scale);
		/// <summary>
		/// ǥ�� �� ���� ������ü�� ���� ��� �Դ�(����ü)�� �����ϴ� ���� ����� ����մϴ�.
		/// ���� 2D ������ ����� ���, ���� ��Ŀ� aspect�� �����ϸ� �˴ϴ�.
		/// </summary>
		/// <param name="fovy">field of view Y: �Դ��� Y�� ����(ȭ�� ���� ����) ���� �����Դϴ�.</param>
		/// <param name="aspect">ǥ�� ����Ʈ ����(����/����)�Դϴ�.</param>
		/// <param name="dnear">�Դ뿡�� ���� ����� �Ÿ��Դϴ�. �� ������ �Ÿ��� ���� ������ �ڿ� �ִ� ������ Ĩ�ϴ�.</param>
		/// <param name="dfar">�Դ뿡�� ���� �� �Ÿ��Դϴ�. �� �̻��� �Ÿ��� ������ �ʽ��ϴ�.</param>
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
	/// 3���� ȸ�� ���� ǥ���ϴ� ������Դϴ�. 1, i, j, k �κп� �ش��ϴ� c1, ci, cj, ck ����� �����ϴ�.
	/// </summary>
	struct Quaternion {
		float c1 = 1, ci = 0, cj = 0, ck = 0;

		/// <summary>
		/// ������� �����մϴ�.
		/// </summary>
		/// <param name="o">�Ǽ��κ�</param>
		/// <param name="i">i�κ�</param>
		/// <param name="j">j�κ�</param>
		/// <param name="k">k�κ�</param>
		Quaternion(float o = 1, float i = 0, float j = 0, float k = 0) :c1(o), ci(i), cj(j), ck(k) {};

		/// <summary>
		/// ����� ũ���� ������ �����մϴ�.
		/// </summary>
		inline float abs2() const { return c1 * c1 + ci * ci + cj * cj + ck * ck; }

		/// <summary>
		/// ����� ũ�⸦ �����մϴ�.
		/// </summary
		inline float abs() const { return sqrtf(abs2()); }

		/// <summary>
		/// �ӷ�(����)������� �����մϴ�.
		/// </summary>
		inline Quaternion conjugate() const { return Quaternion(c1, -ci, -cj, -ck); }

		/// <summary>
		/// ������� ������ ���ϸ� 1�� �Ǵ� ���� �����մϴ�.
		/// </summary>
		inline Quaternion inverse() const { return conjugate() / abs2(); }

		/// <summary>
		/// ����� ���� �������Դϴ�.
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
		/// ����� �� ��Ģ �����Դϴ�. ��� ������ �� ������� �������� ������ ����˴ϴ�.
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
		/// ������� ��ȣ�� �ݴ�� �մϴ�. 180�� ������ �Ͱ� �����մϴ�.
		/// </summary>
		inline Quaternion operator-() const { return Quaternion(-c1, -ci, -cj, -ck); }


		/// <summary>
		/// ����� ȸ���� ��Ĩ�ϴ�. ���� ������� ���� ����Ǹ� ũ�� 1���� Ȯ������ �ʽ��ϴ�.
		/// </summary>
		inline void compound(const Quaternion& q) { *this = q * (*this); }

		/// <summary>
		/// ����� ȸ���� ��Ĩ�ϴ�. ���� ������� ���� ����˴ϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void compound(const vec3& axis, float angle) { auto q = rotation(axis, angle); compound(q); }

		/// <summary>
		/// ������� ȸ�� ��ķ� �����մϴ�.
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
		/// ��� ���� �־����� �� ȸ������ �����մϴ�. ȸ������ ����ȭ�� �̷�����ϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����(����)</param>
		inline static Quaternion rotation(const vec3& axis, float angle) {
			float c = cosf(angle / 2), s = sinf(angle / 2);
			auto nv = axis.normalize() * s;
			return Quaternion(c, nv.x, nv.y, nv.z);
		}

		/// <summary>
		/// ���Ϸ� ȸ���� �ش��ϴ� ������� �����մϴ�. cf) ������ ��Ģ
		/// <para>https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles</para>
		/// </summary>
		/// <param name="yaw">yaw(Z�� ���� ȸ��)</param>
		/// <param name="pitch">pitch(Y�� ���� ȸ��)</param>
		/// <param name="roll">roll(X�� ���� ȸ��)</param>
		inline static Quaternion euler(float yaw, float pitch, float roll) {
			float cy = cosf(yaw / 2);	float sy = sinf(yaw / 2);
			float cp = cosf(pitch / 2);	float sp = sinf(pitch / 2);
			float cr = cosf(roll / 2);	float sr = sinf(roll / 2);
			
			return Quaternion(cr * cp * cy + sr * sp * sy, sr * cp * cy - cr * sp * sy, cr * sp * cy + sr * cp * sy, cr * cr * sy - sr * sp * cy);
		}
		
	};

	/// <summary>
	/// ������� ���� ������ �����մϴ�.
	/// </summary>
	/// <param name="q1">���� ���� ��� 1(t=0�� �������� �̰Ϳ� �������ϴ�.)</param>
	/// <param name="q2">���� ���� ��� 2(t=1�� �������� �̰Ϳ� �������ϴ�.)</param>
	/// <param name="t">���� ���� ��</param>
	inline Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t) {
		Quaternion ret = q1 * (1 - t) + q2 * t;
		return ret / ret.abs();
	}

	/// <summary>
	/// ������� ���� ���� ������ �����մϴ�.
	/// </summary>
	/// <param name="q1">���� ���� ��� 1(t=0�� �������� �̰Ϳ� �������ϴ�.)</param>
	/// <param name="q2">���� ���� ��� 2(t=1�� �������� �̰Ϳ� �������ϴ�.)</param>
	/// <param name="t">���� ���� ��</param>
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
		// �� 36ȸ/�� 6ȸ, T*R*S ���� �ϴ� ��� �� 155ȸ/�� 102ȸ
		mat4 r = rotation.toMat4();
		r[0] *= scale.x;	r[1] *= scale.y;	r[2] *= scale.z;	r[3] = translation.x;
		r[4] *= scale.x;	r[5] *= scale.y;	r[6] *= scale.z;	r[4] = translation.y;
		r[8] *= scale.x;	r[9] *= scale.y;	r[10] *= scale.z;	r[11] = translation.z;
		return r;
	}

	/// <summary>
	/// ���� ����׸� ���� �� ��� �Լ��Դϴ�.
	/// </summary>
	/// <param name="v">ǥ���� ����</param>
	/// <param name="tag">�̸�</param>
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
