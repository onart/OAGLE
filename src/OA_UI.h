/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_UI_H__
#define __OA_UI_H__
#include "OA_Entity.h"
#include "OA_Text.h"
#include "OA_UniversalFunctor.h"

namespace onart {
	class Font;
	class UIAnimation;
	struct Texture;

	/// <summary>
	/// 자주 사용하는 UI 개체를 모아 두었습니다. 이것은 상속하거나 코드를 수정해서 활용할 수 있으며 자체 제어는 응용 계층에서 UniversalFunctor를 만들어서 수행해 주세요.
	/// UI 개체는 기본적으로 마우스로 사용합니다. 이것의 위치가 겹치는 경우 z-index에 무관하게 모두 반응하며, 이에 대한 책임은 엔진이 지지 않습니다. Entity의 isActive 등을 통해 조절해 주세요.
	/// </summary>
	namespace UI {
		class UIEntity : public Entity {
			friend class Slider;
		public:
			inline UIEntity(const EntityKey& _1, const Transform& _2, bool _3 = true, bool _4 = false, bool _5 = false) : Entity(_1, _2, _3, _4, _5) { }
			inline UIEntity(const EntityKey& _1, const Transform& _2, pAnimation& _3, bool _4 = true, bool _5 = false, bool _6 = false) : Entity(_1, _2, _3, _4, _5, _6) {}
			/// <summary>
			/// 개체가 현재 판정이 가능한 상황인지 확인하기 위한 값입니다.
			/// </summary>
			virtual vec4 clickbox() { return vec4(-100, -100, 200, 200); }
		};

		/// <summary>
		/// 텍스트 개체입니다. 글자별로 크기/색상을 조정할 수 있습니다.
		/// <para>크기: 크기는 직사각형에 맞도록 나오는데, 텍스트 안에서 상대적 크기를 다르게 설정하고자 한다면 \a와 늘임축(x 또는 y 또는 a. 대문자는 인정하지 않습니다.), 그리고 정수 부분 1자리 소수 부분 2자리 실수를 입력합니다(즉, 최대 상대 크기는 9.99/0.01=999배). 예를 들어,
		/// u"안녕\ax2.00하세\ay0.40요"라고 입력하면 "안녕"의 각 글자가 크기 1x1이라고 할 때 "하세"의 각 글자 크기는 2x1, "요"의 크기는 2x0.4가 됩니다. \a 뒤 5자리가 유효한 값이 아닌 경우 글자 크기는 변하지 않으며
		/// 5자리만큼은 무시됩니다. a축은 x,y축 모두를 설정합니다.</para>
		/// <para>색: 기본적으로 백색이며, content 내용에서 \b와 16진 코드(RGBA 순)를 이용하여 색을 변경할 수 있습니다. 예를 들어, u"안녕\bff0000ff하세\b00ff00ff요"라고 입력하면
		/// "안녕"은 하양, "하세"는 빨강, "요"는 초록색으로 출력됩니다. 만약 \b 뒤 8자리가 유효하지 않은 값을 가지는 경우 색은 변하지 않으며 값들은 무시합니다.</para>
		/// <para>줄바꿈: \n은 줄을 바꿉니다.</para>
		/// </summary>
		class Text: public UIEntity {
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

