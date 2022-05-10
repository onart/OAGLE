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
	/// 2���� ��鿡�� 2�� ������ ������ �����մϴ�.
	/// ������ ���� ��� (nan, nan)�� ���ϵ˴ϴ�.
	/// ������ ��� ���� ���ο� �����ϰ� (nan, 0)�� ���ϵ˴ϴ�.
	/// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
	/// </summary>
	/// <param name="p1">���� 1�� ���� 1</param>
	/// <param name="p2">���� 1�� ���� 2</param>
	/// <param name="q1">���� 2�� ���� 1</param>
	/// <param name="q2">���� 2�� ���� 2</param>
	inline vec2 intersect(const vec2& p1, const vec2& p2, const vec2& q1, const vec2& q2) {
		// cf) n�� �� �����ϴ� ���� https://www.geeksforgeeks.org/given-a-set-of-line-segments-find-if-any-two-segments-intersect/
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
	/// 2���� ��鿡�� 2�� ������ ������ ������ �����մϴ�. ������ ������ NAN�� �����մϴ�.
	/// </summary>
	/// <param name="p1">���� 1�� ���� 1</param>
	/// <param name="p2">���� 1�� ���� 2</param>
	/// <param name="q1">���� 2�� ���� 1</param>
	/// <param name="q2">���� 2�� ���� 2</param>
	inline vec2 intersect2(const vec2& p1, const vec2& p2, const vec2& q1, const vec2& q2) {
		vec2 p1q1(q1 - p1);
		vec2 p1q2(q2 - p1);
		vec2 p(p2 - p1);
		mat2 adj(
			p1q2[1], -p1q2[0],
			-p1q1[1], p1q1[0]
		);
		float d = adj.det();
		if (d == 0) {	// �� �� Ÿ�� �б�
			vec2 st = adj * p;
			if (st[0] * st[1] > 0) return q1;
			else {
				// ���� ��Ȯ�ϰ� �ϴ� ��: x �Ǵ� y ��ǥ�� ����, p1, p2>q1, q2�ų� �� �ݴ�� NAN,
				// �� ���� ��� ��� 2���� ���
				// ������ ��κ��� ��� �ӵ��� ���� ������ ������ NAN���� ū ���� ����
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
	/// float ���� 2���� ���Ͽ� ���翵�� ����մϴ�.
	/// </summary>
	/// <param name="v1">�翵��ȯ�� �������Դϴ�.</param>
	/// <param name="v2">�翵��ȯ�� �����Դϴ�.</param>
	template <unsigned D>
	inline nvec<D> project(const nvec<D>& v1, const nvec<D>& v2) {
		return v1.dot(v2) * v1.normal();
	}
	
	/// <summary>
	/// ���� ������ �����ų� ������ ���ο� �ִ��� Ȯ���մϴ�. (������ Ȯ���� �����մϴ�.)
	/// </summary>
	/// <param name="center">���� �߽�</param>
	/// <param name="radius">���� ������</param>
	/// <param name="p1">���� ���� 1</param>
	/// <param name="p2">���� ���� 2</param>
	/// <returns></returns>
	inline vec2 lineCircleIntersect(const vec2& center, float radius, const vec2& p1, const vec2& p2) {
		USE_NVEC_XYZW;
		// ���͹����Ŀ� ���� �Ÿ��� �̺а�� 0�� ���� (0,1)�� ��, �� ��� ���� �߽��� �Ÿ��� ���������� ������ Ȯ��
		vec2 pc(center - p1);
		vec2 p(p2 - p1);
		float t = pc.dot(p) / p.dot(p);
		t = clamp(t, 0.0f, 1.0f);
		vec2 nearest = p1 + p * t;
		if (nearest.distance2(center) <= radius * radius) return nearest;
		else return NAN;
	}

	/// <summary>
	/// 3���� ���� ���� ���� ���� ��� �� �ﰢ�� �ȿ� �ִ��� Ȯ���մϴ�.
	/// �ﰢ���� ���� ���� ��� ���� ���� ���� ��쿡 ���� ����� ������ ���ǵ��� �ʾҽ��ϴ�.
	/// </summary>
	/// <param name="p">��</param>
	/// <param name="t1">�ﰢ�� ������ 1</param>
	/// <param name="t2">�ﰢ�� ������ 2</param>
	/// <param name="t3">�ﰢ�� ������ 3</param>
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
		// pseudo inverse �ʿ�.
	}

	/// <summary>
	/// RGB ���� �����ϴ� vec3�� �޾� HSV ����(�ٻ�ġ)�� �����մϴ�.
	/// �Է��ؾ� �� RGB ���� ������ 0~255 ������ ������ �ƴ� 0~1 ������ �Ǽ��̸� ���ϰ��� 0~1 ������ �Ǽ��Դϴ�.
	/// �� �� H���� ������ 360 deg = 2PI rad�Դϴ�. ���� ��� 0.5�� 180���� ���մϴ�.
	/// https://thebookofshaders.com/06/
	/// </summary>
	inline vec3 hsv(const vec3& rgb) {
		vec4 p = rgb[1] < rgb[2] ? vec4(rgb[2], rgb[1], -1.0f, 2.0f / 3) : vec4(rgb[1], rgb[2], 0, -1.0f / 3);
		vec4 q = rgb[0] < p[0] ? vec4(p[0], p[1], p[3], rgb[0]) : vec4(rgb[0], p[1], p[2], p[0]);
		float d = q[0] - std::min(q[3], q[1]);
		return vec3(fabsf(q[2] + (q[3] - q[1]) / (6.0f * d + FLT_EPSILON)), d / (q[0] + FLT_EPSILON), q[0]);
	}

	/// <summary>
	/// HSV ���� �����ϴ� vec3�� �޾� RGB ����(�ٻ�ġ)�� �����մϴ�.
	/// �Է� H���� ������ 360 deg = 2PI rad�̸� �������� 0~1 ������ �Ǽ��Դϴ�.
	/// ���� rgb���� 0~1������ �Ǽ��� ǥ���˴ϴ�.
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

