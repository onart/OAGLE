/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_GAME_H__
#define __OA_GAME_H__

#ifdef _WIN32
	#ifdef _WIN64
		#include "externals/gl/glfw/win64/glfw3.h"
		#pragma comment(lib, "externals/gl/glfw/win64/glfw3_mt.lib")
	#else
		#include "externals/gl/glfw/win32/glfw3.h"
		#pragma comment(lib, "externals/gl/glfw/win32/glfw3_mt.lib")
	#endif // _WIN64
#endif // _WIN32

#include "oaglem.h"
#include <memory>
#include <vector>

int main(int argc, char* argv[]);

namespace onart{
	
	class Camera;
	class Shader;
	class Animation;

	/// <summary>
	/// 게임을 세팅하고 시작합니다. 모든 멤버는 정적입니다.
	/// </summary>
	class Game {
		friend int ::main(int argc, char* argv[]);
	public:
		static onart::Camera mainCamera;	// 월드를 찍는 메인 카메라입니다. 여기 찍힌 것들이 화면에 출력됩니다.
		/// <summary>
		/// 현 시점(게임을 시작하고 흐른 시간(초))을 리턴합니다.
		/// </summary>
		static inline float tp() { return _tp; }
		/// <summary>
		/// 이전 프레임으로부터 현 프레임에 다다르기까지 경과한 시간(초)을 리턴합니다.
		/// </summary>
		static inline float dt() { return _dt; }
		/// <summary>
		/// dt의 역수를 리턴합니다.
		/// </summary>
		static inline float idt() { return _idt; }
		static Shader program3;	// 뷰-프로젝션 행렬 및 셰이딩의 영향을 받는 셰이더 프로그램입니다.
		static Shader program2;	// 뷰-프로젝션 행렬 및 셰이딩의 영향을 받지 않는 셰이더 프로그램입니다. 객체는 표준 뷰 볼륨에 그려집니다.