		/// <summary>
		/// 마우스로 클릭할 수 있는 버튼 개체입니다.
		/// 기본적으로는 마우스 외에 반응을 하지 않지만 반응 함수(애니메이션 트리거)를 public으로 두어 씬에서 키보드로 접근할 수 있게 구현이 가능합니다.
		/// 애니메이션을 주지 않고 생성할 경우 SCTR 매개변수는 vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)으로 주어야 합니다.
		/// 버튼의 이미지로는 UIAnimation 또는 FixedSprite가 추천됩니다.
		/// </summary>
		class Button: public UIEntity
		{
		public:
			/// <summary>
			/// 버튼을 생성합니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용될 개체의 이름입니다.</param>
			/// <param name="baseSCTR">2D 스프라이트/애니메이션의 경우 이 값은 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다. 상속 또는 참고하여 setModel() 같이 해서 3D로 버튼을 만들려는 경우 명시적 지원은 예정에 없으므로 직접 조절해야 합니다. 앞 2차원은 x,y scale이며 뒤 2차원은 x,y 이동입니다.</param>
			/// <param name="ldwh">버튼이 사용할 직사각형 영역(LDWH)입니다. 직사각형이 아닌 영역을 인식 범위로 하고 싶다면 상속하여 Update()를 오버라이드해야 합니다.</param>
			/// <param name="onClick">버튼 클릭 시 반응 함수입니다. UniversalFunctor 추상 클래스를 상속하여 사용하며, Button 객체는 기본적으로 전달 인자가 없습니다. nullptr 전달 시 아무 행동도 하지 않습니다.</param>
			/// <param name="normal">기본 상태의 애니메이션(이미지)입니다. 입력하지 않으면 백색 직사각형이 렌더링됩니다.</param>
			/// <param name="onOver">마우스 오버 상태의 애니메이션(이미지)입니다. 입력하지 않으면 normal 상태의 이미지가 약간 어두워집니다.</param>
			/// <param name="onDown">마우스 왼쪽 버튼을 눌렀을 때부터 떼기 전까지 상태의 애니메이션(이미지)입니다. 입력하지 않으면 normal 상태의 이미지가 onOver보다 어두워집니다.</param>
			Button(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal = pAnimation(), pAnimation onOver = pAnimation(), pAnimation onDown = pAnimation());
			/// <summary>
			/// 마우스 커서가 버튼 위에 위치했을 때의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseOver();
			/// <summary>
			/// 마우스 커서가 버튼으로부터 떠났을 때의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseLeft();
			/// <summary>
			/// 마우스 왼쪽 버튼을 눌렀을 때부터 떼기 전까지의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseDown();
			/// <summary>
			/// 마우스 위치 및 클릭 상태를 파악하여 버튼이 반응합니다. (프레임당 1회 자동 호출됨)
			/// </summary>
			void Update();
			/// <summary>
			/// 버튼의 직사각형 영역을 변경합니다.
			/// </summary>
			/// <param name="newLDWH">좌-하-폭-높이 형식 직사각형입니다.</param>
			void move(const vec4& newLDWH);
			inline vec4 clickbox() { return ldwh; }
			/// <summary>
			/// 버튼을 클릭했을 때의 함수를 호출합니다.
			/// </summary>
			inline void click() { if (onClick)(*onClick)(); }
			/// <summary>
			/// onClick 함수를 변경합니다.
			/// </summary>
			inline void setOnClick(UniversalFunctor* n) { onClick = n; }
		protected:
			int st = 0;
			vec4 ldwh;
		private:
			UniversalFunctor* onClick = nullptr;
			bool hasNormal, hasOver, hasDown;
		};

