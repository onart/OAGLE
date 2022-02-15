/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_TRANSFORM_H__
#define __OA_TRANSFORM_H__

#include "oaglem.h"

namespace onart {
	/// <summary>
	/// ��ü�� ��ġ, ũ��, ȸ�� ���¸� ��Ÿ���� Ŭ�����Դϴ�. (�������� �����Լ� ���̺� ������ ������� �ʰ� ��� �ζ��� �Լ��� ��� �� ������ �ڵ��� �ణ ���ŷο����ϴ�.)
	/// <para>oaglem.h �� �ٸ� ��� Ŭ������ ����ϴ� ��� �� Ŭ������ �״�� ����� �� ���� ���ɼ��� �ſ� �����ϴ�.</para>
	/// <para>�� �տ� d�� ���� setter ����Լ��� �� �����Ӹ��� ��ġ/ȸ��/ũ�� �� �� �̻� ������Ʈ�� �� �ִ� ��ü�� ���Ͽ� ����ϴ� ���� �����ϴ�.</para>
	/// <para>�� �տ� d�� ���� ���� setter ����Լ��� �� �����Ӹ��� �ַ� �ϳ� ���ϰ� ������Ʈ�Ǵ� ��ü�� ���Ͽ� ����ϴ� ���� �����ϴ�.</para>
	/// <para>� ������� ������Ʈ�ϵ� ����� �����Ƿ�, ����ȭ�� ����� ��� d�� ���� ����� ����ϴ� �͵� ����Դϴ�.</para>
	/// (�߿�) parent�� ��ȿ���� ���� �������� ��쿡 ���� ���� ���̽��� �������� �ʾҽ��ϴ�. ���� Transform�� �����ؼ� Entity�� ����θ� ����ϸ�, parent �����͸� ����ϴ� ��� ����(Entity �Ļ�) �������� �ݵ�� �θ���� ������ ���� �Ͻñ� �ٶ��ϴ�.
	/// </summary>
	class Transform
	{
	private:
		Transform* parent = nullptr;
		bool ready;
		Quaternion rotation;
		vec3 scale = 1;
		vec3 pos;
		mat4 model;
		/// <summary>
		/// NOTE: ready�� true�� ���� �� �ִ� �Լ��� ������̾�� �մϴ�.
		/// </summary>
		inline void TRS() { model = mat4::TRS(pos, rotation, scale); ready = true; }
	public:
		inline Transform(const vec3& pos = 0, const vec3& scale = 1, const Quaternion& rot = { 1,0,0,0 }, Transform* parent = nullptr) :pos(pos), rotation(rot), scale(scale), parent(parent) { TRS(); }
		inline Transform(const Transform& tr) : pos(tr.getPosition()), rotation(tr.getRotation()), scale(tr.getScale()), parent(tr.getParent()) { TRS(); }
		/// <summary>
		/// �θ� Ʈ�������� �����մϴ�.
		/// </summary>
		/// <param name="p">�θ� Ʈ������</param>
		inline void setParent(Transform* p = nullptr) { parent = p; }
		inline Transform* getParent() const { return parent; }
		/// <summary>
		/// �ΰ��� �� 4x4 ����� �����մϴ�.
		/// </summary>
		inline const mat4 getModel() { 
			if (!ready) { TRS(); }
			if (parent) {
				return parent->getModel() * model;
			}
			else {
				return model;
			}
		}
		/// <summary>
		/// �� �Լ��� const Transform ��ü���� model�� ��� �� ���Դϴ�.
		/// </summary>
		inline const mat4& getModel() const { return model; }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline const vec3& getPosition() const { return pos; }
		/// <summary>
		/// 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline const vec3& getScale() const { return scale; }
		/// <summary>
		/// 3D ȸ�� ������� �����մϴ�.
		/// </summary>
		inline const Quaternion& getRotation() const { return rotation; }
		/// <summary>
		/// 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline void dSetScale(const vec3& sc) { scale = sc; ready = false; }
		/// <summary>
		/// 3D ũ�� ������ �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setScale(const vec3& sc) {
			vec3 v = sc / scale;
			scale = sc;
			if (isfinite(v.x) && isfinite(v.y) && isfinite(v.z)) {
				float m[4];

				m[0] = model[0]; m[1] = model[4]; m[2] = model[8];
				mul4<float>(m, v.x);
				model[0] = m[0];	model[4] = m[1];	model[8] = m[2];

				m[0] = model[1]; m[1] = model[5]; m[2] = model[9];
				mul4<float>(m, v.y);
				model[1] = m[0];	model[5] = m[1];	model[9] = m[2];

				m[0] = model[2]; m[1] = model[6]; m[2] = model[10];
				mul4<float>(m, v.z);
				model[2] = m[0];	model[6] = m[1];	model[10] = m[2];
			}
			else {
				TRS();
			}
		}
		/// <summary>
		/// 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline void dSetScale(float x, float y, float z) { dSetScale(vec3(x, y, z)); }
		/// <summary>
		/// 3D ũ�� ������ �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setScale(float x, float y, float z) { setScale(vec3(x, y, z)); }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline void dSetPosition(const vec3& p) { pos = p; ready = false; }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline void dSetPosition(float x, float y, float z) { dSetPosition(vec3(x, y, z)); }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionX(float x) { pos.x = x; ready = false; }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionY(float y) { pos.y = y; ready = false; }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionZ(float z) { pos.z = z; ready = false; }
		/// <summary>
		/// x��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPositionX(float x) { pos.x = x; model[3] = x; }
		/// <summary>
		/// y��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPositionY(float y) { pos.y = y; model[7] = y; }
		/// <summary>
		/// z��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPositionZ(float z) { pos.z = z; model[11] = z; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��մϴ�.
		/// </summary>
		inline void dAddPosition(const vec3& dp) { pos += dp; ready = false; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��մϴ�.
		/// </summary>
		inline void dAddPosition(float x, float y, float z) { dAddPosition(vec3(x, y, z)); }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPosition(const vec3& p) { pos += p; model[3] += p.x; model[7] += p.y; model[11] += p.z; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPosition(float x, float y, float z) { addPosition(vec3(x, y, z)); }
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionX(float x) { pos.x += x; ready = false; }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionY(float y) { pos.y += y; ready = false; }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionZ(float z) { pos.z += z; ready = false; }
		/// <summary>
		/// x��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addtPositionX(float x) { pos.x += x; model[3] += x; }
		/// <summary>
		/// y��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPositionY(float y) { pos.y += y; model[7] += y; }
		/// <summary>
		/// z��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPositionZ(float z) { pos.z += z; model[11] += z; }
		/// <summary>
		/// 3D ȸ���� �����մϴ�.
		/// </summary>
		inline void dSetRotation(const Quaternion& q) { rotation = q; ready = false; }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; TRS(); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); ready = false; }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); TRS(); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); ready = false; }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); TRS(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; ready = false; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; TRS(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; ready = false; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void addRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; TRS(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; ready = false; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void addRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; TRS(); }
	};
}

#endif // !__OA_TRANSFORM_H__
