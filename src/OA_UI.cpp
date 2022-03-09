/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_UI.h"

namespace onart::UI {
	oastring toPlain(const oastring& content) {
		oastring ret;
		int charCount = (int)content.size();
		for (int i = 0; i < charCount; i++) {
			oachar c = content[i];
			switch (c)
			{
			case '\a':
				i += 5;
				continue;
			case '\b':
				i += 8;
				continue;
			default:
				ret += c;
			}
		}
		return ret;
	}

	Text::Text(const EntityKey& key, Font* font, const oastring& content, const vec4& targRect, float maxWidth, bool fullFit, AlignH align, float rowGap, const vec4& color)
		:Entity(key, Transform(), true), font(font), input(content), maxWidth(maxWidth), color(color), rectFixed(true), halign(align), rowGap(rowGap), center(targRect.left + targRect.width / 2, targRect.down + targRect.width / 2), fullFit(fullFit), targRect(targRect) {
		setContent(content);
		if (fullFit) { r2r = mat4::r2r(ldwh, targRect); }
		else { r2r = mat4::r2r2(ldwh, targRect); }
	}

	Text::Text(const EntityKey& key, Font* font, const oastring& content, const vec2& center, float maxWidth, AlignH align, AlignV va, float size, float rowGap, const vec4& color)
		: Entity(key, Transform(), true), font(font), input(content), maxWidth(maxWidth), color(color), rectFixed(false), halign(align), valign(va), rowGap(rowGap), center(center), size(size) {
		setContent(content);
	}

	void Text::setAlign(AlignH a) {
		halign = a;
		font->getRectNLine(content, linexy, halign, rowGap);
	}

	void Text::setContent(const oastring& n) {
		input = n;
		plain = toPlain(n);
		if (isfinite(maxWidth)) content = font->cutLine(n, maxWidth);
		else content = n;
		ldwh = font->getRectNLine(content, linexy, halign, rowGap);
		if (rectFixed) {
			if (fullFit) { r2r = mat4::r2r(ldwh, targRect); }
			else { r2r = mat4::r2r2(ldwh, targRect); }
		}
	}

	void Text::append(const oastring& n) {
		setContent(input + n);
	}

	void Text::setRotation(const Quaternion& q) {
		if (q.is1()) { 
			additionalTransform.toI();
			hasAdditional = false;
		}
		else {
			hasAdditional = true;
			additionalTransform = mat4::translate(center) * q.toMat4() * mat4::translate(-center);
		}
	}

	void Text::setAdditinoalTransform(const mat4& m) {
		additionalTransform = m;
		hasAdditional = !m.isI();
	}

	void Text::render()
	{
		if (rectFixed) {
			if (hasAdditional)	font->draw(content, additionalTransform * r2r, linexy, color);
			else font->draw(content, r2r, linexy, color);
		}
		else {
			if (hasAdditional)	font->draw(content, ldwh, linexy, additionalTransform, halign, valign, center, size, color);
			else font->draw(content, ldwh, linexy, halign, valign, center, size, color);
		}
	}

	Button::Button(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal, pAnimation onOver, pAnimation onDown)
		:Entity(key, Transform(), true), ldwh(ldwh), onClick(onClick) {
		FixedSprite::make("__defaultbutton", Material::get("white1x1"));
		if (hasNormal = (bool)normal) { addAnim(normal); } else { addAnim("__defaultbutton"); }
		if (hasOver = (bool)onOver) { addAnim(onOver); } else { addAnim("__defaultbutton"); }
		if (hasDown = (bool)onDown) { addAnim(onDown); }
		vec4 prv(-baseSCTR.x / 2 + baseSCTR.z, -baseSCTR.y / 2 + baseSCTR.w, baseSCTR.x, baseSCTR.y);
		mat4 tr = mat4::r2r(prv, ldwh, -0.8f);
		transform.setScale(tr[0], tr[5], 1);
		transform.setPosition(tr[3], tr[7], -0.8f);
	}

	void Button::onMouseOver() {
		st = 1;
		if (hasOver) { color = 1; setAnim(1); }
		else { color = vec4(vec3(0.8f), 1); setAnim(0); }
	}

	void Button::onMouseLeft() {
		st = 0;
		setAnim(0);
		color = 1;
	}

	void Button::onMouseDown() {
		st = 2;
		if (hasDown) { color = 1; setAnim(2); }
		else { color = vec4(vec3(0.5f), 1); setAnim(0); }		
	}

