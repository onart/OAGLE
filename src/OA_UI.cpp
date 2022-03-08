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
		if (hasDown = (bool)onDown) { addAnim(onDown); } else { addAnim("__defaultbutton"); }
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
		bool isOver = pos.x >= ldwh.left && pos.x <= ldwh.left + ldwh.width && pos.y >= ldwh.down && pos.y <= ldwh.down + ldwh.height;
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
				onMouseLeft();
				if (isOver) {
					if (onClick) (*onClick)();
					onMouseOver();
				}
			}
			break;
		default:
			break;
		}
		
	}

	void Button::move(const vec4& newLDWH) {
		mat4 tr = mat4::r2r(ldwh, newLDWH);
		transform.setScale(transform.getScale() * vec3(tr[0], tr[5], 1));
		transform.addPosition(tr[3], tr[7], 0);
		ldwh = newLDWH;
	}
}