#ifndef __OA_TRANSFORM_H__
#define __OA_TRANSFORM_H__

#include "oaglem.h"

namespace onart {
	/// <summary>
	/// ��ü�� ��ġ, ũ��, ȸ�� ���¸� ��Ÿ���� Ŭ�����Դϴ�. (�������� �����Լ� ���̺� ������ ������� �ʰ� ��� �ζ��� �Լ��� ��� �� ������ �ڵ��� �ణ ���ŷο����ϴ�.)
	/// �־��� �� �� �ٸ� ��� Ŭ������ ����ϴ� ��� �� Ŭ������ �״�� ����� �� ���� ���ɼ��� �ſ� �����ϴ�.
	/// <para>�� �տ� d�� ���� setter ����Լ��� �� �����Ӹ��� ��ġ/ȸ��/ũ�� �� �� �̻� ������Ʈ�� �� �ִ� ��ü�� ���Ͽ� ����ϴ� ���� �����ϴ�. ���� dGetModel()�� �� ����� ���;߸� �մϴ�.</para>
	/// <para>�� �տ� d�� ���� ���� setter ����Լ��� �� �����Ӹ��� �ַ� �ϳ� ���ϰ� ������Ʈ�Ǵ� ��ü�� ���Ͽ� ����ϴ� ���� �����ϴ�. ���� getModel()�� �� ����� ������ �˴ϴ�. ���� ��� 2D ���ӿ��� ȸ���� ���� ���ΰ� ���� ��� �� �Լ��� ����ϴ� ���� �ξ� ȿ���� �����ϴ�.</para>
	/// <para>� ������� ������Ʈ�ϵ� ����� �����Ƿ�, ����ȭ�� ����� ��� d�� ���� ����� ����ϴ� �͵� ����Դϴ�.</para>
	/// </summary>
	class Transform
	{
	private:
		Quaternion rotation;
		vec3 scale = 1;
		vec3 pos;
		mat4 model;
	public:
		inline Transform(const vec3& pos = 0, const vec3& scale = 1, const Quaternion& rot = { 1,0,0,0 }) :pos(pos), rotation(rot), scale(scale) { model = mat4::TRS(pos, rotation, scale); }
		inline Transform(const Transform& tr) : pos(tr.getPosition()), rotation(tr.getRotation()), scale(tr.getScale()) { model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// �ΰ��� �� 4x4 ����� �����մϴ�.
		/// <para>�� �����Ӹ��� ��ġ/ȸ��/ũ�� �� �ַ� �ϳ� ���ϰ� ������Ʈ�Ǵ� ��� �� �Լ��� ���� �� �����ϴ�.</para>
		/// </summary>
		inline const mat4& getModel() const { return model; }
		/// <summary>
		/// �ΰ��� �� 4x4 ����� ����� �� �����մϴ�.
		/// <para>�� �����Ӹ��� ��ġ/ȸ��/ũ�� �� �� �̻� ������Ʈ�� �� �ִ� ��� �� �Լ��� ���� �� �����ϴ�.</para>
		/// </summary>
		inline const mat4& dGetModel() { model = mat4::TRS(pos, rotation, scale); return model; }
		
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
		inline void dSetScale(const vec3& sc) { scale = sc; }
		/// <summary>
		/// 3D ũ�� ������ �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setScale(const vec3& sc) {
			vec3 v = sc / scale;
			scale = sc;
			if (isfinite(v.x) && isfinite(v.y) && isfinite(v.z)) { 
				model[0] *= v.x;	model[1] *= v.y;	model[2] *= v.z;
				model[4] *= v.x;	model[5] *= v.y;	model[6] *= v.z;
				model[8] *= v.x;	model[9] *= v.y;	model[10] *= v.z;
			}
			else {
				model = mat4::TRS(pos, rotation, scale);
			}
		}
		/// <summary>
		/// 3D ũ�� ������ �����մϴ�.
		/// </summary>
		inline void dSetScale(float x = 1, float y = 1, float z = 1) { scale.x = x; scale.y = y; scale.z = z; }
		/// <summary>
		/// 3D ũ�� ������ �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setScale(float x = 1, float y = 1, float z = 1) { setScale({ x,y,z }); }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline void dSetPosition(const vec3& p) { pos = p; }
		/// <summary>
		/// 3D ��ġ�� �����մϴ�.
		/// </summary>
		inline void dSetPosition(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z; }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// 3D ��ġ�� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setPosition(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z; model[3] = x; model[7] = y; model[11] = z; }
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionX(float x) { pos.x = x; }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionY(float y) { pos.y = y; }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dSetPositionZ(float z) { pos.z = z; }
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
		inline void dAddPosition(const vec3& dp) { pos += dp; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��մϴ�.
		/// </summary>
		inline void dAddPosition(float x, float y, float z) { pos.x += x; pos.y += y; pos.z += z; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// �־��� ����ŭ ��ġ�� �̵��ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addPosition(float x, float y, float z) { pos.x += x; pos.y += y; pos.z += z; model[3] += x; model[7] += y; model[11] += z; }
		/// <summary>
		/// x��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionX(float x) { pos.x += x; }
		/// <summary>
		/// y��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionY(float y) { pos.y += y; }
		/// <summary>
		/// z��ǥ�� �����մϴ�.
		/// </summary>
		inline void dAddPositionZ(float z) { pos.z += z; }
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
		inline void dSetRotation(const Quaternion& q) { rotation = q; }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D ȸ���� �����մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); }
		/// <summary>
		/// 3D ȸ���� �����ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�.
		/// </summary>
		/// <param name="axis">ȸ����</param>
		/// <param name="angle">ȸ����</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D ȸ���� �߰��� ���մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; }
		/// <summary>
		/// 3D ȸ���� �߰��� ���ϰ� �� ����� ������Ʈ�մϴ�. ������ ��Ģ�� ����ϼ���.
		/// </summary>
		/// <param name="roll">X�� ������ ȸ��</param>
		/// <param name="pitch">Y�� ������ ȸ��</param>
		/// <param name="yaw">Z�� ������ ȸ��</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; model = mat4::TRS(pos, rotation, scale); }
	};
}

#endif // !__OA_TRANSFORM_H__
