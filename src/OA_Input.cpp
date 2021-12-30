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
		rpos.x -= vp_ldwh[0];	rpos.y -= vp_ldwh[1];					// ����Ʈ ������� ��ǥ�� (0,0)���� �ϵ��� �����̵� (�Ϲ����� ȭ�� ũ�� ���������� ���� ������ ������ ����)
		rpos.x /= vp_ldwh[2];	rpos.y /= vp_ldwh[3];					// ����Ʈ �����ϴ� ��ǥ�� (1,1)�� �ϵ��� ����
		return rpos;
	}
}
