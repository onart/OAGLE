/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_BALLPOOL_H__
#define __OA_BALLPOOL_H__

#include "oaglem.h"

namespace onart {
	class BallCollider2D;
	class Ballpool2D {
	public:
		/// <summary>
		/// 2D 물리 개체를 업데이트합니다.
		/// </summary>
		static void Update();
		/// <summary>
		/// 디버그를 위해 원형 충돌체를 화면에 반투명하게 표시합니다.
		/// </summary>
		static void render();
	private:
		/// <summary>
		/// 2D 물리 개체의 충돌을 확인하고 판정을 가합니다.
		/// </summary>
		static void makeCollisions();
		/// <summary>
		/// 2D 물리 개체의 충돌을 확인하고 판정을 가합니다.
		/// </summary>
		/// <param name="b1"></param>
		/// <param name="b2"></param>
		/// <param name="posvel"></param>
		static void collide(BallCollider2D* b1, BallCollider2D* b2, const vec4& posvel);
		/// <summary>
		/// 2D 물리 개체의 겹침을 해소합니다. 질량에 반비례하게 서로 옮깁니다.
		/// </summary>
		static void resolveOverlap(BallCollider2D* b1, BallCollider2D* b2, float dist, const vec4& posvel);
	};
}

#endif // !__OA_BALLPOOL_H__