	void Button::Update() {
		vec2 pos = Input::cameraCursorPos();
		const bool isOver = pos.x >= ldwh.left && pos.x <= ldwh.left + ldwh.width && pos.y >= ldwh.down && pos.y <= ldwh.down + ldwh.height;
		switch (st)
		{
		case 0:	// 기본
			if (isOver) onMouseOver();
			break;
		case 1:	// 마우스 오버 후
			if (!isOver) onMouseLeft();
			else if (Input::isKeyPressedNow(Input::MouseKeyCode::left)) onMouseDown();
			break;
		case 2:	// 마우스 다운 후
			if (Input::isKeyReleasedNow(Input::MouseKeyCode::left)) {
				if (isOver) {
					if (onClick) (*onClick)();
					onMouseOver();
				}
				else {
					onMouseLeft();
				}
			}
			break;
		default:
			onMouseLeft();
			break;
		}
		
	}

	void Button::move(const vec4& newLDWH) {
		mat4 tr = mat4::r2r(ldwh, newLDWH);
		transform.setScale(transform.getScale() * vec3(tr[0], tr[5], 1));
		transform.addPosition(tr[3], tr[7], 0);
		ldwh = newLDWH;
	}

	ToggleButton::ToggleButton(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal1, pAnimation normal2, pAnimation onOver1, pAnimation onOver2, pAnimation onDown1, pAnimation onDown2)
		:Entity(key, Transform(), true), ldwh(ldwh), onClick(onClick) {
		FixedSprite::make("__defaultbutton", Material::get("white1x1"));
		if (hasNormal1 = (bool)normal1) { addAnim(normal1); } else { addAnim("__defaultbutton"); }
		if (hasNormal2 = (bool)normal2) { addAnim(normal2); } else { addAnim("__defaultbutton"); }
		if (hasOver1 = (bool)onOver1) { addAnim(onOver1); } else { addAnim("__defaultbutton"); }
		if (hasOver2 = (bool)onOver2) { addAnim(onOver2); } else { addAnim("__defaultbutton"); }
		if (hasDown1 = (bool)onDown1) { addAnim(onDown1); } else { addAnim("__defaultbutton"); }
		if (hasDown2 = (bool)onDown2) { addAnim(onDown2); }
		vec4 prv(-baseSCTR.x / 2 + baseSCTR.z, -baseSCTR.y / 2 + baseSCTR.w, baseSCTR.x, baseSCTR.y);
		mat4 tr = std::move(mat4::r2r(prv, ldwh, -0.8f));
		transform.setScale(tr[0], tr[5], 1);
		transform.setPosition(tr[3], tr[7], -0.8f);
	}

	void ToggleButton::onMouseOver(bool isOn) {
		if (!isOn) {
			st = 2;
			if (hasOver1) { 
				setAnim(2);
				color = 1;
			}
			else {
				setAnim(0);
				color = vec4(vec3(0.8f), 1);
			}
		}
		else {
			st = 3;
			if (hasOver2) {
				setAnim(3);
				color = 1;
			}
			else {
				if (hasNormal2) {
					setAnim(1);
					color = 1;
				}
				else {
					setAnim(0);
					color = vec4(0, 0.8f, 0, 1);
				}
			}
		}
	}

	void ToggleButton::onMouseLeft(bool isOn) {
		if (!isOn) {
			st = 0;
			color = 1;
			setAnim(0);
		}
		else {
			st = 1;
			if (hasNormal2) {
				setAnim(1);
				color = 1;
			}
			else {
				setAnim(0);
				color = vec4(0, 1, 0, 1);
			}
		}
	}

	void ToggleButton::onMouseDown(bool isOn) {
		if (!isOn) {
			st = 4;
			if (hasDown1) {
				setAnim(4);
				color = 1;
			}
			else {
				setAnim(0);
				color = vec4(vec3(0.5f), 1);
			}
		}
		else {
			st = 5;
			if (hasDown2) {
				setAnim(5);
				color = 1;
			}
			else {
				if (hasNormal2) {
					setAnim(1);
					color = vec4(vec3(0.5f), 1);
				}
				else {
					setAnim(0);
					color = vec4(0, 0.5f, 0, 1);
				}
			}
		}
	}

	void ToggleButton::Update() {
		vec2 pos = Input::cameraCursorPos();
		const bool isOver = pos.x >= ldwh.left && pos.x <= ldwh.left + ldwh.width && pos.y >= ldwh.down && pos.y <= ldwh.down + ldwh.height;
		const bool var = st % 2;
		switch (st)
		{
		case 0:	// off 기본
		case 1:	// on 기본
			if (isOver)onMouseOver(var);
			break;
		case 2:	// off 오버
		case 3: // on 오버
			if (!isOver)onMouseLeft(var);
			else if (Input::isKeyPressedNow(Input::MouseKeyCode::left)) onMouseDown(var);
			break;
		case 4:	// off 다운
		case 5: // on 다운
			if (Input::isKeyReleasedNow(Input::MouseKeyCode::left)) {
				if (isOver) {
					if (onClick) {						
						if (st % 2)	(*onClick)(&var);	// 이제 켜짐
						else (*onClick)();	// 이제 꺼짐
					}
					onMouseOver(!var);
				}
				else {
					onMouseLeft(var);
				}
			}
			break;
		default:
			onMouseLeft(false);
			break;
		}
	}

