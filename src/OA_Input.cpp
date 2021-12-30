#include "OA_Input.h"

extern onart::ivec2 windowSize;
extern onart::ivec4 vp_lurd;

namespace onart::Input {
	std::vector<KeyCode> allKeyInputsForNow() {
		std::vector<KeyCode> v;	v.reserve(6);
		for (int i = 32; i <= 96; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 256; i <= 301; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 320; i <= 347; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		return v;
	}

	vec2 relativeCursorPos() {
		int vw = vp_lurd[2] - vp_lurd[0];
		int vh = vp_lurd[1] - vp_lurd[3];
		vec2 rpos((float)mousePos.x, (float)mousePos.y);
		rpos.x -= vp_lurd[0];	rpos.y -= vp_lurd[3];	// 뷰포트 좌측상단 좌표를 (0,0)으로 하도록 평행이동 (일반적인 화면 크기 범위에서는 절대 오차가 생기지 않음)
		rpos.x /= vw;	rpos.y /= vh;					// 뷰포트 우측하단 좌표를 (1,1)로 하도록 수축
		return rpos;
	}
}
