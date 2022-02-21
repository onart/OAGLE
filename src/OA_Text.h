/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_TEXT_H__
#define __OA_TEXT_H__

#include <vector>
#include <string>
#include <set>
#include <map>
#include "oaglem.h"

#ifndef OA_NO_UNICODE16
using oastring = std::u16string;
using oachar = char16_t;
#else
using oastring = std::string;
using oachar = char;
#endif

/// <summary>
/// 이 모듈은 기본적으로 UTF-16 문자열, 즉 std::u16string을 사용할 것입니다. 다시 말해, 아스키 범위 내/외의 문자 모두 스트링에서 한 자리를 차지합니다.
/// 리터럴은 u"문자열" 혹은 u'문'이니, 형 변환 컴파일 오류 시 참고해 주세요.
/// OA_NO_UNICODE16 매크로를 정의하면 std::string을 사용할 수 있습니다. 단, 이때 아스키 범위 외 문자가 존재하는 경우 텍스트 출력은 원하는 대로 되지 않을 것입니다.
/// </summary>
namespace onart {
	/// <summary>
	/// 트루타입만 사용 가능합니다.
	/// </summary>
	class Font
	{
	public:
		enum class Align { CENTER, LEFT, RIGHT };
		/// <summary>
		/// 폰트 파일을 불러옵니다. 불러온 파일은 이름을 통해서 접근할 수 있습니다. 이 프로그램에서 사용하는 모듈인 stb_truetype은 파일 길이를 받지 않습니다.
		/// 따라서, 앞 헤더 부분은 잘 꾸며졌지만 실제로 유효한 트루타입 파일이 아닌 경우 문제가 발생할 수 있으므로 사용하지 마시기 바랍니다.
		/// </summary>
		/// <param name="file">불러올 파일 이름입니다.</param>
		/// <param name="v">실제로 사용할 문자의 집합입니다. 겹치는 게 있어도 여러 번 로드하지 않으니 모든 스트링을 그대로 합쳐서 넣어도 됩니다.</param>
		/// <param name="resolution">텍스처 비트맵의 해상도입니다. 값이 클수록 글씨가 더 매끄럽게 출력되겠지만 그만큼 그래픽 용량을 많이 차지하고 느려지므로 필요한 만큼만 사용하는 것이 좋습니다.</param>
		/// <param name="name">프로그램 내에서 사용할 이름입니다. 입력하지 않은 경우 파일 이름을 사용합니다.</param>
		static Font* load(const std::string& file, const std::vector<oachar>& v, float resolution = 48, const std::string& name = "");
		/// <summary>
		/// 폰트 파일에 해당하는 데이터를 메모리에서 불러옵니다. 불러온 파일은 이름을 통해서 접근할 수 있습니다. 이 프로그램에서 사용하는 모듈인 stb_truetype은 파일 길이를 받지 않습니다.
		/// 따라서, 앞 헤더 부분은 잘 꾸며졌지만 실제로 유효한 트루타입 파일이 아닌 경우 문제가 발생할 수 있으므로 사용하지 마시기 바랍니다.
		/// </summary>
		/// <param name="dat">메모리 시작 주소</param>
		/// <param name="name">프로그램 내에서 사용할 이름입니다.</param>
		/// <param name="v">실제로 사용할 문자의 집합입니다. 겹치는 게 있어도 여러 번 로드하지 않으니 모든 스트링을 그대로 합쳐서 넣어도 됩니다.</param>
		/// <param name="resolution">텍스처 비트맵의 해상도입니다. 값이 클수록 글씨가 더 매끄럽게 출력되겠지만 그만큼 그래픽 용량을 많이 차지하고 느려지므로 필요한 만큼만 사용하는 것이 좋습니다.</param>
		static Font* load(const unsigned char* dat, const std::string& name, const std::vector<oachar>& v, float resolution = 48);
		/// <summary>
		/// 불러온 폰트 파일에 대한 것을 메모리에서 제거합니다.
		/// </summary>
		/// <param name="name">제거할 이름</param>
		static void drop(const std::string& name);
		/// <summary>
		/// 폰트 파일의 포인터를 리턴합니다.
		/// </summary>
		static Font* get(const std::string& name);
		/// <summary>
		/// 주어진 영역에 텍스트를 그립니다.
		/// <para>크기: 크기는 직사각형에 맞도록 나오는데, 텍스트 안에서 상대적 크기를 다르게 설정하고자 한다면 \a와 늘임축(x 또는 y 또는 a), 그리고 정수 부분 1자리 소수 부분 2자리 실수를 입력합니다(즉, 최대 상대 크기는 9.99/0.01=999배). 예를 들어,
		/// u"안녕\ax2.00하세\ay0.40요"라고 입력하면 "안녕"의 각 글자가 크기 1x1이라고 할 때 "하세"의 각 글자 크기는 2x1, "요"의 크기는 2x0.4가 됩니다. \a 뒤 5자리가 유효한 값이 아닌 경우 글자 크기는 변하지 않으며
		/// 5자리만큼은 무시됩니다. a축은 x,y축 모두를 설정합니다.</para>
		/// <para>색: 기본적으로 백색이며, content 내용에서 \b와 16진 코드(RGBA 순)를 이용하여 색을 변경할 수 있습니다. 예를 들어, u"안녕\bff0000ff하세\b00ff00ff요"라고 입력하면
		/// "안녕"은 하양, "하세"는 빨강, "요"는 초록색으로 출력됩니다. 만약 \b 뒤 8자리가 유효하지 않은 값을 가지는 경우 색은 변하지 않으며 값들은 무시합니다.</para>
		/// <para>줄바꿈: \n은 줄을 바꿉니다. 그 외 자동 줄바꿈은 현재 버전에서 제공하지 않습니다. 나중에 단위글자 n개 정도의 기준으로 제공될 예정입니다.</para>
		/// </summary>
		/// <param name="content">표시할 문장입니다.</param>
		/// <param name="group">전체 문자열의 위치는 픽셀 단위로 다뤄지는데, 이를 원하는 위치/크기로 변환하는 행렬입니다. getRectNLine()에서 자동으로 계산됩니다.</param>
		/// <param name="lineXY">각 라인의 시작점입니다. getRectNLine()에서 자동으로 계산됩니다.</param>
		void draw(const oastring& content, const mat4& group, const std::vector<vec2>& lineXY, bool fullFit = false);
		/// <summary>
		/// 주어진 위치를 중심으로 고정된 크기의 텍스트를 그립니다. 텍스트는 반드시 가운데 정렬됩니다.
		/// <para>크기: 크기는 직사각형에 맞도록 나오는데, 텍스트 안에서 상대적 크기를 다르게 설정하고자 한다면 \a와 늘임축(x 또는 y 또는 a), 그리고 정수 부분 1자리 소수 부분 2자리 실수를 입력합니다. 
		/// 상대적 크기의 범위가 변하더라도 기본값인 1.0의 크기는 변하지 않습니다. 예를 들어,
		/// u"안녕\ax2.00하세\ay0.40요"라고 입력하면 "안녕"의 각 글자가 크기 1x1이라고 할 때 "하세"의 각 글자 크기는 2x1, "요"의 크기는 2x0.4가 됩니다. \a 뒤 5자리가 유효한 값이 아닌 경우 글자 크기는 변하지 않으며
		/// 5자리만큼은 무시됩니다. a축은 x,y축 모두를 설정합니다.</para>
		/// <para>색: 기본적으로 백색이며, content 내용에서 \b와 16진 코드(RGBA 순)를 이용하여 색을 변경할 수 있습니다. 예를 들어, u"안녕\bff0000ff하세\b00ff00ff요"라고 입력하면
		/// "안녕"은 하양, "하세"는 빨강, "요"는 초록색으로 출력됩니다. 만약 \b 뒤 8자리가 유효하지 않은 값을 가지는 경우 색은 변하지 않으며 값들은 무시합니다.</para>
		/// <para>줄바꿈: \n은 줄을 바꿉니다. 그 외 자동 줄바꿈은 현재 버전에서 제공하지 않습니다. 나중에 단위글자 n개 정도의 기준으로 제공될 예정입니다.</para>
		/// </summary>
		/// <param name="content">표시할 문장입니다.</param>
		/// <param name="center">중심 좌표입니다.</param>
		/// <param name="size">글자의 크기입니다.</param>
		/// <param name="rowGap">행간을 설정합니다. 기본값은 1.0입니다.</param>
		void draw(const oastring& content, const vec2& center, float size = 1, float rowGap = 1);

