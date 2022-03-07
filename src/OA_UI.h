/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_UI_H__
#define __OA_UI_H__
#include "OA_Entity.h"
#include "OA_Text.h"
#include "OA_UniversalFunctor.h"

namespace onart {
	class Font;
	class UIAnimation;
	struct Texture;
	namespace UI {
		/// <summary>
		/// �ؽ�Ʈ ��ü�Դϴ�. ���ں��� ũ��/������ ������ �� �ֽ��ϴ�.
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y �Ǵ� a. �빮�ڴ� �������� �ʽ��ϴ�.), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�(��, �ִ� ��� ũ��� 9.99/0.01=999��). ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Է��ϸ� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�. a���� x,y�� ��θ� �����մϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�.</para>
		/// </summary>
		class Text: public Entity {
		public:
			/// <summary>
			/// �ؽ�Ʈ ��ü�� �����մϴ�. Ư�� ���簢�� ������ �����˴ϴ�. �̸� �׸� ���� UI�� ����ϱ⿡ �����մϴ�.
			/// ���簢�� ������ �����Ǳ� ������ ���� ������ �������� �ʽ��ϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� �ؽ�Ʈ ��ü�� �̸��Դϴ�.</param>
			/// <param name="font">�ؽ�Ʈ�� ����Դϴ�. ���� Ȥ�� �޸𸮿��� �ҷ��� �� �ֽ��ϴ�.</param>
			/// <param name="content">�ؽ�Ʈ�� ���� �� ���ں� ũ��/�����Դϴ�. </param>
			/// <param name="ldwh">�ؽ�Ʈ�� �� ���簢��(L-D-W-H)�� �°� ����մϴ�.</param>
			/// <param name="maxWidth">�ִ� ���Դϴ�. ���簢���� ������ ������ �ƴ�, �ҷ��� ��Ʈ�� ���� �ȼ� �����Դϴ�. ���� �����ϸ鼭 ������ ���ߴ� ���� ���ϰ� ����˴ϴ�.</param>
			/// <param name="fullFit">true�� ��� �־��� ���簢���� ��Ȯ�� �°� ����մϴ�. ��, ������ ���� ������ �������� �ʽ��ϴ�.</param>
			/// <param name="align">���� ���� �����Դϴ�. ���/����/������ �ɼǸ� �����մϴ�.</param>
			/// <param name="rowGap">�� �����Դϴ�. ���� ���α׷���� ������ ������ �����Ǵ� ���� �ƴϹǷ� ���� ������ �ֽñ� �ٶ��ϴ�.</param>
			/// <param name="color">��ü�� ���Դϴ�.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec4& ldwh, float maxWidth = INF, bool fullFit = false, AlignH align = AlignH::CENTER, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// �ؽ�Ʈ ��ü�� �����մϴ�. ���簢�� ������ �������� �ʰ� Ư�� ������ �߽����� �մϴ�.
			/// �̸� �׸� ���� �� ��ü���� �ؽ�Ʈ�� ����� �� ����ϱ⿡ �����մϴ�. �� ��� Camera::world2screen() �Լ��� ������ �ּ���.
			/// ������ ��ȭâ�� ���� �ؽ�Ʈ�� ũ�Ⱑ �ǽð����� ���ϸ� ����� �κп��� ����ϱ� �����մϴ�. �� ��� ���ѵ� ������ ���ڸ� ���ߴ� ����� �Ű����� size�� maxWidth���Դϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� �ؽ�Ʈ ��ü�� �̸��Դϴ�.</param>
			/// <param name="font">�ؽ�Ʈ�� ����Դϴ�. ���� Ȥ�� �޸𸮿��� �ҷ��� �� �ֽ��ϴ�.</param>
			/// <param name="content">�ؽ�Ʈ�� ���� �� ���ں�/ũ��/�����Դϴ�.</param>
			/// <param name="center">�߽��� �Ǵ� ��ǥ�Դϴ�. ���� ȭ�鿡�� ����� (0,0)�̰� ��Ⱦ �� ª�� ���̰� 2�Դϴ�. center�� �ǹ̴� �Ű����� ha, va�� ���� �޶����ϴ�.</param>
			/// <param name="maxWidth">�ִ� ���Դϴ�. ���簢���� ������ ������ �ƴ�, �ҷ��� ��Ʈ�� ���� �ȼ� �����Դϴ�. ���� �����ϸ鼭 ������ ���ߴ� ���� ���ϰ� ����˴ϴ�.</param>
			/// <param name="ha">���� ���� �����Դϴ�. ���/����/������ �ɼǸ� �����մϴ�. Ư���� ���� ������ ������ �ؽ�Ʈ�� �ƴ� ��� ��� ���İ� �����մϴ�.</param>
			/// <param name="va">���� ���� �����Դϴ�. ���/����/�Ʒ��� �ɼǸ� �����մϴ�.</param>
			/// <param name="size">�ؽ�Ʈ�� ũ���Դϴ�. �ٸ� ��Ʈ��� ���� ���� ���� ũ��� ������� �ʽ��ϴ�.</param>
			/// <param name="rowGap">�� �����Դϴ�. ���� ���α׷���� ������ ������ �����Ǵ� ���� �ƴϹǷ� ���� ������ �ֽñ� �ٶ��ϴ�.</param>
			/// <param name="color">��ü�� ���Դϴ�.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec2& center, float maxWidth = INF, AlignH ha = AlignH::CENTER, AlignV va = AlignV::CENTER, float size = 1, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// �־��� ���ڿ��� ������ �ٲߴϴ�. ���ں� ũ��/���� �ɼ��� �����ڸ� �����ϼ���.
			/// </summary>
			void setContent(const oastring&);
			/// <summary>
			/// �־��� ���ڿ��� ���뿡 �����Դϴ�.
			/// </summary>
			/// <param name=""></param>
			void append(const oastring&);
			/// <summary>
			/// ���� ������ �����մϴ�. ���/����/�����ʸ� �����մϴ�.
			/// </summary>
			void setAlign(AlignH);
			void render();
			/// <summary>
			/// �ٸ� �ҷ��� ��Ʈ�� �����մϴ�.
			/// </summary>
			inline void setFont(Font* font) { this->font = font; }
			/// <summary>
			/// ���ڿ� ��ü�� ���� �ٲߴϴ�. ���ں� ������� ������ ����˴ϴ�.
			/// </summary>
			inline void setColor(const vec4& color) { this->color = color; }
			/// <summary>
			/// �߽� ��ġ�� ����Ͽ� ȸ���� �����մϴ�. ������ �ִ� �ٸ� ��ȯ�� ����ϴ�.
			/// </summary>
			void setRotation(const Quaternion&);
			/// <summary>
			/// �߰� ��ȯ�� �����մϴ�. ������ �ִ� �ٸ� ��ȯ�� ����ϴ�.
			/// </summary>
			void setAdditinoalTransform(const mat4&);
			/// <summary>
			/// ũ��/���� ������ Ȯ�忭�� ������ �ؽ�Ʈ�� ����ϴ�.
			/// </summary>
			inline oastring getPlain() { return plain; }
			/// <summary>
			/// �ڵ� �ٹٲ��� �ݿ��� �ؽ�Ʈ �� ���� �����մϴ�. �ּڰ��� 1�Դϴ�.
			/// </summary>
			inline size_t nLines() { return linexy.size(); }
		private:
			Font* font;
			oastring content, plain, input;
			vec4 color;	// ��ü ��
			vec4 ldwh;	// ���� �⺻ ũ��
			vec4 targRect;
			vec2 center;	// ���� �߽�
			mat4 r2r;	// ���簢�� ���� ����
			mat4 additionalTransform;
			std::vector<vec2> linexy;
			AlignH halign;	AlignV valign;
			float rowGap;
			float size;
			const float maxWidth;
			bool rectFixed, fullFit, hasAdditional = false;
		};

