/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_PHYSICALSURFACE_H__
#define __OA_PHYSICALSURFACE_H__

namespace onart {
	// 정지와 이동 마찰, 반발값 정의
	enum class PHYSICAL_SURFACE { DEFAULT_SURFACE, NEVERUSETHIS };
	constexpr size_t NTYPE = (size_t)PHYSICAL_SURFACE::NEVERUSETHIS;

	constexpr float STATIC_FRICTIONS[NTYPE][NTYPE] = {	// 정지 마찰 계수
		{1}
	};
	constexpr float DYNAMIC_FRICTIONS[NTYPE][NTYPE] = {	// 운동 마찰 계수
		{1}
	};
	constexpr float RESTITUTIONS[NTYPE][NTYPE] = {	// 반발 계수
		{0}
	};
}

#endif // !__OA_PHYSICALSURFACE_H__