		/// <summary>
		/// 이 불러온 폰트에 대하여 해당 문장이 차지하는 상대적 공간, 그리고 각 행은 어디서 시작할지를 리턴합니다. 이 둘은 draw()에서 사용됩니다.
		/// 글자 색 및 크기 변경에 대해서는 draw() 설명에서 참고해 주세요.
		/// </summary>
		/// <param name="content">측정될 문장</param>
		/// <param name="lineXY">각 행의 시작점(가장 왼쪽/아랫선)이 들어갑니다. 비어 있지 않았던 경우 자동으로 비우고 새로 들어갑니다.</param>
		/// <param name="align">글자 정렬 상태입니다. 가운데/왼쪽/오른쪽이 있습니다.</param>
		/// <param name="rowGap">행간을 설정합니다. 기본값은 1입니다.</param>
		/// <returns>문장이 차지하는 직사각형입니다. draw()에서 사용됩니다.</returns>
		vec4 getRectNLine(const oastring& content, std::vector<vec2>& lineXY, Align align = Align::CENTER, float rowGap = 1);
	private:
		struct charTex {
			unsigned id;
			float size[2];
			float bearing[2];
			float advance;
		};
		Font(void* info, const std::set<oachar>& vset, float resolution);
		~Font();
		std::map<oachar, charTex> txs;
		const float resolution;
		static std::map<std::string, Font*> list;

		static float parseSize(const oastring&, int start);
		static vec4 parseColor(const oastring&, int start);
	};

}

#endif // !__OA_TEXT_H__
