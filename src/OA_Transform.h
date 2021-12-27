#ifndef __OA_TRANSFORM_H__
#define __OA_TRANSFORM_H__

#include "oaglem.h"

namespace onart {
	/// <summary>
	/// 개체의 위치, 크기, 회전 상태를 나타내는 클래스입니다. (다형성을 가상함수 테이블 때문에 사용하지 않고 모두 인라인 함수인 대신 이 계층의 코딩이 약간 번거로워집니다.)
	/// 주어진 것 외 다른 행렬 클래스를 사용하는 경우 이 클래스를 그대로 사용할 수 없을 가능성이 매우 높습니다.
	/// <para>맨 앞에 d가 붙은 setter 멤버함수는 매 프레임마다 위치/회전/크기 중 둘 이상 업데이트될 수 있는 개체에 대하여 사용하는 것이 좋습니다. 이후 dGetModel()로 모델 행렬을 얻어와야만 합니다.</para>
	/// <para>맨 앞에 d가 붙지 않은 setter 멤버함수는 매 프레임마다 주로 하나 이하가 업데이트되는 개체에 대하여 사용하는 것이 좋습니다. 이후 getModel()로 모델 행렬을 얻어오면 됩니다. 예를 들어 2D 게임에서 회전이 없는 주인공 같은 경우 이 함수를 사용하는 것이 훨씬 효율이 높습니다.</para>
	/// <para>어떤 방식으로 업데이트하든 결과는 같으므로, 최적화가 충분한 경우 d가 붙은 멤버만 사용하는 것도 방법입니다.</para>
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
		/// 인게임 모델 4x4 행렬을 리턴합니다.
		/// <para>매 프레임마다 위치/회전/크기 중 주로 하나 이하가 업데이트되는 경우 이 함수가 조금 더 빠릅니다.</para>
		/// </summary>
		inline const mat4& getModel() const { return model; }
		/// <summary>
		/// 인게임 모델 4x4 행렬을 계산한 후 리턴합니다.
		/// <para>매 프레임마다 위치/회전/크기 중 둘 이상 업데이트될 수 있는 경우 이 함수가 조금 더 빠릅니다.</para>
		/// </summary>
		inline const mat4& dGetModel() { model = mat4::TRS(pos, rotation, scale); return model; }
		
		/// <summary>
		/// 3D 위치를 리턴합니다.
		/// </summary>
		inline const vec3& getPosition() const { return pos; }
		/// <summary>
		/// 3D 크기 배율을 리턴합니다.
		/// </summary>
		inline const vec3& getScale() const { return scale; }
		/// <summary>
		/// 3D 회전 사원수를 리턴합니다.
		/// </summary>
		inline const Quaternion& getRotation() const { return rotation; }
		/// <summary>
		/// 3D 크기 배율을 설정합니다.
		/// </summary>
		inline void dSetScale(const vec3& sc) { scale = sc; }
		/// <summary>
		/// 3D 크기 배율을 설정하고 모델 행렬을 업데이트합니다.
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
		/// 3D 크기 배율을 설정합니다.
		/// </summary>
		inline void dSetScale(float x = 1, float y = 1, float z = 1) { scale.x = x; scale.y = y; scale.z = z; }
		/// <summary>
		/// 3D 크기 배율을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setScale(float x = 1, float y = 1, float z = 1) { setScale({ x,y,z }); }
		/// <summary>
		/// 3D 위치를 설정합니다.
		/// </summary>
		inline void dSetPosition(const vec3& p) { pos = p; }
		/// <summary>
		/// 3D 위치를 설정합니다.
		/// </summary>
		inline void dSetPosition(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z; }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPosition(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z; model[3] = x; model[7] = y; model[11] = z; }
		/// <summary>
		/// x좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionX(float x) { pos.x = x; }
		/// <summary>
		/// y좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionY(float y) { pos.y = y; }
		/// <summary>
		/// z좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionZ(float z) { pos.z = z; }
		/// <summary>
		/// x좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPositionX(float x) { pos.x = x; model[3] = x; }
		/// <summary>
		/// y좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPositionY(float y) { pos.y = y; model[7] = y; }
		/// <summary>
		/// z좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPositionZ(float z) { pos.z = z; model[11] = z; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동합니다.
		/// </summary>
		inline void dAddPosition(const vec3& dp) { pos += dp; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동합니다.
		/// </summary>
		inline void dAddPosition(float x, float y, float z) { pos.x += x; pos.y += y; pos.z += z; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPosition(float x, float y, float z) { pos.x += x; pos.y += y; pos.z += z; model[3] += x; model[7] += y; model[11] += z; }
		/// <summary>
		/// x좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionX(float x) { pos.x += x; }
		/// <summary>
		/// y좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionY(float y) { pos.y += y; }
		/// <summary>
		/// z좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionZ(float z) { pos.z += z; }
		/// <summary>
		/// x좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addtPositionX(float x) { pos.x += x; model[3] += x; }
		/// <summary>
		/// y좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPositionY(float y) { pos.y += y; model[7] += y; }
		/// <summary>
		/// z좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPositionZ(float z) { pos.z += z; model[11] += z; }
		/// <summary>
		/// 3D 회전을 설정합니다.
		/// </summary>
		inline void dSetRotation(const Quaternion& q) { rotation = q; }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D 회전을 설정합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D 회전을 설정합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; model = mat4::TRS(pos, rotation, scale); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; model = mat4::TRS(pos, rotation, scale); }
	};
}

#endif // !__OA_TRANSFORM_H__
