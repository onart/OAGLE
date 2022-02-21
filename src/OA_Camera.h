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
	class Camera
	{
	public:
		/// <summary>
		/// ī�޶� 3���� ���͸� ���󰩴ϴ�. nullptr�� �����ϴ� ��� ī�޶� ���� ������ ��ġ���� ���� ����������,
		/// ī�޶� ���󰡴� ���� �Ҵ�� vec3 ��ü�� ������ ��� ���α׷��� ������ �����ϼ���.
		/// </summary>
		/// <param name="relativePos">ī�޶� �� ������ �������� �� ī�޶��� ��ġ�Դϴ�.</param>
		/// <param name="at">ī�޶� ���� �����Դϴ�.</param>
		inline void follow(const vec3& relativePos, const vec3* at = nullptr) { 
			this->relativePos = relativePos; 
			if (!at) {
				fixedAt = *(this->at);
			}
			this->at = at; 
		}
		/// <summary>
		/// ī�޶� ���ϴ� ��ġ�� ������ �����մϴ�.
		/// </summary>
		/// <param name="relativePos">ī�޶� �� ������ ����(0,0,0)���� �� ī�޶��� ��ġ�Դϴ�.</param>
		/// <param name="fixedAt">ī�޶� �� �����Դϴ�.</param>
		inline void fix(const vec3& relativePos, const vec3& fixedAt) { at = nullptr; this->fixedAt = fixedAt; this->relativePos = relativePos; }

		/// <summary>
		/// ī�޶��� ������ �����̸� �����մϴ�.
		/// </summary>
		/// <param name="s">�������� ���� �����Դϴ�. ī�޶� eye�� ��ġ���� ������(at�� ������ ����, ī�޶� �̵��ؾ� �� �Ÿ��� 99%) �̵��ϴ� �� �ɸ��� �ð��� �ǹ��մϴ�. ���� ���� ���� ��� ������ ���� ������ ��ȯ�˴ϴ�.</param>
		void setDelay(float s);
		
		/// <summary>
		/// ī�޶� ���� �� ����� ������Ʈ�Ͽ� ���̴��� �ѱ�ϴ�. FPS�� 20 �̻��� ��� ī�޶��� �������� ũ�� ���� ���� �ʽ��ϴ�.
		/// </summary>
		void viewUpdate();

		/// <summary>
		/// ī�޶��� �� ������ ���մϴ�. �� ī�޶� ǥ�õǴ� ������ ������ �����մϴ�.
		/// </summary>
		void setZoom(float zoom);

		/// <summary>
		/// Input::relativeCursorPos()�� ����Ʈ ���� ����� (0,0), ���� �ϴ��� (1,1)�� �ǵ��� �����ϴµ� �̸� ���α׷� ������ ���Ǵ� ǥ�� �� ��ǥ(�߽��� (0,0)�̸� ��/Ⱦ �� ª�� ���� ���̰� 2)�� ��ȯ�մϴ�.
		/// Ŀ���� ����Ʈ ������ �������� Ư���� ������� �������� �ʽ��ϴ�.
		/// </summary>
		/// <param name="mousePos">Input::relativeCursorPos()���� ���� ��</param>
		vec2 mouse2screen(const vec2& mousePos);

		/// <summary>
		/// ī�޶� �̵��� �� ���� ������ �����մϴ�. Ư�� 2D ���ӿ��� ī�޶� �����̸� 0���� ũ�� ������ ��� �ʼ��Դϴ�.
		/// </summary>
		/// <param name="fix">���� ����(false�� ���ϸ� ������ �����մϴ�.)</param>
		inline void fixDirection(bool fix = true) { fixdir = fix; }
		
		/// <summary>
		/// ���� �� ��ǥ�� ȭ��󿡼� ��� ��ġ�� �������� �˷��ݴϴ�. ����Ʈ�� �߽��� (0,0)�̸� ��/Ⱦ �� ª�� ���� ���̰� 2�Դϴ�.
		/// </summary>
		inline vec3 world2screen(const vec3& pos) { return zoom * ratio.projM4 * viewM4 * vec4(pos, 1.0f); }
		/// <summary>
		/// ����Ʈ�� ������ �����մϴ�.
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
			/// ��Ⱦ�� �����մϴ�.
			/// </summary>
			/// <param name="rw">������ ����� ����</param>
			/// <param name="rh">������ ����� ����</param>
			inline void setRatio(unsigned rw, unsigned rh) {
				R_WIDTH = rw;
				R_HEIGHT = rh;
				ratio = (float)rw / rh;
			}
			/// <summary>
			/// ������ ��Ⱦ�� �����մϴ�.
			/// </summary>
			inline float getRatio() { return ratio; }

			/// <summary>
			/// 2D �׷����� �������ϴ� ��� ����ϴ� ��Ⱦ�� ����� �ٽ� ���մϴ�. ���� ���� �Է��ϸ� �� �˴ϴ�.
			/// </summary>
			void setProjMatrix2D();

			/// <summary>
			/// 3D �׷����� �������ϴ� ��� ����ϴ� �������� ����� �ٽ� ���մϴ�. ���� ���� �Է��ϸ� �� �˴ϴ�.
			/// </summary>
			/// <param name="fovy">ī�޶��� ���� �þ߰��Դϴ�.</param>
			/// <param name="dnear">�� �� �ִ� ���� ����� �Ÿ��Դϴ�.</param>
			/// <param name="dfar">�� �� �ִ� ���� �� �Ÿ��Դϴ�.</param>
			void setProjMatrix3D(float fovy, float dnear, float dfar);
			/// <summary>
			/// 3D �׷����� �������ϴ� ��� ����ϴ� �������� ��Ŀ��� ���� �þ߰��� �����մϴ�. ���� ���� �Է��ϸ� �� �˴ϴ�.
			/// </summary>
			/// <param name="fovy">ī�޶��� ���� �þ߰��Դϴ�.</param>
			void setFovy(float fovy);
			/// <summary>
			/// 3D �׷����� �������ϴ� ��� ����ϴ� �������� ��Ŀ��� �� �� �ִ� �ִܰŸ��� �����մϴ�. ���� ���� �Է��ϸ� �� �˴ϴ�.
			/// </summary>
			/// <param name="near">�� �� �ִ� ���� ����� �Ÿ��Դϴ�.</param>
			void setNear(float near);
			/// <summary>
			/// 3D �׷����� �������ϴ� ��� ����ϴ� �������� ��Ŀ��� �� �� �ִ� ����Ÿ��� �����մϴ�. ���� ���� �Է��ϸ� �� �˴ϴ�.
			/// </summary>
			/// <param name="far">�� �� �ִ� ���� �� �Ÿ��Դϴ�.</param>
			void setFar(float far);
		} ratio;
	private:
		vec3 fixedAt = { 0,0,-1 }, up = { 0,1,0 };
		vec3 relativePos = { 0,0,1 };
		vec3 currentPos;
		float delay = 0;
		const vec3* at = nullptr;
		mat4 viewM4;	// UI(program2)�� ����(program3)�� ������. ī�޶�� �ϳ��� ���� ���̹Ƿ� �޸� ������ ���� ���ٰ� ���� ����
		float zoom = 1;	// //
		bool fixdir = false;
	};
}

#endif // !__OA_CAMERA_H__
