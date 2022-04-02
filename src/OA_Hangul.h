/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_HANGUL_H__
#define __OA_HANGUL_H__

#include <string>
#include <stack>

namespace onart {
	/// <summary>
	/// 한글 입력을 위한 상태 머신입니다.
	/// </summary>
	class HangulStateMachine
	{
	public:
		inline HangulStateMachine(const std::u16string& content = u"") :content(content), cursor(content.size()), st(hState::EMPTY) {}
		inline std::u16string getContent() { return content; }
		void flush(const std::u16string& content = u"");
		/// <summary>
		/// 한 글자를 입력한 결과가 반영됩니다. 0을 입력하는 경우 현재 입력 중인 한글 낱자의 입력을 종료합니다.
		/// </summary>
		void push(char16_t c = 0);
		/// <summary>
		/// 문자열을 현재 커서 위치에 삽입합니다.
		/// </summary>
		void append(const std::u16string&);
		/// <summary>
		/// 현재 위치에서 커서를 이동합니다.
		/// </summary>
		void moveCursor(ptrdiff_t);
		/// <summary>
		/// 커서를 맨 처음 위치로 이동합니다.
		/// </summary>
		void home();
		/// <summary>
		/// 커서를 맨 끝 위치로 이동합니다.
		/// </summary>
		void end();
	private:
		enum class hState { EMPTY, R, K, ML, RT, RK, RML, RKR, RKFR, RMLR, RMLFR } st;   // 대표 글자로 각각 공백,ㄱ,ㅏ,ㅢ,ㄳ,가,긔,각,갉,긕,긝과 동상 글자를 뜻함
		void clearBuffers();
		std::u16string content;
		std::stack<char16_t> prev, buf;
		size_t cursor;
	};
}

#endif // !__OA_HANGUL_H__
