/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OAGLEM_GEOMETRY_H__
#define __OAGLEM_GEOMETRY_H__

#include "oaglem.h"

namespace onart {
	/// <summary>
	/// 2차원 평면에서 2개 선분의 교점을 리턴합니다.
	/// 교점이 없는 경우 (nan, nan)이 리턴됩니다.
	/// 나란한 경우 교점 여부에 무관하게 (nan, 0)이 리턴됩니다.
	/// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
	/// </summary>
	/// <param name="p1">선분 1의 끝점 1</param>
	/// <param name="p2">선분 1의 끝점 2</param>
	/// <param name="q1">선분 2의 끝점 1</param>
	/// <param name="q2">선분 2의 끝점 2</param>
	inline vec2 intersect(const vec2& p1, const vec2& p2, const vec2& q1, const vec2& q2) {
		// cf) n개 중 접촉하는 선분 https://www.geeksforgeeks.org/given-a-set-of-line-segments-find-if-any-two-segments-intersect/
		vec2 _13(p1 - q1);
		vec2 _34(q1 - q2);
		vec2 _12(p1 - p2);
		float d = cross2(_12, _34);
		if (d == 0) return vec2(NAN, 0);
		float vn = cross2(_13, _12);
		float tn = cross2(_13, _34);
		if (d < 0) {
			if (tn <= 0 && tn >= d && vn <= 0 && vn >= d) return p1 - _12 * (tn / d);
			else return NAN;
		}
		else {
			if (tn >= 0 && tn <= d && vn >= 0 && vn <= d) return p1 - _12 * (tn / d);
			else return NAN;
		}
	}

	/// <summary>
	/// 2차원 평면에서 2개 선분이 만나는 지점을 리턴합니다. 만나지 않으면 NAN을 리턴합니다.
	/// </summary>
	/// <param name="p1">선분 1의 끝점 1</param>
	/// <param name="p2">선분 1의 끝점 2</param>
	/// <param name="q1">선분 2의 끝점 1</param>
	/// <param name="q2">선분 2의 끝점 2</param>
	inline vec2 intersect2(const vec2& p1, const vec2& p2, const vec2& q1, const vec2& q2) {
		vec2 p1q1(q1 - p1);
		vec2 p1q2(q2 - p1);
		vec2 p(p2 - p1);
		mat2 adj(
			p1q2[1], -p1q2[0],
			-p1q1[1], p1q1[0]
		);
		float d = adj.det();
		if (d == 0) {	// 잘 안 타는 분기
			vec2 st = adj * p;
			if (st[0] * st[1] > 0) return q1;
			else {
				// 보다 정확하게 하는 법: x 또는 y 좌표로 정렬, p1, p2>q1, q2거나 그 반대면 NAN,
				// 그 외의 경우 가운데 2개의 평균
				// 하지만 대부분의 경우 속도가 아주 빠르지 않으면 NAN으로 큰 문제 없음
				return NAN;
			}
		}
		else {
			vec2 st = adj * p / d;
			if (st[0] >= 0 && st[1] >= 0 && st[0] + st[1] >= 1.0f) {
				return p1 + p / (st[0] + st[1]);
			}
			return NAN;
		}
	}

	/// <summary>
	/// float 벡터 2개에 대하여 정사영을 계산합니다.
	/// </summary>
	/// <param name="v1">사영변환의 목적지입니다.</param>
	/// <param name="v2">사영변환될 벡터입니다.</param>
	template <unsigned D>
	inline nvec<D> project(const nvec<D>& v1, const nvec<D>& v2) {
		return v1.dot(v2) * v1.normal();
	}
	
	/// <summary>
	/// 원과 선분이 만나거나 선분이 내부에 있는지 확인합니다. (다차원 확장이 가능합니다.)
	/// </summary>
	/// <param name="center">원의 중심</param>
	/// <param name="radius">원의 반지름</param>
	/// <param name="p1">선분 끝점 1</param>
	/// <param name="p2">선분 끝점 2</param>
	/// <returns></returns>
	inline vec2 lineCircleIntersect(const vec2& center, float radius, const vec2& p1, const vec2& p2) {
		USE_NVEC_XYZW;
		// 벡터방정식에 대한 거리의 미분계수 0인 점을 (0,1)로 컷, 그 결과 점과 중심의 거리가 반지름보다 작은지 확인
		vec2 pc(center - p1);
		vec2 p(p2 - p1);
		float t = pc.dot(p) / p.dot(p);
		t = clamp(t, 0.0f, 1.0f);
		vec2 nearest = p1 + p * t;
		if (nearest.distance2(center) <= radius * radius) return nearest;
		else return NAN;
	}

