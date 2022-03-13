/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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
* ����: mat4�� ��� �� ��⿡���� �� �켱 �����Դϴ�. �� uniform ���� �� ��ġ�� �����ϵ��� �����ϴµ�, ���� ���� �ٸ� mat4�� ����ϴ� ���
* shader �ڵ忡�� mat4�� ���� �� ��ġ�� �������� �ʵ��� �ؾ� �մϴ�.
*/

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "oagle_simd.h"

#pragma warning(disable: 6294 6201)

constexpr float PI = 3.14159265358979323846f;
constexpr float INF = INFINITY;
constexpr float _NAN = NAN;

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
		struct { T left, down, width, height; };
#ifdef OAGELM_USE_GLSL_LIKE
		struct { T xy[2]; T zw[2]; };
		struct { T xyz[3]; };
		struct { T _x; T yz[2]; };
		struct { T __x; T yzw[3]; };
#endif
		/// <summary>
		/// �����͸� �����մϴ�.
		/// </summary>
		inline nvec() { memset(entry, 0, sizeof(entry)); }

		/// <summary>
		/// ������ ��� ���� �ϳ��� ������ �ʱ�ȭ�մϴ�.
		/// </summary>
		inline nvec(T a) { setAll(entry, a, D > 4 ? D : 4); }

		/// <summary>
		/// ������ �� �� �� 2~4���� �ʱ�ȭ�մϴ�.
		/// </summary>
		inline nvec(T x, T y, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {  }

		/// <summary>
		/// ���� �������Դϴ�.
		/// </summary>
		inline nvec(const nvec<D,T>& v) { memcpy(entry, v.entry, sizeof(entry)); }

		/// <summary>
		/// �� ���� ���� ���͸� �̿��Ͽ� �����մϴ�.
		/// </summary>
		/// <param name="v">�� ���� ���� ����</param>
		/// <param name="a">������ �ڸ��� ���� ��</param>
		inline nvec(const nvec<D - 1, T>& v, T a) { memcpy(entry, v.entry, sizeof(entry) - sizeof(T)); entry[D - 1] = a; }

		/// <summary>
		/// �ٸ� ������ ���͸� ����ϴ� ���� �������Դϴ�. ������ ���� ��ҿ��� ����ϴ� ���� �����ϴ�.
		/// </summary>
		template <unsigned E> inline nvec(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; memcpy(entry, v.entry, min * sizeof(T)); }

		/// <summary>
		/// ������ ��� ������ �ϳ��� ������ �ʱ�ȭ�մϴ�. operator=�� �����մϴ�.
		/// </summary>
		inline void set(T a) { setAll(entry, a, D > 4 ? D : 4); }
		
		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. operator=�� �����մϴ�.
		/// </summary>
		/// <param name="v"></param>
		inline void set(const nvec& v) { memcpy(entry, v.entry, sizeof(entry));; }

		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. operator=�� �����մϴ�.
		/// </summary>
		template <unsigned E> inline void set(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; memcpy(entry, v.entry, min * sizeof(T)); }

		/// <summary>
		/// ������ ��� ������ �ϳ��� ������ �ʱ�ȭ�մϴ�. set()�� �����մϴ�.
		/// </summary>
		inline nvec& operator=(T a) { set(a); return *this; }

		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. set()�� �����մϴ�.
		/// </summary>
		inline nvec& operator=(const nvec& v) { set(v); return *this; }

		/// <summary>
		/// �ٸ� ������ ���� ������ �ɴϴ�. set()�� �����մϴ�.
		/// </summary>
		template <unsigned E> inline nvec& operator=(const nvec<E, T>& v) { set(v); return *this; }

		/// <summary>
		/// �ٸ� ���Ϳ� ���к� ������ �������� ���� ���� �������� �մϴ�.
		/// </summary>
		template <unsigned E> inline nvec& operator+=(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] += v[i]; return *this; }
		template <unsigned E> inline nvec& operator-=(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] -= v[i]; return *this; }
		template <unsigned E> inline nvec& operator*=(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] *= v[i]; return *this; }
		template <unsigned E> inline nvec& operator/=(const nvec<E, T>& v) { constexpr unsigned min = D > E ? E : D; for (unsigned i = 0; i < min; i++)entry[i] /= v[i]; return *this; }

		/// <summary>
		/// ������ ��� ���п� ���Ͽ� �־��� ���� �����մϴ�.
		/// </summary>
		inline nvec& operator+=(T a) { add4<T>(entry, a); for (unsigned i = 4; i < D; i++) entry[i] += a; return *this; }
		inline nvec& operator-=(T a) { sub4<T>(entry, a); for (unsigned i = 4; i < D; i++) entry[i] -= a; return *this; }
		inline nvec& operator*=(T a) { mul4<T>(entry, a); for (unsigned i = 4; i < D; i++) entry[i] *= a; return *this; }
		inline nvec& operator/=(T a) { div4<T>(entry, a); for (unsigned i = 4; i < D; i++) entry[i] /= a; return *this; }
		inline nvec operator+(T a) const { auto r(*this); r += a; return r; }
		inline nvec operator-(T a) const { auto r(*this); r -= a; return r; }
		inline nvec operator*(T a) const { auto r(*this); r *= a; return r; }
		inline nvec operator/(T a) const { auto r(*this); r /= a; return r; }

		/// <summary>
		/// ������ ��� ������ ������ ��� ���� �����մϴ�. �ٸ� ũ���� ���Ϳ��� �񱳸� �������� �ʽ��ϴ�.
		/// </summary>
		inline bool operator==(const nvec& v) const { return memcmp(entry, v.entry, sizeof(nvec)) == 0; }
		inline bool operator!=(const nvec& v) const { return !operator==(v); }

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
		template <class T2> inline operator nvec<D, T2>() const { nvec<D, T2> n; for (unsigned i = 0; i < D; i++) { n[i] = (T2)entry[i]; } return n; }

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
		/// �ٸ� ���Ϳ��� ������ �����մϴ�. �ٸ� �������� ������ �������� �ʽ��ϴ�.
		/// </summary>
		inline T dot(const nvec& v) const { auto nv = (*this) * v; T s = 0; for (unsigned i = 0; i < D; i++)s += nv[i]; return s; }

		/// <summary>
		/// ���� ������ ������ �����մϴ�.
		/// </summary>
		inline float length2() const { return dot(*this); }

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
	template <unsigned D, class T> inline nvec<D, T> lerp(const nvec<D, T>& a, const nvec<D, T>& b, const nvec<D>& t) { return a * (1 - t) + b * t; }

	/// <summary>
	/// 2���� ���͸� ���� �����մϴ�.
	/// </summary>
	/// <param name="a">���� ���� ��� 1(t=0�� �������� �� ���Ϳ� �������ϴ�.)</param>
	/// <param name="b">���� ���� ��� 2(t=1�� �������� �� ���Ϳ� �������ϴ�.)</param>
	/// <param name="t">���� ���� ��</param>
	template <unsigned D, class T> inline nvec<D, T> lerp(const nvec<D, T>& a, const nvec<D, T>& b, float t) { return a * (1 - t) + b * t; }

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
		inline mat3() { memset(a, 0, sizeof(a)); _11 = _22 = _33 = 1; }

		/// <summary>
		/// �� �켱 ������ �Ű������� �־� ����� �����մϴ�.
		/// </summary>
		inline mat3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33) :_11(_11), _12(_12), _13(_13), _21(_21), _22(_22), _23(_23), _31(_31), _32(_32), _33(_33) { }

		/// <summary>
		/// ����� ������ �����ؼ� �����մϴ�.
		/// </summary>
		inline mat3(const mat3& m) { memcpy(a, m.a, sizeof(a)); }

		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// ����� ������ķ� �ٲߴϴ�.
		/// </summary>
		inline void toI() { memset(a, 0, sizeof(a)); _11 = _22 = _33 = 1; }

		/// <summary>
		/// �ٸ� ��İ� ���к��� ���ϰų� ���ϴ�.
		/// </summary>
		inline mat3& operator+=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] += m[i]; return *this; }
		inline mat3& operator-=(const mat3& m) { for (int i = 0; i < 9; i++)a[i] -= m[i]; return *this; }
		inline mat3 operator+(const mat3& m) const { auto r = mat3(*this); r += m; return r; }
		inline mat3 operator-(const mat3& m) const { auto r = mat3(*this); r -= m; return r; }

		/// <summary>
		/// n�� ���͸� �����մϴ�. 1~3�� �Է� �����մϴ�.
		/// </summary>
		/// <param name="i">�� �ε���(1 base)</param>
		inline vec3 row(int i) const { assert(i <= 3 && i >= 1); int st = 3 * i - 3; vec3 ret; memcpy(ret.entry, a + st, sizeof(ret.entry)); return ret; }

		/// <summary>
		/// n�� ���͸� �����մϴ�. 1~4�� �Է� �����մϴ�.
		/// </summary>
		/// <param name="i">�� �ε���(1 base)</param>
		inline vec3 col(int i) const { assert(i <= 3 && i >= 1); return vec3(a[i - 1], a[i + 2], a[i + 5]); }

		/// <summary>
		/// ��İ��� �����մϴ�.
		/// </summary>
		inline mat3 operator*(const mat3& m) const {
			mat3 ret;
			for (int i = 1, ent = 0; i <= 3; i++) {
				vec3 r = row(i);
				for (int j = 1; j <= 3; j++, ent++) {
					vec3 c = m.col(j);
					ret[ent] = r.dot(c);
				}
			}
			return ret;
		}

		/// <summary>
		/// ��İ��� �����մϴ�.
		/// </summary>
		inline mat3& operator*=(const mat3& m) { return *this = operator*(m); }

		/// <summary>
		/// ���Ϳ� ������ȯ�� �����Ͽ� �����մϴ�.
		/// </summary>
		inline vec3 operator*(const vec3& v) const { return vec3(row(1).dot(v), row(2).dot(v), row(3).dot(v)); }

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
		/// ��� �밢 ������ ���� ��ȯ�մϴ�.
		/// </summary>
		inline float trace() const { return _11 + _22 + _33; }

		/// <summary>
		/// ������� ��ȯ�մϴ�.
		/// </summary>
		inline mat3 inverse() const {
			float d = det();
			if (d == 0) printf("%p: �� ����� ������� ���ų� �ſ� ū ������ �����ϴ�. NaN�� ���� ����ġ ���� ������ �߻��� �� �ֽ��ϴ�.\n", this);
			return mat3(
				(_22 * _33 - _32 * _23), (_13 * _32 - _12 * _33), (_12 * _23 - _13 * _22),
				(_23 * _31 - _21 * _33), (_11 * _33 - _13 * _31), (_21 * _13 - _11 * _23),
				(_21 * _32 - _31 * _22), (_31 * _12 - _11 * _32), (_11 * _22 - _21 * _12)
			) / d;
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
		/// 3���� ȸ���� �����մϴ�.
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
		inline mat4() { memset(a, 0, sizeof(a)); _11 = _22 = _33 = _44 = 1; }

		/// <summary>
		/// �� �켱 ������ �Ű������� �־� ����� �����մϴ�.
		/// </summary>
		inline mat4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44) :_11(_11), _12(_12), _13(_13), _14(_14), _21(_21), _22(_22), _23(_23), _24(_24), _31(_31), _32(_32), _33(_33), _34(_34), _41(_41), _42(_42), _43(_43), _44(_44) { }

		/// <summary>
		/// ����� ������ �����ؼ� �����մϴ�.
		/// </summary>
		inline mat4(const mat4& m) { memcpy(a, m.a, sizeof(a)); }

		/// <summary>
		/// �ε��� ������
		/// </summary>
		inline float& operator[](ptrdiff_t i) { return a[i]; }
		inline const float& operator[](ptrdiff_t i) const { return a[i]; }

		/// <summary>
		/// ����� ������ķ� �ٲߴϴ�.
		/// </summary>
		inline void toI() { memset(a, 0, sizeof(a)); _11 = _22 = _33 = _44 = 1; }

		/// <summary>
		/// ��������̸� true�� �����մϴ�.
		/// </summary>
		inline bool isI() const { return memcmp(mat4().a, a, sizeof(a)) == 0; }

		/// <summary>
		/// �ٸ� ��İ� ���к��� ���ϰų� ���ϴ�.
		/// </summary>
		inline mat4& operator+=(const mat4& m) { add4<float>(a, m.a); add4<float>(a + 4, m.a + 4); add4<float>(a + 8, m.a + 8); add4<float>(a + 12, m.a + 12); return *this; }
		inline mat4& operator-=(const mat4& m) { sub4<float>(a, m.a); sub4<float>(a + 4, m.a + 4); sub4<float>(a + 8, m.a + 8); sub4<float>(a + 12, m.a + 12); return *this; }
		inline mat4 operator+(const mat4& m) const { auto r = mat4(*this); r += m; return r; }
		inline mat4 operator-(const mat4& m) const { auto r = mat4(*this); r -= m; return r; }

		/// <summary>
		/// n�� ���͸� �����մϴ�. 1~4�� �Է� �����մϴ�.
		/// </summary>
		/// <param name="i">�� �ε���(1 base)</param>
		inline vec4 row(int i) const { assert(i <= 4 && i >= 1); int st = 4 * i - 4; vec4 ret; memcpy(ret.entry, a + st, sizeof(ret.entry)); return ret; }

		/// <summary>
		/// n�� ���͸� �����մϴ�. 1~4�� �Է� �����մϴ�.
		/// </summary>
		/// <param name="i">�� �ε���(1 base)</param>
		inline vec4 col(int i) const { assert(i <= 4 && i >= 1); return vec4(a[i - 1], a[i + 3], a[i + 7], a[i + 11]); }

		/// <summary>
		/// ��İ��� �����մϴ�.
		/// </summary>
		inline mat4 operator*(const mat4& m) const {
			mat4 ret;
			for (int i = 1, ent = 0; i <= 4; i++) {
				vec4 r = row(i);
				for (int j = 1; j <= 4; j++, ent++) {
					vec4 c = m.col(j);
					ret[ent] = r.dot(c);
				}
			}
			return ret;
		}

		/// <summary>
		/// ��İ��� �����մϴ�.
		/// </summary>
		inline mat4& operator*=(const mat4& m) { return *this = operator*(m); }

		/// <summary>
		/// ���Ϳ� ������ȯ�� �����Ͽ� �����մϴ�.
		/// </summary>
		inline vec4 operator*(const vec4& v) const { 
			return vec4(row(1).dot(v), row(2).dot(v), row(3).dot(v), row(4).dot(v));
		}

		/// <summary>
		/// ��Ŀ� �Ǽ��踦 �մϴ�.
		/// </summary>
		inline mat4& operator*=(float f) { mul4<float>(a, f); mul4<float>(a + 4, f); mul4<float>(a + 8, f); mul4<float>(a + 12, f); return *this; }
		inline mat4 operator*(float f) const { mat4 r(*this); r *= f; return r; }
		inline mat4& operator/=(float f) { div4<float>(a, f); div4<float>(a + 4, f); div4<float>(a + 8, f); div4<float>(a + 12, f); return *this; }
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
		/// ����� �밢�� ���� ���� �����մϴ�.
		/// </summary>
		inline float trace() const { return _11 + _22 + _33 + _44; }

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
				1, 0, 0, 0,
				0, 1 / tanf(fovy / 2), 0, 0,
				0, 0, (dnear + dfar) / (dnear - dfar), (2 * dnear * dfar) / (dnear - dfar) ,
				0, 0,					-1, 0
			);
			r._11 = r._22 / aspect;
			return r;
		}

		/// <summary>
		/// �� ���簢���� �ٸ� ���簢������ ��ȯ�ϴ� ����� ����մϴ�. ���簢���� ������ ��-��-��-�����Դϴ�. z ��ǥ�� �����ϴٰ� �����Ͽ� xy ��鿡���� �̵��մϴ�.
		/// </summary>
		/// <param name="r1">��ȯ �� ���簢��</param>
		/// <param name="r2">��ȯ �� ���簢��</param>
		/// <param name="z">���簢���� ��ġ�� z��ǥ(-1�� ���� ��)</param>
		inline static mat4 r2r(const vec4& r1, const vec4& r2, float z = 0) {
			vec4 sc = r2 / r1;	vec4 tr = r2 - r1 * vec4(sc.z, sc.w);
			return mat4(
				sc.z, 0, 0, tr.x,
				0, sc.w, 0, tr.y,
				0, 0, 1, z,
				0, 0, 0, 1
			);
		}
		
		/// <summary>
		/// �� �������� Mesh::get("rect")�� �����Ǵ� ���� ���簢��(�߽��� 0,0�̰� �� ���� ���̰� 1�� ���簢��)�� �ٸ� ���簢������ ��ȯ�ϴ� ����� ����մϴ�.
		/// ���簢���� ������ ��-��-��-�����Դϴ�. z ��ǥ�� �����ϴٰ� �����Ͽ� xy ��鿡���� �̵��մϴ�.
		/// </summary>
		/// <param name="r2">��ȯ �� ���簢��</param>
		/// <param name="z">���簢���� ��ġ�� z��ǥ(-1�� ���� ��)</param>
		inline static mat4 r2r(const vec4& r2, float z = 0) {
			return r2r(vec4(-0.5f, -0.5f, 1, 1), r2, z);
		}

		/// <summary>
		/// �� ���簢��(L-D-W-H ����)�� �ٸ� ���簢���� ���ʿ� �°� ��ȯ�մϴ�. �� �߽��� �����ϸ�, ���� ���簢���� ��Ⱦ��� �����ϸ鼭 ���� ū ���簢���� �ǵ��� �����մϴ�.
		/// </summary>
		/// <param name="r1">��ȯ �� ���簢��</param>
		/// <param name="r2">��ȯ �� ���簢��</param>
		/// <param name="z">���簢���� ��ġ�� z��ǥ(-1�� ���� ��)</param>
		inline static mat4 r2r2(const vec4& r1, const vec4& r2, float z = 0) {
			float r = r1.width / r1.height;
			vec4 targ(r2);
			if (targ.width < targ.height * r) {	// ���μ��� ���� ��
				targ.down += (targ.height - targ.width / r) / 2;
				targ.height = targ.width / r;
			}
			else {	// ���μ��� ���� ��
				targ.left += (targ.width - targ.height * r) / 2;
				targ.width = targ.height * r;
			}
			return r2r(r1, targ, z);
		}
	};

	/// <summary>
	/// 3���� ȸ�� ���� ǥ���ϴ� ������Դϴ�. 1, i, j, k �κп� �ش��ϴ� c1, ci, cj, ck ����� �����ϴ�.
	/// </summary>
	struct Quaternion {
		union {
			struct { float c1, ci, cj, ck; };
			struct { float w, x, y, z; };
		};

		/// <summary>
		/// ������� �����մϴ�.
		/// </summary>
		/// <param name="o">�Ǽ��κ�</param>
		/// <param name="i">i�κ�</param>
		/// <param name="j">j�κ�</param>
		/// <param name="k">k�κ�</param>
		inline Quaternion(float o = 1, float i = 0, float j = 0, float k = 0) :c1(o), ci(i), cj(j), ck(k) {};

		/// <summary>
		/// ������� �����ؼ� �����մϴ�.
		/// </summary>
		inline Quaternion(const Quaternion& q) { set4<float>(&c1, &(q.c1)); }

		/// <summary>
		/// ����� ũ���� ������ �����մϴ�.
		/// </summary>
		inline float abs2() const { return reinterpret_cast<const vec4*>(this)->length2(); }

		/// <summary>
		/// ����� ũ�⸦ �����մϴ�.
		/// </summary
		inline float abs() const { return sqrtf(abs2()); }

		/// <summary>
		/// ������� ��ȸ������ Ȯ���մϴ�.
		/// </summary>
		inline bool is1() const { return c1 == 1 && ci == 0 && cj == 0 && ck == 0; }

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
			Quaternion q_c1 = q * c1;
			Quaternion q_ci = Quaternion(-q.ci, q.c1, -q.ck, q.cj) * ci;
			Quaternion q_cj = Quaternion(-q.cj, q.ck, q.c1, -q.ci) * cj;
			Quaternion q_ck = Quaternion(-q.ck, -q.cj, q.ci, q.c1) * ck;
			return q_c1 + q_ci + q_cj + q_ck;
		}

		/// <summary>
		/// ����� �� ��Ģ �����Դϴ�. ��� ������ �� ������� �������� ������ ����˴ϴ�.
		/// </summary>
		inline Quaternion& operator+=(const Quaternion& q) { add4<float>(&c1, &(q.c1)); return *this; }
		inline Quaternion& operator-=(const Quaternion& q) { sub4<float>(&c1, &(q.c1)); return *this; }
		inline Quaternion& operator*=(const Quaternion& q) { *this = *this * q; return *this; }	// multiplying on the left is more commonly used operation
		inline Quaternion& operator/=(const Quaternion& q) { *this = *this * q.inverse(); return *this; }
		inline Quaternion operator*(float f) const { Quaternion ret(*this); mul4<float>(&(ret.c1), f); return ret; }
		inline Quaternion operator/(float f) const { Quaternion ret(*this); div4<float>(&(ret.c1), f); return ret; }
		inline Quaternion& operator*=(float f) { *this = *this * f; return *this; }
		inline Quaternion& operator/=(float f) { *this = *this / f; return *this; }
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
		/// <param name="angle">ȸ����(����)</param>
		inline void compound(const vec3& axis, float angle) { auto q = rotation(axis, angle); compound(q); }

		/// <summary>
		/// ������� ȸ�� ��ķ� �����մϴ�.
		/// </summary>
		inline mat4 toMat4() const {

			Quaternion i = (*this) * ci;
			Quaternion j = (*this) * cj;
			Quaternion k = (*this) * ck;

			return mat4(
				1 - 2 * (j.cj + k.ck), 2 * (i.cj - k.c1), 2 * (i.ck + j.c1), 0,
				2 * (i.cj + k.c1), 1 - 2 * (i.ci + k.ck), 2 * (j.ck - i.c1), 0,
				2 * (i.ck - j.c1), 2 * (j.ck + i.c1), 1 - 2 * (i.ci + j.cj), 0,
				0, 0, 0, 1
			);
		}

		/// <summary>
		/// ù ���п� ȸ����(����), ������ ���п� 3���� ȸ������ ��� �����մϴ�.
		/// �ε��Ҽ��� ���е� ������ ����Ͽ� ����ȭ�Ͽ� ����մϴ�. ȸ��������� �ƴ϶� nan�� �߻����� �����Ƿ� �����ϼ���.
		/// </summary>
		inline vec4 axis() const {
			Quaternion ax = *this / abs();
			float angle = acosf(ax.c1) * 2;
			float sinha = sqrtf(1 - ax.c1 * ax.c1);
			ax /= sinha;
			ax.c1 = angle;
			return vec4(*(reinterpret_cast<vec4*>(&ax)));
		}
		
		/// <summary>
		/// �� ȸ���� ���Ϸ� �� (x,y,z��)�� ���·� �����մϴ�.
		/// �ε��Ҽ��� ���е� ������ ����Ͽ� ����ȭ�Ͽ� ����մϴ�. ȸ��������� �ƴ϶� nan�� �߻����� �����Ƿ� �����ϼ���.
		/// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_conversion
		/// </summary>
		inline vec3 toEuler() const {
			Quaternion q = *this / abs();
			vec3 a;
			float sinrcosp = 2 * (q.c1 * q.ci + q.cj * q.ck);
			float cosrcosp = 1 - 2 * (q.ci * q.ci + q.cj * q.cj);
			a.x = atan2f(sinrcosp, cosrcosp);
			float sinp = 2 * (q.c1 * q.cj - q.ck * q.ci);
			if (sinp >= 1) a.y = PI / 2;
			else if (sinp <= -1) a.y = -PI / 2;
			else a.y = asinf(sinp);
			float sinycosp = 2 * (q.c1 * q.ck + q.ci * q.cj);
			float cosycosp = 1 - 2 * (q.cj * q.cj + q.ck * q.ck);
			a.z = atan2f(sinycosp, cosycosp);
			return a;
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
		/// <param name="roll">roll(X�� ���� ȸ��)</param>
		/// <param name="yaw">yaw(Z�� ���� ȸ��)</param>
		/// <param name="pitch">pitch(Y�� ���� ȸ��)</param>
		inline static Quaternion euler(float roll, float pitch, float yaw) {
			vec3 cypr(yaw, pitch, roll), sypr(cypr);
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
		float costh = reinterpret_cast<const vec4*>(&q1)->dot(*reinterpret_cast<const vec4*>(&q2)) / q1.abs() / q2.abs();
		// ���е� ������ ���� nan ����
		if (costh > 1) costh = 1;
		else if (costh < -1) costh = -1;
		float theta = acos(costh);
		float sn = sin(theta);

		// q1=q2�̰ų� 180�� ������ ���
		if (sn <= FLT_EPSILON) return q1;
		Wa = sin((1 - t) * theta) / sn;
		Wb = sin(t * theta) / sn;

		Quaternion r = q1 * Wa + q2 * Wb;
		return r / r.abs();
	}

	inline mat3 mat3::rotate(float roll, float pitch, float yaw) { return mat3(mat4::rotate(roll, pitch, yaw)); }
	inline mat4 mat4::rotate(const vec3& axis, float angle) { return Quaternion::rotation(axis, angle).toMat4(); }
	inline mat4 mat4::rotate(float roll, float pitch, float yaw) { return Quaternion::euler(roll, pitch, yaw).toMat4(); }
	inline mat4 mat4::TRS(const vec3& translation, const Quaternion& rotation, const vec3& scale) {
		// SIMD ������ �� �� 30ȸ/�� 6ȸ, T*R*S ���� �ϴ� ��� �� 149ȸ/�� 102ȸ
		// SIMD ���� �� �� 15ȸ/�� 6ȸ, ���� �ϴ� ��� �� 44ȸ/�� 102ȸ
		mat4 r = rotation.toMat4();
		vec4 sc(scale);
		mul4<float>(r.a, sc);
		mul4<float>(r.a + 4, sc);
		mul4<float>(r.a + 8, sc);
		r[3] = translation.x;
		r[7] = translation.y;
		r[11] = translation.z;
		return r;
	}

	// ������ �߰� �����ε�
	template<unsigned D, class T>inline nvec<D, T> operator+(float f, const nvec<D, T>& v) { return v + f; }
	template<unsigned D, class T>inline nvec<D, T> operator*(float f, const nvec<D, T>& v) { return v * f; }
	inline mat4 operator*(float f, const mat4& m) { return m * f; }
	inline Quaternion operator*(float f, const Quaternion& q) { return q * f; }

	/// <summary>
	/// �ִ�/�ּ� ������ �ξ� �ڸ� ���� �����մϴ�. ù ���� ��������Ǵ� �⺻�ڷ����� �ƴ� ���� ����� ���� �����ϼ���.
	/// </summary>
	/// <param name="t">�ڸ��� �� ��</param>
	/// <param name="min">�ּڰ�</param>
	/// <param name="max">�ִ�</param>
	/// <returns></returns>
	template<class T>inline T clamp(T t, const T& min, const T& max) {
		t = t < min ? min : t;
		t = t > max ? max : t;
		return t;
	};

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
	inline void print(int i, const char* tag = "", char end = '\n') { printf("%s: %d%c", tag, i, end); }
	inline void print(int64_t i, const char* tag = "", char end = '\n') { printf("%s: %lld%c", tag, i, end); }
	inline void print(unsigned i, const char* tag = "", char end = '\n') { printf("%s: %d%c", tag, i, end); }
	inline void print(uint64_t i, const char* tag = "", char end = '\n') { printf("%s: %lld%c", tag, i, end); }
	inline void print(float i, const char* tag = "", char end = '\n') { printf("%s: %f%c", tag, i, end); }
}

#endif // !__OAGLEM_H__