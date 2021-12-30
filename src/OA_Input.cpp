#include "OA_Input.h"

extern onart::ivec2 windowSize;
extern onart::ivec4 vp_ldwh;

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
		vec2 rpos(mousePos);
		rpos.x -= vp_ldwh[0];	rpos.y -= vp_ldwh[1];					// 뷰포트 좌측상단 좌표를 (0,0)으로 하도록 평행이동 (일반적인 화면 크기 범위에서는 절대 오차가 생기지 않음)
		rpos.x /= vp_ldwh[2];	rpos.y /= vp_ldwh[3];					// 뷰포트 우측하단 좌표를 (1,1)로 하도록 수축
		return rpos;
	}
}
