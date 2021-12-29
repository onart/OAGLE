#include "OA_Input.h"

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
}