		/// <summary>
		/// 마우스로 클릭할 수 있는 버튼 개체입니다.
		/// 1회 클릭하면 상태가 변하고 다시 클릭하면 이전 상태로 돌아옵니다. 프로그램 내적으로는 초기 상태가 off, 변한 상태가 on입니다.
		/// 애니메이션을 주지 않고 생성할 경우 SCTR 매개변수는 vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)으로 주어야 합니다.
		/// 기본적으로는 마우스 외에 반응을 하지 않지만 반응 함수(애니메이션 트리거)를 public으로 두어 씬에서 키보드로 접근할 수 있게 구현이 가능합니다.
		/// </summary>
		class ToggleButton : public UIEntity {
		public:
			/// <summary>
			/// 토글 버튼을 생성합니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용될 개체의 이름입니다.</param>
			/// <param name="baseSCTR">2D 스프라이트/애니메이션의 경우 이 값은 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다. 3D로 버튼을 만들려는 경우 명시적 지원은 없어서 직접 조절해야 합니다. 앞 2차원은 x,y scale이며 뒤 2차원은 x,y 이동입니다.</param>
			/// <param name="ldwh">버튼이 사용할 직사각형 영역(LDWH)입니다. 직사각형이 아닌 영역을 인식 범위로 하고 싶다면 상속하여 Update()를 오버라이드해야 합니다.</param>
			/// <param name="onClick">클릭했을 시 반응 함수입니다. UniversalFunctor 추상 클래스를 상속하여 사용하며, ToggleButton 객체는 기본적으로 변화 후의 상태가 on이면 어떤 bool 포인터가 전달되며, off이면 nullptr가 전달됩니다. 역참조로 값을 읽는 것이 아님에 주의하세요.</param>
			/// <param name="normal1">off, 기본 상태의 애니메이션(이미지)입니다. 입력하지 않으면 백색 직사각형이 렌더링됩니다.</param>
			/// <param name="normal2">on, 기본 상태의 애니메이션(이미지)입니다. 입력하지 않으면 off 상태의 이미지의 green 성분만 남은 이미지로 렌더링됩니다.</param>
			/// <param name="onOver1">off, 마우스 오버 상태의 애니메이션(이미지)입니다. 입력하지 않으면 off, normal 상태의 이미지가 약간 어두워집니다.</param>
			/// <param name="onOver2">on, 마우스 오버 상태의 애니메이션(이미지)입니다. 입력하지 않으면 on, normal 상태의 이미지가 약간 어두워집니다.</param>
			/// <param name="onDown1">off, 마우스 다운 상태의 애니메이션(이미지)입니다. 입력하지 않으면 off, normal 상태의 이미지가 onOver보다 어두워집니다.</param>
			/// <param name="onDown2">on, 마우스 다운 상태의 애니메이션(이미지)입니다. 입력하지 않으면 on, normal 상태의 이미지가 onOver보다 어두워집니다.</param>
			ToggleButton(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal1 = pAnimation(), pAnimation normal2 = pAnimation(), pAnimation onOver1 = pAnimation(), pAnimation onOver2 = pAnimation(), pAnimation onDown1 = pAnimation(), pAnimation onDown2 = pAnimation());
			/// <summary>
			/// 마우스 커서가 버튼 위에 위치했을 때의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseOver(bool isOn);
			/// <summary>
			/// 마우스 커서가 버튼으로부터 떠났을 때의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseLeft(bool isOn);
			/// <summary>
			/// 마우스 왼쪽 버튼을 눌렀을 때부터 떼기 전까지의 애니메이션이 나옵니다.
			/// </summary>
			virtual void onMouseDown(bool isOn);
			/// <summary>
			/// 마우스 위치 및 클릭 상태를 파악하여 버튼이 반응합니다. (프레임당 1회 자동 호출됨)
			/// </summary>
			void Update();
			/// <summary>
			/// 버튼의 직사각형 영역을 변경합니다.
			/// </summary>
			/// <param name="newLDWH">좌-하-폭-높이 형식 직사각형입니다.</param>
			void move(const vec4& newLDWH);
			/// <summary>
			/// onClick 함수를 변경합니다.
			/// </summary>
			inline void setOnClick(UniversalFunctor* n) { onClick = n; }
		protected:
			int st = 0;
			vec4 ldwh;
		private:
			UniversalFunctor* onClick = nullptr;
			bool hasNormal1, hasNormal2, hasOver1, hasOver2, hasDown1, hasDown2;
		};

