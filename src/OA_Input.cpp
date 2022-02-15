/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Input.h"

extern onart::ivec2 windowSize;
extern onart::ivec4 vp_ldwh;

namespace onart::Input {
	std::vector<KeyCode> allKeyInputsForNow() {
		std::vector<KeyCode> v;	v.reserve(6);
		for (int i = 32; i <= 96; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 256; i <= 301; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 320; i <= 347; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		return v;
	}

	vec2 relativeCursorPos() {
		vec2 rpos(mousePos);
		rpos.x -= vp_ldwh[0];	rpos.y -= vp_ldwh[1];					// 뷰포트 좌측상단 좌표를 (0,0)으로 하도록 평행이동 (일반적인 화면 크기 범위에서는 절대 오차가 생기지 않음)
		rpos.x /= vp_ldwh[2];	rpos.y /= vp_ldwh[3];					// 뷰포트 우측하단 좌표를 (1,1)로 하도록 수축
		return rpos;
	}
}
