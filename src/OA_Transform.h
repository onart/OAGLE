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
		std::vector<Transform*> children;
		Quaternion rotation;
		vec3 scale = 1;
		vec3 pos;
		mat4 model;

		vec3 globalScale;	// 부모에 회전이 있는 경우, 의미가 퇴색. 외부 수정 제공x
		vec3 globalPos;		// 외부 수정 제공
		Quaternion globalRotation;	// 외부 수정 제공x
		mat4 globalModel;
		bool ready = false, globalReady = false;

		/// <summary>
		/// NOTE: ready를 true로 만들 수 있는 함수는 TRS()와 mat2prs()뿐이어야 합니다.
		/// </summary>
		inline void TRS() { model = mat4::TRS(pos, rotation, scale); ready = true; }
		/// <summary>
		/// NOTE: ready를 true로 만들 수 있는 함수는 TRS()와 mat2prs()뿐이어야 합니다.
		/// </summary>
		void mat2prs();
		/// <summary>
		/// 
		/// </summary>
		void gmat2prs();
	public:
		/// <summary>
		/// 로컬 위치/크기/회전과 부모 변환을 명시하여 변환을 생성합니다.
		/// </summary>
		Transform(const vec3& pos = 0, const vec3& scale = 1, const Quaternion& rot = { 1,0,0,0 }, Transform* parent = nullptr);
		/// <summary>
		/// 변환을 복사합니다. 부모 변환은 공유하지만, 자식 변환은 복사하지 않습니다.
		/// </summary>
		Transform(const Transform& tr);
		/// <summary>
		/// 아핀 변환 행렬과 부모 변환을 주어 변환을 생성합니다. 아핀 변환이 아닌 경우는 상정되지 않았습니다.
		/// </summary>
		Transform(const mat4& tr, Transform* parent = nullptr);
		/// <summary>
		/// 부모 트랜스폼을 변경합니다. nullptr를 주어 독립하거나 부모가 변경되는 경우에도 글로벌 변환은 유지됩니다.
		/// </summary>
		/// <param name="p">부모 트랜스폼</param>
		void setParent(Transform* p = nullptr);
		inline Transform* getParent() const { return parent; }
		/// <summary>
		/// 주어진 변환이 자식 중에 있으면 제외합니다.
		/// </summary>
		inline void excludeChild(Transform* t) { children.erase(std::remove(children.begin(), children.end(), t), children.end()); }
		/// <summary>
		/// 주어진 변환을 자식에 포함시킵니다. 중복 포함은 검사하지 않습니다.
		/// </summary>
		inline void addChild(Transform* t) { if (t && t != this)children.push_back(t); }
		/// <summary>
		/// 자식 변환이 글로벌 변환을 만들기 위해 변환을 받아가야 함을 뜻합니다. 이 함수는 필요할 때만 자동으로 호출되므로, 응용에서는 호출해도 오류가 발생하지 않지만, 성능만 나빠지는 결과를 얻게 됩니다.
		/// </summary>
		void globalNotReady();
		/// <summary>
		/// 인게임(글로벌) 모델 변환의 역행렬을 리턴합니다.
		/// </summary>
		mat4 getInverseTransform();
		/// <summary>
		/// 인게임 모델 4x4 행렬을 리턴합니다.
		/// NOTE: globalReady를 true로 만들 수 있는 곳은 여기와 setGlobal 계열뿐이어야 합니다.
		/// </summary>
		const mat4& getModel();
		/// <summary>
		/// 행렬을 이용해 위치, 회전, 크기를 업데이트합니다. 아핀 변환이 아니거나 기울임 변환(전단, shear)인 경우에 발생하는 고려되지 않은 상황은 책임지지 않습니다.
		/// 더 정확히는, 추가 변환을 가하지 않는 경우 원하는 변환대로 수행되지만 추가 회전/병진/크기 변환을 하는 경우 처음 변환과 달라집니다.
		/// </summary>
		inline void setModel(const mat4& m) { model = m; mat2prs(); globalNotReady(); }
		/// <summary>
		/// 글로벌 위치를 리턴합니다.
		/// </summary>
		inline const vec3& getGlobalPosition() { if (!parent)return pos; getModel(); return globalPos; }
		/// <summary>
		/// 3D 글로벌 위치를 리턴합니다.
		/// </summary>
		inline const vec3& getLocalPosition() const { return pos; }
		/// <summary>
		/// 글로벌 3D 크기 배율을 리턴합니다.
		/// NOTE: 글로벌 배율은 부모 변환 중 회전이 있으면 의미가 크게 퇴색됩니다.
		/// </summary>
		inline const vec3& getScale() { if (!parent)return scale; getModel(); return globalScale; }
		/// <summary>
		/// 현재 변환의 3D 크기 배율을 리턴합니다.
		/// </summary>
		inline const vec3& getLocalScale() const { return scale; }
		/// <summary>
		/// 글로벌 3D 회전 사원수를 리턴합니다.
		/// </summary>
		inline const Quaternion& getRotation() { if (!parent)return rotation; getModel(); return globalRotation; }
		/// <summary>
		/// 3D 지역 회전 사원수를 리턴합니다.
		/// </summary>
		inline const Quaternion& getLocalRotation() const { return rotation; }
		/// <summary>
		/// 3D 크기 배율을 설정합니다.
		/// </summary>
		inline void dSetScale(const vec3& sc) { scale = sc; ready = false; globalNotReady(); }
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
			globalNotReady();
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
		inline void dSetLocalPosition(const vec3& p) { pos = p; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 위치를 설정합니다.
		/// </summary>
		inline void dSetLocalPosition(float x, float y, float z) { dSetLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setLocalPosition(const vec3& p) { pos = p; model[3] = p.x; model[7] = p.y; model[11] = p.z; globalNotReady(); }
		/// <summary>
		/// 3D 위치를 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setLocalPosition(float x, float y, float z) { setLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// 월드 포지션을 정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void setPosition(const vec3& p);
		/// <summary>
		/// 월드 포지션을 변화시키고 모델 행렬을 업데이트합니다.
		/// </summary>
		void addPosition(const vec3& p);
		/// <summary>
		/// 월드 X좌표를 정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void setPositionX(float x);
		/// <summary>
		/// 월드 Y좌표를 정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void setPositionY(float y);
		/// <summary>
		/// 월드 Z좌표를 정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void setPositionZ(float z);
		/// <summary>
		/// 월드 X좌표를 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void addPositionX(float x);
		/// <summary>
		/// 월드 Y좌표를 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void addPositionY(float y);
		/// <summary>
		/// 월드 Z좌표를 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		void addPositionZ(float z);
		/// <summary>
		/// x좌표만 변경합니다.
		/// </summary>
		inline void dSetLocalPositionX(float x) { pos.x = x; ready = false; globalNotReady(); }
		/// <summary>
		/// y좌표만 변경합니다.
		/// </summary>
		inline void dSetLocalPositionY(float y) { pos.y = y; ready = false; globalNotReady(); }
		/// <summary>
		/// z좌표만 변경합니다.
		/// </summary>
		inline void dSetLocalPositionZ(float z) { pos.z = z; ready = false; globalNotReady(); }
		/// <summary>
		/// x좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setLocalPositionX(float x) { pos.x = x; model[3] = x; globalNotReady(); }
		/// <summary>
		/// y좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setLocalPositionY(float y) { pos.y = y; model[7] = y; globalNotReady(); }
		/// <summary>
		/// z좌표만 변경하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setLocalPositionZ(float z) { pos.z = z; model[11] = z; globalNotReady(); }
		/// <summary>
		/// 주어진 값만큼 위치를 이동합니다.
		/// </summary>
		inline void dAddLocalPosition(const vec3& dp) { pos += dp; ready = false; globalNotReady(); }
		/// <summary>
		/// 주어진 값만큼 위치를 이동합니다.
		/// </summary>
		inline void dAddLocalPosition(float x, float y, float z) { dAddLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addLocalPosition(const vec3& p) { pos += p; model[3] += p.x; model[7] += p.y; model[11] += p.z; globalNotReady(); }
		/// <summary>
		/// 주어진 값만큼 위치를 이동하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addLocalPosition(float x, float y, float z) { addLocalPosition(vec3(x, y, z)); }
		/// <summary>
		/// x좌표만 누적합니다.
		/// </summary>
		inline void dAddLocalPositionX(float x) { pos.x += x; ready = false; globalNotReady(); }
		/// <summary>
		/// y좌표만 누적합니다.
		/// </summary>
		inline void dAddLocalPositionY(float y) { pos.y += y; ready = false; globalNotReady(); }
		/// <summary>
		/// z좌표만 누적합니다.
		/// </summary>
		inline void dAddLocalPositionZ(float z) { pos.z += z; ready = false; globalNotReady(); }
		/// <summary>
		/// x좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addLocalPositionX(float x) { pos.x += x; model[3] += x; globalNotReady(); }
		/// <summary>
		/// y좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addLocalPositionY(float y) { pos.y += y; model[7] += y; globalNotReady(); }
		/// <summary>
		/// z좌표만 누적하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addLocalPositionZ(float z) { pos.z += z; model[11] += z; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정합니다.
		/// </summary>
		inline void dSetRotation(const Quaternion& q) { rotation = q; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const Quaternion& q) { rotation = q; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dSetRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void setRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle); TRS(); globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dSetRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw); ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 설정하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void setRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw); TRS(); globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		inline void dAddRotation(const Quaternion& q) { rotation = q * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		inline void addRotation(const Quaternion& q) { rotation = q * rotation; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void dAddRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다.
		/// </summary>
		/// <param name="axis">회전축</param>
		/// <param name="angle">회전각</param>
		inline void addRotation(const vec3& axis, float angle) { rotation = Quaternion::rotation(axis, angle) * rotation; TRS(); globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void dAddRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw) * rotation; ready = false; globalNotReady(); }
		/// <summary>
		/// 3D 회전을 추가로 가하고 모델 행렬을 업데이트합니다. 오른손 법칙을 기억하세요.
		/// </summary>
		/// <param name="roll">X축 방향의 회전</param>
		/// <param name="pitch">Y축 방향의 회전</param>
		/// <param name="yaw">Z축 방향의 회전</param>
		inline void addRotation(float roll, float pitch, float yaw) { rotation = Quaternion::euler(roll, pitch, yaw) * rotation; TRS(); globalNotReady(); }
	};
}

#endif // !__OA_TRANSFORM_H__