		/// <summary>
		/// 마우스로 수평으로 끌 수 있는 버튼 개체입니다. 1을 단위로 값 양자화가 가능합니다.
		/// 양자화되지 않은 게이지의 값은 0~1의 float로 제공됩니다.
		/// 애니메이션을 주지 않고 생성할 경우 그에 해당하는 SCTR 매개변수는 vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)으로 주어야 합니다.
		/// Gauge를 생성할 경우 그 이름의 뒤에 "_bar"를 덧붙인 이름으로 다른 개체가 함께 생성됩니다(타입은 기본 Entity입니다). 이는 Gauge가 사라질 때 자동으로 제거됩니다.
		/// 이 때문에, Gauge를 생성하여 사용할 때 그 뒤에 "_bar"를 덧붙인 이름을 사용할 경우 그것이 유일한 개체가 아니며, 또한 이렇게 생성된 개체를 임의로 삭제할 경우 Gauge에 의해 세그먼테이션 오류가 발생할 수 있으니 주의하세요.
		/// </summary>
		class GaugeH : public UIEntity {
		public:
			/// <summary>
			/// 게이지를 생성합니다. 생성 직후 게이지는 오른쪽 끝에 가 있습니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용할 이름입니다.</param>
			/// <param name="barSCTR">2D 스프라이트/애니메이션의 경우 이 값은 막대 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다.</param>
			/// <param name="handleSCTR">2D 스프라이트/애니메이션의 경우 이 값은 손잡이 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다.</param>
			/// <param name="barLdwh">게이지의 막대가 차지하는 직사각형 영역입니다.</param>
			/// <param name="handleSize">게이지의 손잡이의 크기입니다. 크기 1은 게이지 막대의 높이와 동일합니다.</param>
			/// <param name="length">게이지가 가질 수 있는 값의 수입니다. 1 이하가 입력되는 경우 게이지는 0과 1 사이의 float 값을 가지며 그 이상의 경우 0부터 시작하는 int 값을 가지게 됩니다. (ex: length 3인 경우 0,1,2)</param>
			/// <param name="onScroll">드래그할 때 호출되는 함수입니다. length가 1 이하인 경우 매개변수는 float*형입니다. length가 2 이상인 경우 매개변수는 int*형입니다.</param>
			/// <param name="margin">양 끝에 들어갈 여유 공간입니다. 게이지 손잡이의 중심은 이 여유 공간까지 가지 않습니다. 이 값은 0보다 작거나 0.495보다 크면 자동으로 잘립니다.</param>
			/// <param name="isContinuous">true인 경우 게이지가 움직임에 따라 연속으로 반응 함수가 호출되고 false인 경우 마우스를 뗐을 때에만 반응 함수가 호출됩니다.</param>
			/// <param name="handle">손잡이 이미지(애니메이션)입니다.</param>
			/// <param name="bar">막대 이미지(애니메이션)입니다.</param>
			GaugeH(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin = 0.05f, bool isContinuous = false, pAnimation handle = pAnimation(), pAnimation bar = pAnimation());
			~GaugeH();
			/// <summary>
			/// 마우스 위치 및 클릭 상태를 파악하여 게이지가 반응합니다. (프레임당 1회 자동 호출됨)
			/// </summary>
			void Update();
			void render();
			/// <summary>
			/// 게이지가 가진 값을 설정합니다. 매개변수는 float이지만 이산(discrete) 게이지로 설정했더라도 그대로 사용하면 됩니다.
			/// 범위를 넘는 값이 들어오면 자동으로 잘립니다.
			/// </summary>
			inline void setValue(float v) { if (length != 1)v /= length - 1; onHolding(leftmost + (rightmost - leftmost) * v); }
		protected:
			void onHolding(float x);

			inline float getLength() { return length; }
			inline float getLeft() { return leftmost; }
			inline float getRight() { return rightmost; }
			inline bool isContin() { return isContinuous; }

			vec4 barArea, handleArea;
			const vec4 baseColor;
			const float margin;
			int st = 0;
		private:
			UniversalFunctor* onScroll = nullptr;
			Entity* bar = nullptr;
			union {
				int q;
				float c;
			}value;
			const float length;
			float leftmost, rightmost;
			bool isContinuous;
		};