		static Shader shadowMap;	// 그림자 맵을 생성합니다.
		/// <summary>
		/// 창을 닫고 게임을 종료합니다. stdlib exit과 다르게 정상적 경로로 게임을 종료합니다.
		/// </summary>
		static void exit();
		/// <summary>
		/// 뷰포트를 기본 프레임버퍼에 맞춥니다.
		/// </summary>
		static void defaultFramebufferViewport();
		/// <summary>
		/// 창에 대한 함수를 포함합니다.
		/// </summary>
		class Window {			
		public:
			/// <summary>
			/// 마우스 커서 이미지를 생성합니다. 이것은 별개의 개체를 생성하여 렌더링합니다. 이름은 "__cursor"이며 중복된 이름을 사용하지 않도록 주의하세요.
			/// </summary>
			/// <param name="anim">커서 이미지입니다. FixedSprite 또는 UIAnimation 객체만 가능합니다. 입력하지 않는 경우 원래의 커서로 돌아옵니다.</param>
			/// <param name="scale">커서 상대 크기입니다. 1이 만들어진 크기 그대로입니다.</param>
			static void setCursorImage(const std::shared_ptr<Animation>& anim = std::shared_ptr<Animation>(), const vec2& scale = 1);
			/// <summary>
			/// 주어진 문자열(UTF8 인코딩)로 창 이름을 변경합니다. u8"이름"과 같은 리터럴을 사용하는 것이 가장 좋습니다.
			/// </summary>
			static void setWindowTitle(const char* name);
		};
		/// <summary>
		/// 창에 대한 입력을 관리합니다.
		/// </summary>
		class Input {
			friend class Game;
		public:
			/// <summary>
			/// 입력된 키를 찾기 위한 코드입니다.
			/// </summary>
			enum class KeyCode
			{
				// 숫자열
				alpha0 = GLFW_KEY_0,
				alpha1 = GLFW_KEY_1,
				alpha2 = GLFW_KEY_2,
				alpha3 = GLFW_KEY_3,
				alpha4 = GLFW_KEY_4,
				alpha5 = GLFW_KEY_5,
				alpha6 = GLFW_KEY_6,
				alpha7 = GLFW_KEY_7,
				alpha8 = GLFW_KEY_8,
				alpha9 = GLFW_KEY_9,
				minus = GLFW_KEY_MINUS,
				equal = GLFW_KEY_EQUAL,
				prime = GLFW_KEY_GRAVE_ACCENT,
				backspace = GLFW_KEY_BACKSPACE,
				// 키패드
				pad0 = GLFW_KEY_KP_0,
				pad1 = GLFW_KEY_KP_1,
				pad2 = GLFW_KEY_KP_2,
				pad3 = GLFW_KEY_KP_3,
				pad4 = GLFW_KEY_KP_4,
				pad5 = GLFW_KEY_KP_5,
				pad6 = GLFW_KEY_KP_6,
				pad7 = GLFW_KEY_KP_7,
				pad8 = GLFW_KEY_KP_8,
				pad9 = GLFW_KEY_KP_9,
				pad_slash = GLFW_KEY_KP_DIVIDE,
				asterisk = GLFW_KEY_KP_MULTIPLY,
				pad_minus = GLFW_KEY_KP_SUBTRACT,
				plus = GLFW_KEY_KP_ADD,
				pad_enter = GLFW_KEY_KP_ENTER,
				numlock = GLFW_KEY_NUM_LOCK,
				// 알파벳
				A = GLFW_KEY_A,
				B = GLFW_KEY_B,
				C = GLFW_KEY_C,
				D = GLFW_KEY_D,
				E = GLFW_KEY_E,
				F = GLFW_KEY_F,
				G = GLFW_KEY_G,
				H = GLFW_KEY_H,
				I = GLFW_KEY_I,
				J = GLFW_KEY_J,
				K = GLFW_KEY_K,
				L = GLFW_KEY_L,
				M = GLFW_KEY_M,
				N = GLFW_KEY_N,
				O = GLFW_KEY_O,
				P = GLFW_KEY_P,
				Q = GLFW_KEY_Q,
				R = GLFW_KEY_R,
				S = GLFW_KEY_S,
				T = GLFW_KEY_T,
				U = GLFW_KEY_U,
				V = GLFW_KEY_V,
				W = GLFW_KEY_W,
				X = GLFW_KEY_X,
				Y = GLFW_KEY_Y,
				Z = GLFW_KEY_Z,
				// 최상단
				F1 = GLFW_KEY_F1,
				F2 = GLFW_KEY_F2,
				F3 = GLFW_KEY_F3,
				F4 = GLFW_KEY_F4,
				F5 = GLFW_KEY_F5,
				F6 = GLFW_KEY_F6,
				F7 = GLFW_KEY_F7,
				F8 = GLFW_KEY_F8,
				F9 = GLFW_KEY_F9,
				F10 = GLFW_KEY_F10,
				F11 = GLFW_KEY_F11,
				F12 = GLFW_KEY_F12,
				escape = GLFW_KEY_ESCAPE,
				print = GLFW_KEY_PRINT_SCREEN,
				scroll = GLFW_KEY_SCROLL_LOCK,
				pause = GLFW_KEY_PAUSE,
				// 기능 패드
				insert = GLFW_KEY_INSERT,
				home = GLFW_KEY_HOME,
				pageup = GLFW_KEY_PAGE_UP,
				pagedown = GLFW_KEY_PAGE_DOWN,
				end = GLFW_KEY_END,
				Delete = GLFW_KEY_DELETE,
				// 좌/하단
				tab = GLFW_KEY_TAB,
				capslock = GLFW_KEY_CAPS_LOCK,
				shift_L = GLFW_KEY_LEFT_SHIFT,
				shift_R = GLFW_KEY_RIGHT_SHIFT,
				ctrl_L = GLFW_KEY_LEFT_CONTROL,
				ctrl_R = GLFW_KEY_RIGHT_CONTROL,
				window_L = GLFW_KEY_LEFT_SUPER,
				window_R = GLFW_KEY_RIGHT_SUPER,
				alt_L = GLFW_KEY_LEFT_ALT,
				alt_R = GLFW_KEY_RIGHT_ALT,
				// 방향키
				left = GLFW_KEY_LEFT,
				right = GLFW_KEY_RIGHT,
				up = GLFW_KEY_UP,
				down = GLFW_KEY_DOWN,
				// 나머지
				comma = GLFW_KEY_COMMA,
				period = GLFW_KEY_PERIOD,
				slash = GLFW_KEY_SLASH,
				semicolon = GLFW_KEY_SEMICOLON,
				apostrophe = GLFW_KEY_APOSTROPHE,
				backslash = GLFW_KEY_BACKSLASH,
				left_br = GLFW_KEY_LEFT_BRACKET,
				right_br = GLFW_KEY_RIGHT_BRACKET,
				space = GLFW_KEY_SPACE,
				enter = GLFW_KEY_ENTER
			};

			/// <summary>
			/// 마우스 클릭 및 휠 이동에 대하여 나온 키 코드입니다.
			/// </summary>
			enum class MouseKeyCode {
				left = GLFW_MOUSE_BUTTON_LEFT,
				right = GLFW_MOUSE_BUTTON_RIGHT,
				middle = GLFW_MOUSE_BUTTON_MIDDLE,
				wheel_up = GLFW_MOUSE_BUTTON_LAST + 1,
				wheel_down = GLFW_MOUSE_BUTTON_LAST + 2
			};