		/// <summary>
		/// ���콺�� Ŭ���� �� �ִ� ��ư ��ü�Դϴ�.
		/// �⺻�����δ� ���콺 �ܿ� ������ ���� ������ ���� �Լ�(�ִϸ��̼� Ʈ����)�� public���� �ξ� ������ Ű����� ������ �� �ְ� ������ �����մϴ�.
		/// </summary>
		class Button: public Entity
		{
		public:
			/// <summary>
			/// ��ư�� �����մϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� ��ü�� �̸��Դϴ�.</param>
			/// <param name="ldwh">��ư�� ����� ���簢�� ����(LDWH)�Դϴ�. ���簢���� �ƴ� ������ �ν� ������ �ϰ� �ʹٸ� ����Ͽ� Update()�� �������̵��ؾ� �մϴ�.</param>
			/// <param name="onClick">��ư Ŭ�� �� ���� �Լ��Դϴ�. UniversalFunctor �߻� Ŭ������ ����Ͽ� ����ϸ�, Button ��ü�� �⺻������ ���� ���ڰ� �����ϴ�.</param>
			/// <param name="normal">�⺻ ������ �ִϸ��̼�(�̹���)�Դϴ�.</param>
			/// <param name="onOver">���콺 ���� ������ �ִϸ��̼�(�̹���)�Դϴ�.</param>
			/// <param name="onDown">���콺 ���� ��ư�� ������ ������ ���� ������ ������ �ִϸ��̼�(�̹���)�Դϴ�.</param>
			Button(const EntityKey& key, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal = pAnimation(), pAnimation onOver = pAnimation(), pAnimation onDown = pAnimation());
			/// <summary>
			/// ���콺 Ŀ���� ��ư ���� ��ġ���� ���� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseOver();
			/// <summary>
			/// ���콺 Ŀ���� ��ư���κ��� ������ ���� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseLeft();
			/// <summary>
			/// ���콺 ���� ��ư�� ������ ������ ���� �������� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseDown();
			/// <summary>
			/// ���콺 ��ġ �� Ŭ�� ���¸� �ľ��Ͽ� ��ư�� �����մϴ�. (�����Ӵ� 1ȸ �ڵ� ȣ���)
			/// </summary>
			void Update();
			/// <summary>
			/// onClick �Լ��� �����մϴ�.
			/// </summary>
			inline void setOnClick(UniversalFunctor* n) { onClick = n; }
		protected:
			int st = 0;
			vec4 ldwh;
		private:
			UniversalFunctor* onClick = nullptr;
			bool hasNormal, hasOver, hasDown;
		};