	/// <summary>
	/// 3차원 공간 상의 점이 동일 평면 내 삼각형 안에 있는지 확인합니다.
	/// 삼각형과 점이 동일 평면 내에 있지 않은 경우에 대한 결과는 별도로 정의되지 않았습니다.
	/// </summary>
	/// <param name="p">점</param>
	/// <param name="t1">삼각형 꼭짓점 1</param>
	/// <param name="t2">삼각형 꼭짓점 2</param>
	/// <param name="t3">삼각형 꼭짓점 3</param>
	inline bool pointInTriangle(const vec3& p, const vec3& t1, const vec3& t2, const vec3& t3) {
		vec3 _12(t2 - t1);
		vec3 _23(t3 - t2);
		vec3 p1(p - t1);
		vec3 p2(p - t2);
		vec3 p12(std::move(cross(_12, p1)));
		vec3 p23(std::move(cross(_12, p1)));
		{
			vec3 p123(p12 * p23);
			if (p123[0] < 0 || p123[1] < 0 || p123[2] < 0) {
				return false;
			}
		}
		vec3 p3(p - t3);
		vec3 _31(t1 - t3);
		vec3 p31(std::move(cross(_12, p1)));
		{
			vec3 p123(p12 * p31);
			return !(p123[0] < 0 || p123[1] < 0 || p123[2] < 0);
		}
	}

	inline bool pointInTriangle2(const vec3& p, const vec3& t1, const vec3& t2, const vec3& t3) {
		// pseudo inverse 필요.
	}

	/// <summary>
	/// RGB 값을 포함하는 vec3를 받아 HSV 형식(근사치)을 리턴합니다.
	/// 입력해야 할 RGB 값의 범위는 0~255 사이의 정수가 아닌 0~1 사이의 실수이며 리턴값도 0~1 사이의 실수입니다.
	/// 이 중 H값의 단위는 360 deg = 2PI rad입니다. 예를 들어 0.5는 180도를 뜻합니다.
	/// https://thebookofshaders.com/06/
	/// </summary>
	inline vec3 hsv(const vec3& rgb) {
		vec4 p = rgb[1] < rgb[2] ? vec4(rgb[2], rgb[1], -1.0f, 2.0f / 3) : vec4(rgb[1], rgb[2], 0, -1.0f / 3);
		vec4 q = rgb[0] < p[0] ? vec4(p[0], p[1], p[3], rgb[0]) : vec4(rgb[0], p[1], p[2], p[0]);
		float d = q[0] - std::min(q[3], q[1]);
		return vec3(fabsf(q[2] + (q[3] - q[1]) / (6.0f * d + FLT_EPSILON)), d / (q[0] + FLT_EPSILON), q[0]);
	}

	/// <summary>
	/// HSV 값을 포함하는 vec3를 받아 RGB 형식(근사치)을 리턴합니다.
	/// 입력 H값의 단위는 360 deg = 2PI rad이며 나머지도 0~1 사이의 실수입니다.
	/// 리턴 rgb값은 0~1사이의 실수로 표현됩니다.
	/// https://thebookofshaders.com/06/
	/// </summary>
	inline vec3 rgb(const vec3& hsv) {
		vec3 _rgb = vec3(0, 4, 2) + hsv[0] * 6;
		_rgb[0] = fmodf(_rgb[0], 6.0f);
		_rgb[1] = fmodf(_rgb[1], 6.0f);
		_rgb[2] = fmodf(_rgb[2], 6.0f);
		_rgb -= 3;
		abs4<float>(_rgb.entry);
		_rgb -= 1;
		clamp4<float>(_rgb.entry, 0, 1);
		_rgb = _rgb * _rgb * (-2.0f * _rgb + 3.0f);
		return lerp(vec3(1), _rgb, hsv[1]) * hsv[2];
	}
}

#endif // !__OAGLEM_GEOMETRY_H__

