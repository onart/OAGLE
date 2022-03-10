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
	/// �ѱ� �Է��� ���� ���� �ӽ��Դϴ�.
	/// </summary>
	class HangulStateMachine
	{
	public:
		inline HangulStateMachine(const std::u16string& content = u"") :content(content), cursor(content.size()), st(hState::EMPTY) {}
		inline std::u16string getContent() { return content; }
		void flush(const std::u16string& content = u"");
		/// <summary>
		/// �� ���ڸ� �Է��� ����� �ݿ��˴ϴ�. 0�� �Է��ϴ� ��� ���� �Է� ���� �ѱ� ������ �Է��� �����մϴ�.
		/// </summary>
		void push(char16_t c = 0);
		/// <summary>
		/// ���ڿ��� ���� Ŀ�� ��ġ�� �����մϴ�.
		/// </summary>
		void append(const std::u16string&);
		/// <summary>
		/// ���� ��ġ���� Ŀ���� �̵��մϴ�.
		/// </summary>
		void moveCursor(ptrdiff_t);
		/// <summary>
		/// Ŀ���� �� ó�� ��ġ�� �̵��մϴ�.
		/// </summary>
		void home();
		/// <summary>
		/// Ŀ���� �� �� ��ġ�� �̵��մϴ�.
		/// </summary>
		void end();
	private:
		enum class hState { EMPTY, R, K, ML, RT, RK, RML, RKR, RKFR, RMLR, RMLFR } st;   // ��ǥ ���ڷ� ���� ����,��,��,��,��,��,��,��,��,�S,�a�� ���� ���ڸ� ����
		void clearBuffers();
		std::u16string content;
		std::stack<char16_t> prev, buf;
		size_t cursor;
	};
}

#endif // !__OA_HANGUL_H__
