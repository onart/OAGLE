/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_TRANSFORM_H__
#define __OA_TRANSFORM_H__

#include <vector>
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
		std::vector<Transform*> children;
		Quaternion rotation;
		vec3 scale = 1;
		vec3 pos;
		mat4 model;

		vec3 globalScale;	// �θ� ȸ���� �ִ� ���, �ǹ̰� ���. �ܺ� ���� ����x
		vec3 globalPos;		// �ܺ� ���� ����
		Quaternion globalRotation;	// �ܺ� ���� ����x
		mat4 globalModel;
		bool ready = false, globalReady = false;

		/// <summary>
		/// NOTE: ready�� true�� ���� �� �ִ� �Լ��� TRS()�� mat2prs()���̾�� �մϴ�.
		/// </summary>
		inline void TRS() { model = mat4::TRS(pos, rotation, scale); ready = true; }
		/// <summary>
		/// NOTE: ready�� true�� ���� �� �ִ� �Լ��� TRS()�� mat2prs()���̾�� �մϴ�.
		/// </summary>
		void mat2prs();
		/// <summary>
		/// 
		/// </summary>
		void gmat2prs();
	public:
		/// <summary>
		/// ���� ��ġ/ũ��/ȸ���� �θ� ��ȯ�� ����Ͽ� ��ȯ�� �����մϴ�.
		/// </summary>
		Transform(const vec3& pos = 0, const vec3& scale = 1, const Quaternion& rot = { 1,0,0,0 }, Transform* parent = nullptr);
		/// <summary>
		/// ��ȯ�� �����մϴ�. �θ� ��ȯ�� ����������, �ڽ� ��ȯ�� �������� �ʽ��ϴ�.
		/// </summary>
		Transform(const Transform& tr);
		/// <summary>
		/// ���� ��ȯ ��İ� �θ� ��ȯ�� �־� ��ȯ�� �����մϴ�. ���� ��ȯ�� �ƴ� ���� �������� �ʾҽ��ϴ�.
		/// </summary>
		Transform(const mat4& tr, Transform* parent = nullptr);
		/// <summary>
		/// �θ� Ʈ�������� �����մϴ�. nullptr�� �־� �����ϰų� �θ� ����Ǵ� ��쿡�� �۷ι� ��ȯ�� �����˴ϴ�.
		/// </summary>
		/// <param name="p">�θ� Ʈ������</param>
		void setParent(Transform* p = nullptr);
		inline Transform* getParent() const { return parent; }
		/// <summary>
		/// �־��� ��ȯ�� �ڽ� �߿� ������ �����մϴ�.
		/// </summary>
		inline void excludeChild(Transform* t) { children.erase(std::remove(children.begin(), children.end(), t), children.end()); }
		/// <summary>
		/// �־��� ��ȯ�� �ڽĿ� ���Խ�ŵ�ϴ�. �ߺ� ������ �˻����� �ʽ��ϴ�.
		/// </summary>
		inline void addChild(Transform* t) { if (t && t != this)children.push_back(t); }
		/// <summary>
		/// �ڽ� ��ȯ�� �۷ι� ��ȯ�� ����� ���� ��ȯ�� �޾ư��� ���� ���մϴ�. �� �Լ��� �ʿ��� ���� �ڵ����� ȣ��ǹǷ�, ���뿡���� ȣ���ص� ������ �߻����� ������, ���ɸ� �������� ����� ��� �˴ϴ�.
		/// </summary>
		void globalNotReady();
		/// <summary>
		/// �ΰ���(�۷ι�) �� ��ȯ�� ������� �����մϴ�.
		/// </summary>
		mat4 getInverseTransform();
		/// <summary>
		/// �ΰ��� �� 4x4 ����� �����մϴ�.
		/// NOTE: globalReady�� true�� ���� �� �ִ� ���� ����� setGlobal �迭���̾�� �մϴ�.
		/// </summary>
		const mat4& getModel();
		/// <summary>
		/// ����� �̿��� ��ġ, ȸ��, ũ�⸦ ������Ʈ�մϴ�. ���� ��ȯ�� �ƴϰų� ����� ��ȯ(����, shear)�� ��쿡 �߻��ϴ� ������� ���� ��Ȳ�� å������ �ʽ��ϴ�.
		/// �� ��Ȯ����, �߰� ��ȯ�� ������ �ʴ� ��� ���ϴ� ��ȯ��� ��������� �߰� ȸ��/����/ũ�� ��ȯ�� �ϴ� ��� ó�� ��ȯ�� �޶����ϴ�.
		/// </summary>
		inline void setModel(const mat4& m) { model = m; mat2prs(); globalNotReady(); }
		/// <summary>
		/// �۷ι� ��ġ�� �����մϴ�.
		/// </summary>
		inline const vec3& getGlobalPosition() { if (!parent)return pos; getModel(); return globalPos; }
		/// <summary>
		/// 3D �۷ι� ��ġ�� �����մϴ�.
		/// </summary>
		inline const vec3& getLocalPosition() const { return pos; }
		/// <summary>
		/// �۷ι� 3D ũ�� ������ �����մϴ�.
		/// NOTE: �۷ι� ������ �θ� ��ȯ �� ȸ���� ������ �ǹ̰� ũ�� ����˴ϴ�.
		/// </summary>
		inline const vec3& getScale() { if (!parent)return scale; getModel(); return globalScale; }
		/// <summary>
		/// ���� ��ȯ�� 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline const vec3& getLocalScale() const { return scale; }
		/// <summary>
		/// �۷ι� 3D ȸ�� ������� �����մϴ�.
		/// </summary>
		inline const Quaternion& getRotation() { if (!parent)return rotation; getModel(); return globalRotation; }
		/// <summary>
		/// 3D ���� ȸ�� ������� �����մϴ�.
		/// </summary>
		inline const Quaternion& getLocalRotation() const { return rotation; }
		/// <summary>
		/// 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline void dSetScale(const vec3& sc) { scale = sc; ready = false; globalNotReady(); }
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
			globalNotReady();
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
		inline void dSetLocalPosition(const vec3& p) { pos = p; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline void dSetLocalPosition(float x, float y, float z) { dSetLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setLocalPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; globalNotReady(); }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setLocalPosition(float x, float y, float z) { setLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// ���� �������� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void setPosition(const vec3& p);
		/// <summary>
		/// ���� �������� ��ȭ��Ű�� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void addPosition(const vec3& p);
		/// <summary>
		/// ���� X��ǥ�� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void setPositionX(float x);
		/// <summary>
		/// ���� Y��ǥ�� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void setPositionY(float y);
		/// <summary>
		/// ���� Z��ǥ�� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void setPositionZ(float z);
		/// <summary>
		/// ���� X��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void addPositionX(float x);
		/// <summary>
		/// ���� Y��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void addPositionY(float y);
		/// <summary>
		/// ���� Z��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		void addPositionZ(float z);
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetLocalPositionX(float x) { pos.x = x; ready = false; globalNotReady(); }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetLocalPositionY(float y) { pos.y = y; ready = false; globalNotReady(); }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetLocalPositionZ(float z) { pos.z = z; ready = false; globalNotReady(); }
		/// <summary>
		/// x��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setLocalPositionX(float x) { pos.x = x; model[3] = x; globalNotReady(); }
		/// <summary>
		/// y��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setLocalPositionY(float y) { pos.y = y; model[7] = y; globalNotReady(); }
		/// <summary>
		/// z��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setLocalPositionZ(float z) { pos.z = z; model[11] = z; globalNotReady(); }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��մϴ�.
		/// </summary>
		inline void dAddLocalPosition(const vec3& dp) { pos += dp; ready = false; globalNotReady(); }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��մϴ�.
		/// </summary>
		inline void dAddLocalPosition(float x, float y, float z) { dAddLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addLocalPosition(const vec3& p) { pos += p; model[3] += p.x; model[7] += p.y; model[11] += p.z; globalNotReady(); }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addLocalPosition(float x, float y, float z) { addLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddLocalPositionX(float x) { pos.x += x; ready = false; globalNotReady(); }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddLocalPositionY(float y) { pos.y += y; ready = false; globalNotReady(); }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddLocalPositionZ(float z) { pos.z += z; ready = false; globalNotReady(); }
		/// <summary>
		/// x��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addLocalPositionX(float x) { pos.x += x; model[3] += x; globalNotReady(); }
		/// <summary>
		/// y��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addLocalPositionY(float y) { pos.y += y; model[7] += y; globalNotReady(); }
		/// <summary>
		/// z��ǥ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addLocalPositionZ(float z) { pos.z += z; model[11] += z; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�.
		/// </summary>
		inline void dSetRotation(const Quaternion& q) { rotation = q; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); TRS(); globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw); ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw); TRS(); globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void addRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw) * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void addRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw) * rotation; TRS(); globalNotReady(); }
	};
}

#endif // !__OA_TRANSFORM_H__
