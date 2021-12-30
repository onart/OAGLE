#ifndef __OA_INPUT_H__
#define __OA_INPUT_H__

#ifdef _WIN32
	#ifdef _WIN64
		#include "gl/glfw/win64/glfw3.h"
		#pragma comment(lib, "win64/glfw3_mt.lib")
	#else
		#include "gl/glfw/win32/glfw3.h"
		#pragma comment(lib, "win32/glfw3_mt.lib")
	#endif // _WIN64
#endif // _WIN32

#include <vector>

#include "oaglem.h"

extern int pressedKey[];
extern int pressedMouseKey[];
extern int frame;
extern int keyCount;

extern onart::ivec2 mousePos;

namespace onart::Input {
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
	inline bool isKeyPressed(KeyCode code) { return pressedKey[(int)code] > 0; }

	/// <summary>
	/// 현재 키가 눌려 있다면 true를 리턴합니다. 마우스의 경우 좌/우/휠 버튼만 제공되며, 마우스 휠 up/down의 경우 이 함수는 의도대로 동작하지 않을 가능성이 높으며, 사용하지 않는 것을 권장드립니다.
	/// </summary>
	/// /// <param name="code">확인할 키</param>
	inline bool isKeyPressed(MouseKeyCode code) { return pressedMouseKey[(int)code] > 0; }

	/// <summary>
	/// 현재 프레임에 클릭되었다면 true를 리턴합니다. 마우스 휠 up/down의 경우 이번 프레임에 그 방향으로 굴리고 있다면 true가 리턴됩니다.
	/// </summary>
	/// <param name="code">확인할 키</param>
	inline bool isKeyPressedNow(MouseKeyCode code) { return pressedMouseKey[(int)code] == frame; }

	/// <summary>
	/// 현재 프레임에 키가 눌렸다면 true를 리턴합니다.
	/// </summary>
	/// <param name="code">확인할 키</param>
	inline bool isKeyPressedNow(KeyCode code) { return pressedKey[(int)code] == frame; }

	/// <summary>
	/// 현재 프레임에 키가 떼였다면 true를 리턴합니다.
	/// </summary>
	/// <param name="code">확인할 키</param>
	inline bool isKeyReleasedNow(MouseKeyCode code) { return pressedMouseKey[(int)code] == -frame; }

	/// <summary>
	/// 현제 프레임에 키가 떼였다면 true를 리턴합니다.
	/// </summary>
	/// <param name="code">확인할 키</param>
	inline bool isKeyReleasedNow(KeyCode code) { return pressedKey[(int)code] == -frame; }

	/// <summary>
	/// (마우스를 제외하고) 어떤 키든지 눌려 있는 상황인 경우 true를 리턴합니다.
	/// </summary>
	inline bool isAnyKeyIn() { return keyCount; }

	/// <summary>
	/// 현재 프레임에 눌린 모든 키를 STL 벡터 형태로 리턴합니다. "눌려 있는"이 아닙니다.
	/// GLFW 3.3.6 버전에서는 안전합니다.
	/// </summary>
	std::vector<KeyCode> allKeyInputsForNow();

	/// <summary>
	/// 창 내에서 마우스 포인터의 위치를 리턴합니다. 좌측 상단이 (0,0)이며 단위는 px입니다.
	/// </summary>
	inline ivec2 cursorPos() { return mousePos; }

	/// <summary>
	/// 마우스 포인터의 위치를 게임 화면 기준으로 리턴합니다. 좌측 상단이 (0,0), 그리고 우측 하단이 (1,1)입니다.
	/// *고 dpi 화면에 대한 보정이 검증되지 않은 상태입니다.
	/// </summary>
	vec2 relativeCursorPos();
}

#endif // !__OA_INPUT_H__