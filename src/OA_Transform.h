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
	/// 개체의 위치, 크기, 회전 상태를 나타내는 클래스입니다. (다형성을 가상함수 테이블 때문에 사용하지 않고 모두 인라인 함수인 대신 이 계층의 코딩이 약간 번거로워집니다.)
	/// <para>oaglem.h 외 다른 행렬 클래스를 사용하는 경우 이 클래스를 그대로 사용할 수 없을 가능성이 매우 높습니다.</para>
	/// <para>맨 앞에 d가 붙은 setter 멤버함수는 매 프레임마다 위치/회전/크기 중 둘 이상 업데이트될 수 있는 개체에 대하여 사용하는 것이 좋습니다.</para>
	/// <para>맨 앞에 d가 붙지 않은 setter 멤버함수는 매 프레임마다 주로 하나 이하가 업데이트되는 개체에 대하여 사용하는 것이 좋습니다.</para>
	/// <para>어떤 방식으로 업데이트하든 결과는 같으므로, 최적화가 충분한 경우 d가 붙은 멤버만 사용하는 것도 방법입니다.</para>
	/// (중요) parent가 유효하지 않은 포인터인 경우에 대한 예외 케이스는 상정되지 않았습니다. 따라서 Transform은 웬만해선 Entity의 멤버로만 사용하며, parent 포인터를 사용하는 경우 응용(Entity 파생) 계층에서 반드시 부모와의 관리를 직접 하시기 바랍니다.
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
		/// NOTE: ready를 true로 만들 수 있는 함수는 여기뿐이어야 합니다.
		/// </summary>
		inline void TRS() { model = mat4::TRS(pos, rotation, scale); ready = true; }
	public:
		inline Transform(const vec3& pos = 0, const vec3& scale = 1, const Quaternion& rot = { 1,0,0,0 }, Transform* parent = nullptr) :pos(pos), rotation(rot), scale(scale), parent(parent) { TRS(); }
		inline Transform(const Transform& tr) : pos(tr.getPosition()), rotation(tr.getRotation()), scale(tr.getScale()), parent(tr.getParent()) { TRS(); }
		/// <summary>
		/// 부모 트랜스폼을 설정합니다.
		/// </summary>
		/// <param name="p">부모 트랜스폼</param>
		inline void setParent(Transform* p = nullptr) { parent = p; }
		inline Transform* getParent() const { return parent; }
		/// <summary>
		/// 인게임 모델 4x4 행렬을 리턴합니다.
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
		/// 이 함수는 const Transform 객체에서 model을 얻는 데 쓰입니다.
		/// </summary>
		inline const mat4& getModel() const { return model; }
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
		inline void dSetScale(const vec3& sc) { scale = sc; ready = false; }
		/// <summary>
		/// 3D 크기 배율을 설정하고 모델 행렬을 업데이트합니다.
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
		/// 3D 크기 배율을 설정합니다.
		/// </summary>
		inline void dSetScale(float x, float y, float z) { dSetScale(vec3(x, y, z)); }
		/// <summary>
		/// 3D 크기 배율을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setScale(float x, float y, float z) { setScale(vec3(x, y, z)); }
		/// <summary>
		/// 3D 위치를 설정합니다.
		/// </summary>
		inline void dSetPosition(const vec3& p) { pos = p; ready = false; }
		/// <summary>
		/// 3D 위치를 설정합니다.
		/// </summary>
		inline void dSetPosition(float x, float y, float z) { dSetPosition(vec3(x, y, z)); }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
		/// <summary>
		/// x좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionX(float x) { pos.x = x; ready = false; }
		/// <summary>
		/// y좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionY(float y) { pos.y = y; ready = false; }
		/// <summary>
		/// z좌표만 변경합니다.
		/// </summary>
		inline void dSetPositionZ(float z) { pos.z = z; ready = false; }
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
		inline void dAddPosition(const vec3& dp) { pos += dp; ready = false; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동합니다.
		/// </summary>
		inline void dAddPosition(float x, float y, float z) { dAddPosition(vec3(x, y, z)); }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPosition(const vec3& p) { pos += p; model[3] += p.x; model[7] += p.y; model[11] += p.z; }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addPosition(float x, float y, float z) { addPosition(vec3(x, y, z)); }
		/// <summary>
		/// x좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionX(float x) { pos.x += x; ready = false; }
		/// <summary>
		/// y좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionY(float y) { pos.y += y; ready = false; }
		/// <summary>
		/// z좌표만 누적합니다.
		/// </summary>
		inline void dAddPositionZ(float z) { pos.z += z; ready = false; }
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
		inline void dSetRotation(const Quaternion& q) { rotation = q; ready = false; }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; TRS(); }
		/// <summary>
		/// 3D 회전을 설정합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); ready = false; }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); TRS(); }
		/// <summary>
		/// 3D 회전을 설정합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); ready = false; }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll); TRS(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; ready = false; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; TRS(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; ready = false; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void addRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; TRS(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; ready = false; }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void addRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(yaw, pitch, roll) * rotation; TRS(); }
	};
}

#endif // !__OA_TRANSFORM_H__
