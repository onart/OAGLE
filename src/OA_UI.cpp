#include "OA_UI.h"

namespace onart::UI {
	oastring toPlain(const oastring& content) {
		oastring ret;
		int charCount = (int)content.size();
		for (int i = 0; i < charCount; i++) {	// 이 부분은 매번 수행할 필요 없음(totalLDWH만 도출되면 됨 -> 텍스트 개체를 만들어서 스트링 혹은 목표 직사각형이 변할 때마다 행렬 변경하도록)
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

	Text::Text(const EntityKey& key, Font* font, const oastring& content, const vec4& targRect, float maxWidth, bool fullFit, Align align, float rowGap, const vec4& color)
		:Entity(key, Transform(), true), font(font), content(content), color(color), rectFixed(true), align(align), rowGap(rowGap), center(targRect.left + targRect.width / 2, targRect.down + targRect.width / 2) {
		if (isfinite(maxWidth))	this->content = font->cutLine(content, maxWidth);
		ldwh = font->getRectNLine(this->content, linexy, align, rowGap);
		plain = toPlain(content);
		if (fullFit) { r2r = mat4::r2r(ldwh, targRect); }
		else { r2r = mat4::r2r2(ldwh, targRect); }
	}

	Text::Text(const EntityKey& key, Font* font, const oastring& content, const vec2& center, Align align, float size, float rowGap, const vec4& color)
		: Entity(key, Transform(), true), font(font), content(content), color(color), rectFixed(false), rowGap(rowGap), center(center), size(size) {
		ldwh = font->getRectNLine(content, linexy, align, rowGap);
		plain = toPlain(content);
	}

	void Text::setAlign(Align a) {
		align = a;
		font->getRectNLine(content, linexy, align, rowGap);
	}

	void Text::setContent(const oastring& n) {
		ldwh = font->getRectNLine(n, linexy, align, rowGap);
		plain = toPlain(content);
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
			if (hasAdditional)	font->draw(content, ldwh, linexy, additionalTransform, center, size, color); 
			else font->draw(content, ldwh, linexy, center, size, color);
		}
	}
}