/********************************************************************************
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
		/// Ŀ�� �̹��� �Ǵ� �ִϸ��̼��� �����մϴ�. UIAnimation �Ǵ� FixedSprite�� �����մϴ�. �� ���� �ִϸ��̼� �Ǵ� nullptr�� ���޵Ǵ� ��� �⺻ Ŀ���� ���ƿɴϴ�.
		/// GLFW ��ü������ Ŀ�� �̹����� �ٲٴ� ����� ����������, �� �Լ��� ���� �ִϸ��̼��� ������ �� �ִٴ� ���̰� �ֽ��ϴ�. ���콺 Ŀ���� Ŭ�� ������ ���� �ִϸ��̼� �ǹ��� �ֽ��ϴ�.
		/// </summary>
		void setCursorImage(const std::shared_ptr<Animation>& p = std::shared_ptr<Animation>(), const vec2& scale = 1);
		/// <summary>
		/// â�� �̸��� �����մϴ�. (������)
		/// </summary>
		void setWindowTitle(const char*);
	}
}

#endif // !__OA_WINDOW_H__