	void ToggleButton::move(const vec4& newLDWH) {
		mat4 tr = std::move(mat4::r2r(ldwh, newLDWH));
		transform.setScale(transform.getScale() * vec3(tr[0], tr[5], 1));
		transform.addPosition(tr[3], tr[7], 0);
		ldwh = newLDWH;
	}

	GaugeH::GaugeH(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin, bool isContinuous, pAnimation handle, pAnimation bar)
		:Entity(key, Transform(), true), length(length), barArea(barLdwh), margin(margin), baseColor(handle ? 1 : vec4(0.5f, 0.5f, 0.5f, 1)), isContinuous(isContinuous) {
		if (length <= 1) { value.c = 1; }
		else { value.q = length - 1; }
		margin = clamp(margin, 0.0f, 0.495f);
		vec4 prv(-barSCTR.x / 2 + barSCTR.z, -barSCTR.y / 2 + barSCTR.w, barSCTR.x, barSCTR.y);
		mat4 tr = std::move(mat4::r2r(prv, barArea));
		if (!bar)bar = std::move(FixedSprite::make("__defaultbutton", Material::get("white1x1")));
		if (!handle) { 
			handle = std::move(FixedSprite::make("__defaultbutton", Material::get("white1x1"))); 
			color = std::move(vec4(vec3(0.5f), 1));
		}
		this->bar = new Entity(key + "bar", Transform(vec3(tr[3], tr[7], -0.8f), vec3(tr[0], tr[5], 1)), bar, true);
		leftmost = barArea.x + barArea.width * margin;
		rightmost = barArea.x + barArea.width * (1 - margin);
		prv = std::move(vec4(-handleSCTR.x / 2 + handleSCTR.z, -handleSCTR.y / 2 + handleSCTR.w, handleSCTR.x, handleSCTR.y));
		vec2 center(barArea.left + barArea.width * (1 - margin), barArea.down + barArea.height / 2);
		vec2 newSize = vec2(barArea.height, barArea.height) * handleSize;
		handleArea = std::move(vec4(center.x - newSize.x / 2, center.y - newSize.y / 2, newSize.x, newSize.y));
		tr = std::move(mat4::r2r(prv, handleArea));
		transform.setPosition(tr[3], tr[7], -0.81f);
		transform.setScale(tr[0], tr[5], 1);
		addAnim(bar);
	}

	GaugeV::GaugeV(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin, bool isContinuous, pAnimation handle, pAnimation bar)
		:Entity(key, Transform(), true), length(length), barArea(barLdwh), margin(margin), baseColor(handle ? 1 : vec4(0.5f, 0.5f, 0.5f, 1)), isContinuous(isContinuous) {
		if (length <= 1) { value.c = 1; }
		else { value.q = length - 1; }
		margin = clamp(margin, 0.0f, 0.495f);
		vec4 prv(-barSCTR.x / 2 + barSCTR.z, -barSCTR.y / 2 + barSCTR.w, barSCTR.x, barSCTR.y);
		mat4 tr = std::move(mat4::r2r(prv, barArea));
		if (!bar)bar = std::move(FixedSprite::make("__defaultbutton", Material::get("white1x1")));
		if (!handle) {
			handle = std::move(FixedSprite::make("__defaultbutton", Material::get("white1x1")));
			color = std::move(vec4(vec3(0.5f), 1));
		}
		this->bar = new Entity(key + "bar", Transform(vec3(tr[3], tr[7], -0.8f), vec3(tr[0], tr[5], 1)), bar, true);
		bottommost = barArea.y + barArea.height * margin;
		topmost = barArea.y + barArea.height * (1 - margin);
		prv = std::move(vec4(-handleSCTR.x / 2 + handleSCTR.z, -handleSCTR.y / 2 + handleSCTR.w, handleSCTR.x, handleSCTR.y));
		vec2 center(barArea.left + barArea.width / 2, barArea.down + barArea.height * (1 - margin));
		vec2 newSize = vec2(barArea.width, barArea.width) * handleSize;
		handleArea = std::move(vec4(center.x - newSize.x / 2, center.y - newSize.y / 2, newSize.x, newSize.y));
		tr = std::move(mat4::r2r(prv, handleArea));
		transform.setPosition(tr[3], tr[7], -0.81f);
		transform.setScale(tr[0], tr[5], 1);
		addAnim(bar);
	}

