/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_UNIVERSAL_FUNCTOR_H__
#define __OA_UNIVERSAL_FUNCTOR_H__

#include<cstdarg>

namespace onart {
	/// <summary>
	/// 이 프로그램에서 사용될 모든 펑터의 형태입니다. 매개변수는 기본 void* 한 개이며 사전 약속이 된 기준 원하는 만큼 사용할 수 있습니다.
	/// 기본값이 nullptr로 존재하므로 매개변수가 없는 경우에도 사용 가능합니다.
	/// 다형성을 활용해야 하는 특성상 참조자를 씁니다. 즉, 반드시 사용 범위 내에서 소멸하지 않는 객체를 사용해 주세요.
	/// </summary>
	class UniversalFunctor
	{
	public:
		virtual void operator()(void* usr = nullptr, ...) = 0;
	};
}

#endif // !__OA_UNIVERSAL_FUNCTOR_H__

