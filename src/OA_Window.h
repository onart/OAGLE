﻿/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_WINDOW_H__
#define __OA_WINDOW_H__

#include <memory>
#include "oaglem.h"

namespace onart {
	class Animation;
	namespace window {
		/// <summary>
		/// 커서 이미지 또는 애니메이션을 설정합니다. UIAnimation 또는 FixedSprite만 가능합니다. 그 외의 애니메이션 또는 nullptr가 전달되는 경우 기본 커서로 돌아옵니다.
		/// GLFW 자체에서도 커서 이미지를 바꾸는 기능을 제공하지만, 이 함수는 단일 애니메이션을 설정할 수 있다는 차이가 있습니다. 마우스 커서의 클릭 판정은 물론 애니메이션 피벗에 있습니다.
		/// </summary>
		void setCursorImage(const std::shared_ptr<Animation>& p = std::shared_ptr<Animation>(), const vec2& scale = 1);
		/// <summary>
		/// 창의 이름을 변경합니다. (공사중)
		/// </summary>
		void setWindowTitle(const char*);
	}
}

#endif // !__OA_WINDOW_H__

