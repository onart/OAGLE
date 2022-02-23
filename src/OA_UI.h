#ifndef __OA_UI_H__
#define __OA_UI_H__
#include "OA_Entity.h"
#include "OA_Text.h"

namespace onart {
	class Font;
	namespace UI {
		/// <summary>
		/// 텍스트 개체입니다. 글자별로 크기/색상을 조정할 수 있습니다.
		/// <para>크기: 크기는 직사각형에 맞도록 나오는데, 텍스트 안에서 상대적 크기를 다르게 설정하고자 한다면 \a와 늘임축(x 또는 y 또는 a. 대문자는 인정하지 않습니다.), 그리고 정수 부분 1자리 소수 부분 2자리 실수를 입력합니다(즉, 최대 상대 크기는 9.99/0.01=999배). 예를 들어,
		/// u"안녕\ax2.00하세\ay0.40요"라고 입력하면 "안녕"의 각 글자가 크기 1x1이라고 할 때 "하세"의 각 글자 크기는 2x1, "요"의 크기는 2x0.4가 됩니다. \a 뒤 5자리가 유효한 값이 아닌 경우 글자 크기는 변하지 않으며
		/// 5자리만큼은 무시됩니다. a축은 x,y축 모두를 설정합니다.</para>
		/// <para>색: 기본적으로 백색이며, content 내용에서 \b와 16진 코드(RGBA 순)를 이용하여 색을 변경할 수 있습니다. 예를 들어, u"안녕\bff0000ff하세\b00ff00ff요"라고 입력하면
		/// "안녕"은 하양, "하세"는 빨강, "요"는 초록색으로 출력됩니다. 만약 \b 뒤 8자리가 유효하지 않은 값을 가지는 경우 색은 변하지 않으며 값들은 무시합니다.</para>
		/// <para>줄바꿈: \n은 줄을 바꿉니다.</para>
		/// </summary>
		class Text: public Entity {
		public:
			/// <summary>
			/// 텍스트 개체를 생성합니다. 특정 직사각형 범위에 고정됩니다. 이를 테면 고정 UI에 사용하기에 적합합니다.
			/// 직사각형 범위에 고정되기 때문에 수직 정렬은 제공되지 않습니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용될 텍스트 개체의 이름입니다.</param>
			/// <param name="font">텍스트의 모양입니다. 파일 혹은 메모리에서 불러올 수 있습니다.</param>
			/// <param name="content">텍스트의 내용 및 글자별 크기/색상입니다. </param>
			/// <param name="ldwh">텍스트를 이 직사각형(L-D-W-H)에 맞게 출력합니다.</param>
			/// <param name="maxWidth">최대 폭입니다. 직사각형과 동일한 단위가 아닌, 불러온 폰트에 대한 픽셀 단위입니다. 직접 조절하면서 영역을 맞추는 것이 강하게 권장됩니다.</param>
			/// <param name="fullFit">true인 경우 주어진 직사각형에 정확히 맞게 출력합니다. 즉, 글자의 원본 비율을 유지하지 않습니다.</param>
			/// <param name="align">수평 정렬 상태입니다. 가운데/왼쪽/오른쪽 옵션만 가능합니다.</param>
			/// <param name="rowGap">행 간격입니다. 문서 프로그램들과 동일한 단위로 제공되는 것이 아니므로 직접 조절해 주시기 바랍니다.</param>
			/// <param name="color">전체의 색입니다.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec4& ldwh, float maxWidth = INF, bool fullFit = false, AlignH align = AlignH::CENTER, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// 텍스트 개체를 생성합니다. 직사각형 범위에 고정되지 않고 특정 지점을 중심으로 합니다.
			/// 이를 테면 게임 상 물체에서 텍스트가 생산될 때 사용하기에 적합합니다. 이 경우 Camera::world2screen() 함수를 참고해 주세요.
			/// 게임의 대화창과 같이 텍스트의 크기가 실시간으로 변하면 어색한 부분에도 사용하기 적합합니다. 이 경우 제한된 범위에 글자를 맞추는 기능은 매개변수 size와 maxWidth뿐입니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용될 텍스트 개체의 이름입니다.</param>
			/// <param name="font">텍스트의 모양입니다. 파일 혹은 메모리에서 불러올 수 있습니다.</param>
			/// <param name="content">텍스트의 내용 및 글자별/크기/색상입니다.</param>
			/// <param name="center">중심이 되는 좌표입니다. 게임 화면에서 가운데가 (0,0)이고 종횡 중 짧은 길이가 2입니다. center의 의미는 매개변수 ha, va에 따라서 달라집니다.</param>
			/// <param name="maxWidth">최대 폭입니다. 직사각형과 동일한 단위가 아닌, 불러온 폰트에 대한 픽셀 단위입니다. 직접 조절하면서 영역을 맞추는 것이 강하게 권장됩니다.</param>
			/// <param name="ha">수평 정렬 상태입니다. 가운데/왼쪽/오른쪽 옵션만 가능합니다. 특성상 여러 행으로 구성된 텍스트가 아닌 경우 가운데 정렬과 동일합니다.</param>
			/// <param name="va">수직 정렬 상태입니다. 가운데/위쪽/아래쪽 옵션만 가능합니다.</param>
			/// <param name="size">텍스트의 크기입니다. 다른 폰트라면 같은 값이 같은 크기로 보장되지 않습니다.</param>
			/// <param name="rowGap">행 간격입니다. 문서 프로그램들과 동일한 단위로 제공되는 것이 아니므로 직접 조절해 주시기 바랍니다.</param>
			/// <param name="color">전체의 색입니다.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec2& center, float maxWidth = INF, AlignH ha = AlignH::CENTER, AlignV va = AlignV::CENTER, float size = 1, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// 주어진 문자열로 내용을 바꿉니다. 글자별 크기/색상 옵션은 생성자를 참조하세요.
			/// </summary>
			void setContent(const oastring&);
			/// <summary>
			/// 주어진 문자열을 내용에 덧붙입니다.
			/// </summary>
			/// <param name=""></param>
			void append(const oastring&);
			/// <summary>
			/// 정렬 기준을 변경합니다. 가운데/왼쪽/오른쪽만 가능합니다.
			/// </summary>
			void setAlign(AlignH);
			void render();
			/// <summary>
			/// 다른 불러온 폰트로 변경합니다.
			/// </summary>
			inline void setFont(Font* font) { this->font = font; }
			/// <summary>
			/// 문자열 전체의 색을 바꿉니다. 글자별 색상과는 곱으로 적용됩니다.
			/// </summary>
			inline void setColor(const vec4& color) { this->color = color; }
			/// <summary>
			/// 중심 위치를 고려하여 회전을 설정합니다. 기존에 있던 다른 변환은 덮어씁니다.
			/// </summary>
			void setRotation(const Quaternion&);
			/// <summary>
			/// 추가 변환을 적용합니다. 기존에 있던 다른 변환은 덮어씁니다.
			/// </summary>
			void setAdditinoalTransform(const mat4&);
			/// <summary>
			/// 크기/색상 변경의 확장열을 제외한 텍스트를 얻습니다.
			/// </summary>
			inline oastring getPlain() { return plain; }
			/// <summary>
			/// 자동 줄바꿈이 반영된 텍스트 행 수를 리턴합니다. 최솟값은 1입니다.
			/// </summary>
			inline size_t nLines() { return linexy.size(); }
		private:
			Font* font;
			oastring content, plain, input;
			vec4 color;	// 전체 색
			vec4 ldwh;	// 문장 기본 크기
			vec4 targRect;
			vec2 center;	// 최종 중심
			mat4 r2r;	// 직사각형 고정 전용
			mat4 additionalTransform;
			std::vector<vec2> linexy;
			AlignH halign;	AlignV valign;
			float rowGap;
			float size;
			const float maxWidth;
			bool rectFixed, fullFit, hasAdditional = false;
		};
	}
}

#endif // !__OA_UI_H__

