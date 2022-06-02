/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_FRAMEBUFFER_H__
#define __OA_FRAMEBUFFER_H__

#include "oaglem.h"
#include <map>

namespace onart {
	/// <summary>
	/// 화면 외에 무언가 그릴 프레임버퍼를 생성합니다.
	/// 프레임버퍼는 이름을 통해서 접근/제거하는데, string::c_str()과 같이 동적인 문자열은 인정될 수 없으니 주의하시기 바랍니다.
	/// </summary>
	class FrameBuffer {
	public:
		/// <summary>
		/// 프레임버퍼 첨부물의 속성을 결정합니다.
		/// NONE: 첨부물을 사용하지 않습니다. 모든 값이 NONE일 수 없습니다.
		/// RW: 텍스처를 사용하며, 그린 데이터를 읽어올 수 있습니다.
		/// W: 렌더버퍼를 사용하며, 그린 데이터를 읽어올 수 없습니다. 일반적으로 텍스처보다 빠르므로 사용하되 직접 읽을 일이 없는 경우 W를 사용합니다.
		/// </summary>
		enum class ATTACHMENT { NONE, RW, W };
		/// <summary>
		/// 프레임버퍼를 생성합니다. 이름이 겹치는 경우 아무것도 하지 않습니다.
		/// 프레임버퍼를 생성한 직후에는 자동으로 기본 프레임버퍼가 바인드되므로 혼란을 피하려면 필요한 프레임버퍼는 시작할 때 바로 만드시기 바랍니다.
		/// 스텐실 버퍼 첨부물은 용도가 한정적이므로, 사용할 경우 반드시 깊이 버퍼와 붙은 상태로만 사용되며 NONE이 아닌 옵션이 주어진 경우 depthBuffer에 주어진 것과 동일하게 주어집니다.
		/// 단, 깊이 버퍼가 NONE이고 스텐실 버퍼가 존재하는 것은 가능합니다.
		/// </summary>
		/// <param name="name">접근할 프레임버퍼 이름입니다.</param>
		/// <param name="width">프레임버퍼 폭입니다.</param>
		/// <param name="height">프레임버퍼 높이입니다.</param>
		/// <param name="colorBuffer">색 버퍼 사용 여부입니다.</param>
		/// <param name="depthBuffer">깊이 버퍼 사용 여부입니다.</param>
		/// <param name="stencilBuffer">스텐실 버퍼 사용 여부입니다.</param>
		/// <param name="func">glStencilFunc()의 인수(통과 조건)</param>
		/// <param name="ref">glStencilFunc()의 인수(비교 값)</param>
		/// <param name="mask">glStencilFunc()의 인수(유효 비트 지정)</param>
		/// <param name="sfail">glStencilOp()의 인수(스텐실 실패 시 동작)</param>
		/// <param name="dpfail">glStencilOp()의 인수(스텐실 통과, 깊이 실패 시 동작)</param>
		/// <param name="dppass">glStencilOp()의 인수(스텐실과 깊이 통과 시 동작)</param>
		static void make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer = ATTACHMENT::NONE, unsigned func = 0, int ref = 0, unsigned mask = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		/// <summary>
		/// 포인트 그림자를 위하여, 큐브맵 깊이 텍스처를 가진 프레임버퍼를 생성합니다.
		/// </summary>
		/// <param name="name">접근할 이름입니다.</param>
		/// <param name="width">프레임버퍼 폭입니다.</param>
		/// <param name="height">프레임버퍼 높이입니다.</param>
		static void makeCube(const char* name, unsigned width, unsigned height);
		/// <summary>
		/// 만든 프레임버퍼를 제거합니다.
		/// </summary>
		static void drop(const char* name);
		/// <summary>
		/// 그릴 프레임버퍼를 결정(바인드)하고 그것이 사용하는 버퍼를 클리어합니다. NULL 입력이 들어오는 경우 GLFW에서 생성한 기본 프레임버퍼, 즉 화면으로 전환됩니다.
		/// use() 호출 후에는 render 명령이 해당 프레임버퍼에 그리게 됩니다.
		/// </summary>
		static void use(const char* name = nullptr);
		/// <summary>
		/// 프레임버퍼 포인터를 얻어옵니다. 이것은 텍스처를 얻는 함수를 위해 나옵니다.
		/// </summary>
		inline static const FrameBuffer* get(const char* name) { return fbs[name]; }
		/// <summary>
		/// 현재 프레임 버퍼에서 색 버퍼의 값을 읽을 수 있는 경우 읽어 옵니다. 리턴값은 동적할당된 배열 주소입니다.
		/// 읽는 범위가 큰 경우 glGetTextureSubImage가 더 빠른 것으로 알려져 있습니다. 이는 기본적으로 GL 4.6을 사용하는 이 엔진의 기본 세팅에 의하면 사용 가능하지만
		/// 기본적으로 여기서는 지원하지 않습니다.
		/// 픽셀 좌측 하단의 좌표가 (0,0)입니다.
		/// 색 버퍼를 사용하지 않는 경우라도 검사하지 않으니 주의하세요.
		/// 색 데이터는 각 값마다 RGBA 형식으로 배열됩니다. 따라서 4차원 nvec 형식의 배열 캐스팅이 가능합니다.
		/// 단, 이 동적할당된 주소는 new에 의한 자유 공간 할당이므로 해제 시 주의하세요.
		/// 가능한 타입은 (unsigned) char, (unsigned) short, (unsigned) int, float입니다.
		/// 이것을 참고하세요. https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
		/// </summary>
		template <class T> static T* readColorValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// 현재 프레임 버퍼에서 깊이 버퍼의 값을 읽을 수 있는 경우 읽어 옵니다. 값은 0과 1 사이의 실수입니다.
		/// 픽셀 좌측 하단의 좌표가 (0,0)입니다.
		/// 리턴되는 동적할당된 주소는 new에 의한 자유 공간 할당이므로 해제 시 주의하세요.
		/// </summary>
		static float* readDepthValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// 현재 프레임 버퍼에서 깊이 버퍼의 값을 읽을 수 있는 경우 읽어 옵니다. 값은 0과 1 사이의 실수입니다.
		/// 픽셀 좌측 하단의 좌표가 (0,0)입니다.
		/// 리턴되는 동적할당된 주소는 new에 의한 자유 공간 할당이므로 해제 시 주의하세요.
		/// </summary>
		static unsigned char* readStencilValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// 이 프레임버퍼의 색상 텍스처 오브젝트를 리턴합니다.
		/// 없는 경우 0을 리턴합니다.
		/// </summary>
		inline unsigned getColorTex() const { return CDS[0] == ATTACHMENT::RW ? color : 0; }
		/// <summary>
		/// 이 프레임버퍼의 깊이 혹은 깊이+스텐실 텍스처 오브젝트를 리턴합니다.
		/// 없는 경우 0을 리턴합니다.
		/// </summary>
		inline unsigned getDepthTex() const { return CDS[1] == ATTACHMENT::RW ? depth : 0; }
		/// <summary>
		/// 이 프레임버퍼의 스텐실 텍스처 오브젝트를 리턴합니다.
		/// 깊이 텍스처가 없고 스텐실 텍스처가 있을 때에는 getDepthTex() 대신 이것을 사용해야 합니다.
		/// </summary>
		inline unsigned getStencilTex() const { return CDS[2] == ATTACHMENT::RW ? depth : 0; }
	private:
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth, ATTACHMENT stencil, unsigned func = 0, int ref = 0, unsigned mask = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth);
		FrameBuffer(unsigned width, unsigned height);
		~FrameBuffer();

		void setViewport();

		static std::map<const char*, FrameBuffer*> fbs;

		const unsigned width, height;
		const ATTACHMENT CDS[3];
		unsigned fbo;
		unsigned color, depth;
		unsigned stFunc, stMask, sfail, dpfail, dppass;
		int stRef;
	};
}

#endif // !OA_FRAMEBUFFER_H__