	void GaugeH::onHolding(float x) {
		st = 2;
		color = baseColor * vec4(vec3(0.6f), 1);
		float scale = (x - leftmost) / (rightmost - leftmost);
		scale = clamp(scale, 0.0f, 1.0f);
		if (length == 1) {
			float prev = value.c;
			if (scale == 0)x = leftmost;
			else if (scale == 1)x = rightmost;
			transform.setPositionX(x);
			handleArea.left = x - handleArea.width / 2;
			value.c = scale;
			if (isContinuous && onScroll && prev != value.c) {
				(*onScroll)(&value);
			}
		}
		else {
			int prev = value.q;
			value.q = lroundf(scale * (length - 1));
			float xpos = leftmost + barArea.width / (length - 1) * value.q * (1 - margin * 2);
			xpos = xpos > rightmost ? rightmost : xpos;
			transform.setPositionX(xpos);
			handleArea.left = xpos - handleArea.width / 2;
			if (isContinuous && onScroll && prev != value.q) {
				(*onScroll)(&value);
			}
		}
		if (!Input::isKeyPressed(Input::MouseKeyCode::left)) {
			st = 0;
			if (!isContinuous && onScroll) {
				(*onScroll)(&value);
			}
		}
	}

	void GaugeV::onHolding(float y) {
		st = 2;
		color = baseColor * vec4(vec3(0.6f), 1);
		float scale = (y - bottommost) / (topmost - bottommost);
		scale = clamp(scale, 0.0f, 1.0f);
		if (length == 1) {
			float prev = value.c;
			if (scale == 0)y = bottommost;
			else if (scale == 1)y = topmost;
			transform.setPositionY(y);
			handleArea.down = y - handleArea.height / 2;
			value.c = scale;
			if (isContinuous && onScroll && prev != value.c) {
				(*onScroll)(&value);
			}
		}
		else {
			int prev = value.q;
			value.q = lroundf(scale * (length - 1));
			float ypos = bottommost + barArea.height / (length - 1) * value.q * (1 - margin * 2);
			ypos = ypos > topmost ? topmost : ypos;
			transform.setPositionY(ypos);
			handleArea.down = ypos - handleArea.height / 2;
			if (isContinuous && onScroll && prev != value.q) {
				(*onScroll)(&value);
			}
		}
		if (!Input::isKeyPressed(Input::MouseKeyCode::left)) {
			st = 0;
			if (!isContinuous && onScroll) {
				(*onScroll)(&value);
			}
		}
	}

	void GaugeH::Update() {
		vec2 pos = Input::cameraCursorPos();
		const bool isOnHandle = pos.x >= handleArea.left && pos.x <= handleArea.left + handleArea.width && pos.y >= handleArea.down && pos.y <= handleArea.down + handleArea.height;
		const bool isOnBar = pos.x >= barArea.left && pos.x <= barArea.left + barArea.width && pos.y >= barArea.down && pos.y <= barArea.down + barArea.height;
		switch (st)
		{
		case 0:	// 기본
			if (isOnHandle) {
				color = baseColor * vec4(vec3(0.8f), 1);
				st = 1;
			}
			else {
				color = baseColor;
				if (isOnBar && Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.x);
				}
			}
			break;
		case 1:	// 손잡이 위 커서
			if (isOnHandle) {
				if (Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.x);
				}
			}
			else {
				color = baseColor;
				if (isOnBar && Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.x);
				}
				else {
					st = 0;
				}
			}
			break;
		case 2:	// 손잡이를 잡고 있는 커서
			onHolding(pos.x);
			break;
		default:
			break;
		}
	}

	void GaugeV::Update() {
		vec2 pos = Input::cameraCursorPos();
		const bool isOnHandle = pos.x >= handleArea.left && pos.x <= handleArea.left + handleArea.width && pos.y >= handleArea.down && pos.y <= handleArea.down + handleArea.height;
		const bool isOnBar = pos.x >= barArea.left && pos.x <= barArea.left + barArea.width && pos.y >= barArea.down && pos.y <= barArea.down + barArea.height;
		switch (st)
		{
		case 0:	// 기본
			if (isOnHandle) {
				color = baseColor * vec4(vec3(0.8f), 1);
				st = 1;
			}
			else {
				color = baseColor;
				if (isOnBar && Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.y);
				}
			}
			break;
		case 1:	// 손잡이 위 커서
			if (isOnHandle) {
				if (Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.y);
				}
			}
			else {
				color = baseColor;
				if (isOnBar && Input::isKeyPressedNow(Input::MouseKeyCode::left)) {
					onHolding(pos.y);
				}
				else {
					st = 0;
				}
			}
			break;
		case 2:	// 손잡이를 잡고 있는 커서
			onHolding(pos.y);
			break;
		default:
			st = 0;
			break;
		}
	}

	void GaugeH::render() {
		bar->render();
		Entity::render();
	}

	void GaugeV::render() {
		bar->render();
		Entity::render();
	}

	GaugeH::~GaugeH() {
		delete bar;
	}

	GaugeV::~GaugeV() {
		delete bar;
	}
}