			/// <summary>
			/// 현재 키가 눌려 있다면 true를 리턴합니다.
			/// </summary>
			/// <param name="code">확인할 키</param>
			static inline bool isKeyPressed(KeyCode code) { return pressedKey[(int)code] > 0; }

			/// <summary>
			/// 현재 키가 눌려 있다면 true를 리턴합니다. 마우스의 경우 좌/우/휠 버튼만 제공되며, 마우스 휠 up/down의 경우 이 함수는 의도대로 동작하지 않을 가능성이 높으며, 사용하지 않는 것을 권장드립니다.
			/// </summary>
			/// /// <param name="code">확인할 키</param>
			static inline bool isKeyPressed(MouseKeyCode code) { return pressedMouseKey[(int)code] > 0; }

			/// <summary>
			/// 현재 프레임에 클릭되었다면 true를 리턴합니다. 마우스 휠 up/down의 경우 이번 프레임에 그 방향으로 굴리고 있다면 true가 리턴됩니다.
			/// </summary>
			/// <param name="code">확인할 키</param>
			static inline bool isKeyPressedNow(MouseKeyCode code) { return pressedMouseKey[(int)code] == Game::frame; }

			/// <summary>
			/// 현재 프레임에 키가 눌렸다면 true를 리턴합니다.
			/// </summary>
			/// <param name="code">확인할 키</param>
			static inline bool isKeyPressedNow(KeyCode code) { return pressedKey[(int)code] == Game::frame; }

			/// <summary>
			/// 현재 프레임에 키가 떼였다면 true를 리턴합니다.
			/// </summary>
			/// <param name="code">확인할 키</param>
			static inline bool isKeyReleasedNow(MouseKeyCode code) { return pressedMouseKey[(int)code] == -Game::frame; }

			/// <summary>
			/// 현제 프레임에 키가 떼였다면 true를 리턴합니다.
			/// </summary>
			/// <param name="code">확인할 키</param>
			static inline bool isKeyReleasedNow(KeyCode code) { return pressedKey[(int)code] == -Game::frame; }

			/// <summary>
			/// (마우스를 제외하고) 어떤 키든지 눌려 있는 상황인 경우 true를 리턴합니다.
			/// </summary>
			static inline bool isAnyKeyIn() { return keyCount; }

			/// <summary>
			/// 현재 프레임에 눌린 모든 키를 STL 벡터 형태로 리턴합니다. "눌려 있는"이 아닙니다.
			/// GLFW 3.3.6 버전에서는 안전합니다.
			/// </summary>
			static std::vector<KeyCode> allKeyInputsForNow();

			/// <summary>
			/// 창 내에서 마우스 포인터의 위치를 리턴합니다. 좌측 상단이 (0,0)이며 단위는 px입니다.
			/// </summary>
			static inline ivec2 cursorPos() { return mousePos; }

			/// <summary>
			/// 마우스 포인터의 위치를 게임 화면 기준으로 리턴합니다(창 기준이 아닙니다). 좌측 상단이 (0,0), 그리고 우측 하단이 (1,1)입니다.
			/// cf) Camera::mouse2screen(), Input::cameraCursorPos()
			/// *고 dpi 화면에 대한 보정이 검증되지 않은 상태입니다.
			/// </summary>
			static vec2 relativeCursorPos();

			/// <summary>
			/// 마우스 포인터의 위치를 뷰포트 기준으로 리턴합니다. 중심이 (0,0)이며 종횡 중 짧은 쪽의 길이가 2입니다.
			/// 프레임당 여러 번 필요할 경우 이것을 활용하는 것이 훨씬 좋습니다.
			/// </summary>
			static vec2 cameraCursorPos();
		private:
			static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouse(GLFWwindow* window, int key, int action, int mods);
			static void mouseMoved(GLFWwindow* window, double x, double y);
			static void scroll(GLFWwindow* window, double x, double y);
			static onart::ivec2 windowSize;
			static ivec2 mousePos;
			static int pressedKey[];
			static int pressedMouseKey[];
			static int keyCount;
		};

	private:
		static int frame;
		static float _tp, _dt, _idt;
		static GLFWwindow* window;
		static onart::ivec2 windowSize;
		static onart::ivec4 vp_ldwh;
		static int start();		// 진입점에서 초기화 및 시작
		static bool init();
		static void update();	// 게임 루프 시 호출
		static void render();	// 게임 루프 시 호출
		static void finalize();
		static void reshape(GLFWwindow* window, int x, int y);
	};

	using Input = Game::Input;		// 창에 대한 입력을 관리합니다.
	using Window = Game::Window;	// 창에 대한 옵션을 관리합니다.
}

#endif // !__OA_GAME_H__

