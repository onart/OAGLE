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
		rpos.x -= vp_lurd[0];	rpos.y -= vp_lurd[3];	// ����Ʈ ������� ��ǥ�� (0,0)���� �ϵ��� �����̵� (�Ϲ����� ȭ�� ũ�� ���������� ���� ������ ������ ����)
		rpos.x /= vw;	rpos.y /= vh;					// ����Ʈ �����ϴ� ��ǥ�� (1,1)�� �ϵ��� ����
		return rpos;
	}
}