		/// <summary>
		/// ���콺�� Ŭ���� �� �ִ� ��ư ��ü�Դϴ�.
		/// 1ȸ Ŭ���ϸ� ���°� ���ϰ� �ٽ� Ŭ���ϸ� ���� ���·� ���ƿɴϴ�.
		/// �⺻�����δ� ���콺 �ܿ� ������ ���� ������ ���� �Լ�(�ִϸ��̼� Ʈ����)�� public���� �ξ� ������ Ű����� ������ �� �ְ� ������ �����մϴ�.
		/// </summary>
		class ToggleButton : public Entity {
		public:
			ToggleButton(const EntityKey& key, const vec4& ldwh, UIAnimation* normal1 = nullptr, UIAnimation* normal2 = nullptr, UIAnimation* onOver1 = nullptr, UIAnimation* onOver2 = nullptr, UIAnimation* onDown1 = nullptr, UIAnimation* onDown2 = nullptr);
			virtual void onClick1();
			virtual void onClick2();
			void onMouseOver1();
			void onMouseOver2();
			void onMouseDown1();
			void onMouseDown2();
		private:
		};

		/// <summary>
		/// ���콺�� �� �� �ִ� ��ư ��ü�Դϴ�. 1�� ������ �� ����ȭ�� �����մϴ�.
		/// ����ȭ���� ���� �������� ���� 0~1�� float�� �����˴ϴ�.
		/// </summary>
		class Gauge : public Entity {
		public:
			Gauge(const EntityKey& key, const vec4& ldwh, Texture* handle, Texture* bar, int min, int max);
		private:
			union {
				int q;
				float c;
			}value;
		};
	}
}

#endif // !__OA_UI_H__

