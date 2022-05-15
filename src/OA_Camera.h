/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_CAMERA_H__
#define __OA_CAMERA_H__
#include "oaglem.h"

namespace onart {
	class Transform;
	class Camera
	{
	public:
		/// <summary>
		/// 카메라가 3차원 벡터를 따라갑니다. nullptr를 대입하는 경우 카메라가 원래 가려던 위치까지 가서 정지하지만,
		/// 카메라가 따라가던 동적 할당된 vec3 객체가 해제된 경우 프로그램이 깨지니 주의하세요.
		/// </summary>
		/// <param name="relativePos">카메라가 볼 지점을 기준으로 한 카메라의 위치입니다.</param>
		/// <param name="at">카메라가 따라갈 트랜스폼입니다.</param>
		void follow(const vec3& relativePos, Transform* at = nullptr);
		/// <summary>
		/// 카메라가 원하는 위치를 보도록 고정합니다.
		/// </summary>
		/// <param name="relativePos">카메라가 볼 지점을 기준(0,0,0)으로 한 카메라의 위치입니다.</param>
		/// <param name="fixedAt">카메라가 볼 지점입니다.</param>
		inline void fix(const vec3& relativePos, const vec3& fixedAt) { at = nullptr; this->fixedAt = fixedAt; this->relativePos = relativePos; }

		/// <summary>
		/// 카메라의 움직임 딜레이를 설정합니다.
		/// </summary>
		/// <param name="s">딜레이의 결정 변수입니다. 카메라가 eye의 위치까지 완전히(at이 정지한 기준, 카메라가 이동해야 할 거리의 99%) 이동하는 데 걸리는 시간을 의미합니다. 음의 값을 넣을 경우 강제로 양의 값으로 변환됩니다.</param>
		void setDelay(float s);
		
		/// <summary>
		/// 카메라에 따른 뷰 행렬을 업데이트하여 셰이더에 넘깁니다. FPS가 20 이상인 경우 카메라의 움직임이 크게 차이 나지 않습니다.
		/// </summary>
		void viewUpdate();

		/// <summary>
		/// 카메라의 줌 수준을 정합니다. 즉 카메라에 표시되는 월드의 범위를 조정합니다.
		/// </summary>
		void setZoom(float zoom);

		/// <summary>
		/// Input::relativeCursorPos()는 뷰포트 좌측 상단이 (0,0), 우측 하단이 (1,1)이 되도록 리턴하는데 이를 프로그램 내에서 통용되는 표준 뷰 좌표(중심이 (0,0)이며 종/횡 중 짧은 쪽의 길이가 2)로 변환합니다.
		/// 커서가 뷰포트 밖으로 나갔더라도 특별한 결과값을 리턴하지 않습니다.
		/// </summary>
		/// <param name="mousePos">Input::relativeCursorPos()에서 받은 값</param>
		vec2 mouse2screen(const vec2& mousePos);

		/// <summary>
		/// 카메라가 이동할 때 보는 방향을 고정합니다. 특히 2D 게임에서 카메라 딜레이를 0보다 크게 설정할 경우 필수입니다.
		/// </summary>
		/// <param name="fix">고정 여부(false로 정하면 고정을 해제합니다.)</param>
		inline void fixDirection(bool fix = true) { fixdir = fix; }
		
		/// <summary>
		/// 월드 상 좌표가 화면상에서 어느 위치에 나오는지 알려줍니다. 뷰포트는 중심이 (0,0)이며 종/횡 중 짧은 쪽의 길이가 2입니다.
		/// </summary>
		inline vec3 world2screen(const vec3& pos) { return zoom * ratio.projM4 * viewM4 * vec4(pos, 1.0f); }
		
		/// <summary>
		/// 뷰 행렬을 리턴합니다.
		/// </summary>
		inline const mat4& getViewMatrix() const { return viewM4; }
		/// <summary>
		/// 뷰포트의 비율을 결정합니다.
		/// </summary>
		struct Ratio {
			friend class Camera;
		private:
			unsigned R_WIDTH = 16, R_HEIGHT = 9;
			float ratio = 16.0f / 9;
			float fovy = PI / 6, dnear = -1, dfar = 1;
			mat4 projM4;
			mat4 getAspectMatrix();
		public:
			/// <summary>
			/// 종횡비를 설정합니다.
			/// </summary>
			/// <param name="rw">가로의 상대적 길이</param>
			/// <param name="rh">세로의 상대적 길이</param>
			inline void setRatio(unsigned rw, unsigned rh) {
				R_WIDTH = rw;
				R_HEIGHT = rh;
				ratio = (float)rw / rh;
			}
			/// <summary>
			/// 설정된 종횡비를 리턴합니다.
			/// </summary>
			inline float getRatio() { return ratio; }

			/// <summary>
			/// 2D 그래픽을 렌더링하는 경우 사용하는 종횡비 행렬을 다시 정합니다. 음의 값을 입력하면 안 됩니다.
			/// </summary>
			void setProjMatrix2D();

			/// <summary>
			/// 3D 그래픽을 렌더링하는 경우 사용하는 프로젝션 행렬을 다시 정합니다. 음의 값을 입력하면 안 됩니다.
			/// </summary>
			/// <param name="fovy">카메라의 상하 시야각입니다.</param>
			/// <param name="dnear">볼 수 있는 가장 가까운 거리입니다.</param>
			/// <param name="dfar">볼 수 있는 가장 먼 거리입니다.</param>
			void setProjMatrix3D(float fovy, float dnear, float dfar);
			/// <summary>
			/// 3D 그래픽을 렌더링하는 경우 사용하는 프로젝션 행렬에서 상하 시야각만 변경합니다. 음의 값을 입력하면 안 됩니다.
			/// </summary>
			/// <param name="fovy">카메라의 상하 시야각입니다.</param>
			void setFovy(float fovy);
			/// <summary>
			/// 3D 그래픽을 렌더링하는 경우 사용하는 프로젝션 행렬에서 볼 수 있는 최단거리만 변경합니다. 음의 값을 입력하면 안 됩니다.
			/// </summary>
			/// <param name="near">볼 수 있는 가장 가까운 거리입니다.</param>
			void setNear(float near);
			/// <summary>
			/// 3D 그래픽을 렌더링하는 경우 사용하는 프로젝션 행렬에서 볼 수 있는 최장거리만 변경합니다. 음의 값을 입력하면 안 됩니다.
			/// </summary>
			/// <param name="far">볼 수 있는 가장 먼 거리입니다.</param>
			void setFar(float far);
		} ratio;
	private:
		vec3 fixedAt = { 0,0,-1 }, up = { 0,1,0 };
		vec3 relativePos = { 0,0,1 };
		vec3 currentPos;
		float delay = 0;
		const vec3* at = nullptr;
		mat4 viewM4;	// UI(program2)와 월드(program3)의 연결점. 카메라는 하나만 있을 것이므로 메모리 문제는 거의 없다고 봐도 무방
		float zoom = 1;	// //
		bool fixdir = false;
	};
}

#endif // !__OA_CAMERA_H__