		/// <summary>
		/// 마우스로 수직으로 끌 수 있는 버튼 개체입니다. 1을 단위로 값 양자화가 가능합니다.
		/// 양자화되지 않은 게이지의 값은 0~1의 float로 제공됩니다.
		/// 애니메이션을 주지 않고 생성할 경우 그에 해당하는 SCTR 매개변수는 vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)으로 주어야 합니다.
		/// Gauge를 생성할 경우 그 이름의 뒤에 "_bar"를 덧붙인 이름으로 다른 개체가 함께 생성됩니다(타입은 기본 Entity입니다). 이는 Gauge가 사라질 때 자동으로 제거됩니다.
		/// 이 때문에, Gauge를 생성하여 사용할 때 그 뒤에 "_bar"를 덧붙인 이름을 사용할 경우 그것이 유일한 개체가 아니며, 또한 이렇게 생성된 개체를 임의로 삭제할 경우 Gauge에 의해 세그먼테이션 오류가 발생할 수 있으니 주의하세요.
		/// </summary>
		class GaugeV : public UIEntity {
		public:
			/// <summary>
			/// 게이지를 생성합니다. 생성 직후 게이지는 오른쪽 끝에 가 있습니다.
			/// </summary>
			/// <param name="key">프로그램 내에서 사용할 이름입니다.</param>
			/// <param name="barSCTR">2D 스프라이트/애니메이션의 경우 이 값은 막대 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다.</param>
			/// <param name="handleSCTR">2D 스프라이트/애니메이션의 경우 이 값은 손잡이 애니메이션 객체 내의 대표 sctr값을 입력하면 됩니다.</param>
			/// <param name="barLdwh">게이지의 막대가 차지하는 직사각형 영역입니다.</param>
			/// <param name="handleSize">게이지의 손잡이의 크기입니다. 크기 1은 게이지 막대의 폭과 동일합니다.</param>
			/// <param name="length">게이지가 가질 수 있는 값의 수입니다. 1 이하가 입력되는 경우 게이지는 0과 1 사이의 float 값을 가지며 그 이상의 경우 0부터 시작하는 int 값을 가지게 됩니다. (ex: length 3인 경우 0,1,2)</param>
			/// <param name="onScroll">드래그할 때 호출되는 함수입니다. length가 1 이하인 경우 매개변수는 float*형입니다. length가 2 이상인 경우 매개변수는 int*형입니다.</param>
			/// <param name="margin">양 끝에 들어갈 여유 공간입니다. 게이지 손잡이의 중심은 이 여유 공간까지 가지 않습니다. 이 값은 0보다 작거나 0.495보다 크면 자동으로 잘립니다.</param>
			/// <param name="isContinuous">true인 경우 게이지가 움직임에 따라 연속으로 반응 함수가 호출되고 false인 경우 마우스를 뗐을 때에만 반응 함수가 호출됩니다.</param>
			/// <param name="handle">손잡이 이미지(애니메이션)입니다.</param>
			/// <param name="bar">막대 이미지(애니메이션)입니다.</param>
			GaugeV(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin = 0.05f, bool isContinuous = false, pAnimation handle = pAnimation(), pAnimation bar = pAnimation());
			~GaugeV();
			void onHolding(float y);
			void Update();
			void render();
			/// <summary>
			/// 게이지가 가진 값을 설정합니다. 매개변수는 float이지만 이산(discrete) 게이지로 설정했더라도 그대로 사용하면 됩니다.
			/// 범위를 넘는 값이 들어오면 자동으로 잘립니다.
			/// </summary>
			inline void setValue(float v) { if (length != 1)v /= length - 1; onHolding(bottommost + (topmost - bottommost) * v); }
		protected:
			inline float getLength() { return length; }
			inline float getTop() { return topmost; }
			inline float getRight() { return bottommost; }
			inline bool isContin() { return isContinuous; }
			vec4 barArea, handleArea;
			const vec4 baseColor;
			const float margin;
			int st = 0;
		private:
			UniversalFunctor* onScroll = nullptr;
			Entity* bar = nullptr;
			union {
				int q;
				float c;
			}value;
			const float length;
			float topmost, bottommost;
			bool isContinuous;
		};
		
		/// <summary>
		/// 좁은 공간에 UI 개체들을 포함하며 위치를 변경하여 접근합니다. 보이지 않게 된 개체들은 클릭 등 액션이 먹히지 않습니다.
		/// </summary>
		class Slider : public UIEntity {
		public:
			/// <summary>
			/// 슬라이더를 생성합니다. 초기에는 좌측 상단에 꼭 붙어 있습니다.
			/// </summary>
			/// <param name="key">개체 이름입니다.</param>
			/// <param name="area">슬라이더가 차지할 고정 영역입니다.</param>
			/// <param name="size">슬라이더의 논리적 공간의 가로/세로 크기입니다.</param>
			/// <param name="component">슬라이더에 포함될 개체들입니다. 존재 이유 자체에 따라, 이 개체들은 슬라이더가 제거될 때 모두 제거됩니다. 이는 여기 들어가는 개체들은 scene의 entities에 포함되지 않아야 한다는 의미와 같습니다.</param>
			Slider(const EntityKey& key, const vec4& area, const vec2& size, const std::vector<UIEntity*>& component);
			inline vec2 getPos() { return pos; }
			inline vec4 clickbox() { return area; }
			/// <summary>
			/// 슬라이더가 보여줄 위치를 변경합니다. 생성 시 적용한 범위를 넘어가지 않도록 자동 조절됩니다.
			/// </summary>
			void setPos(const vec2&);
			void Update();	// 메인 이슈: 변경하기
			void render();	// clickbox가 겹치는 것만 render
			~Slider();
		private:
			vec4 area;
			vec2 size;
			vec2 pos;
			std::vector<UIEntity*> component;
		};

		/// <summary>
		/// 텍스트를 포함하는 드롭다운 리스트입니다.
		/// </summary>
		class Dropdown :public UIEntity {
			
		};

		/// <summary>
		/// 텍스트를 입력받는 인터페이스입니다.
		/// </summary>
		class InputField : public UIEntity {

		};
	}
}

#endif // !__OA_UI_H__

