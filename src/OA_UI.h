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

	/// <summary>
	/// ���� ����ϴ� UI ��ü�� ��� �ξ����ϴ�. �̰��� ����ϰų� �ڵ带 �����ؼ� Ȱ���� �� ������ ��ü ����� ���� �������� UniversalFunctor�� ���� ������ �ּ���.
	/// UI ��ü�� �⺻������ ���콺�� ����մϴ�. �̰��� ��ġ�� ��ġ�� ��� z-index�� �����ϰ� ��� �����ϸ�, �̿� ���� å���� ������ ���� �ʽ��ϴ�. Entity�� isActive ���� ���� ������ �ּ���.
	/// </summary>
	namespace UI {
		class UIEntity : public Entity {
			friend class Slider;
		public:
			inline UIEntity(const EntityKey& _1, const Transform& _2, bool _3 = false, bool _4 = false) :Entity(_1, _2, _3, _4) { }
			inline UIEntity(const EntityKey& _1, const Transform& _2, pAnimation& _3, bool _4 = false, bool _5 = false) : Entity(_1, _2, _3, _4, _5) {}
			/// <summary>
			/// ��ü�� ���� ������ ������ ��Ȳ���� Ȯ���ϱ� ���� ���Դϴ�.
			/// </summary>
			virtual vec4 clickbox() { return vec4(-1, -1, 1, 1); }
		};

		/// <summary>
		/// �ؽ�Ʈ ��ü�Դϴ�. ���ں��� ũ��/������ ������ �� �ֽ��ϴ�.
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y �Ǵ� a. �빮�ڴ� �������� �ʽ��ϴ�.), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�(��, �ִ� ��� ũ��� 9.99/0.01=999��). ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Է��ϸ� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�. a���� x,y�� ��θ� �����մϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�.</para>
		/// </summary>
		class Text: public UIEntity {
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
		/// �ִϸ��̼��� ���� �ʰ� ������ ��� SCTR �Ű������� vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)���� �־�� �մϴ�.
		/// ��ư�� �̹����δ� UIAnimation �Ǵ� FixedSprite�� ��õ�˴ϴ�.
		/// </summary>
		class Button: public UIEntity
		{
		public:
			/// <summary>
			/// ��ư�� �����մϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� ��ü�� �̸��Դϴ�.</param>
			/// <param name="baseSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�. ��� �Ǵ� �����Ͽ� setModel() ���� �ؼ� 3D�� ��ư�� ������� ��� ����� ������ ������ �����Ƿ� ���� �����ؾ� �մϴ�. �� 2������ x,y scale�̸� �� 2������ x,y �̵��Դϴ�.</param>
			/// <param name="ldwh">��ư�� ����� ���簢�� ����(LDWH)�Դϴ�. ���簢���� �ƴ� ������ �ν� ������ �ϰ� �ʹٸ� ����Ͽ� Update()�� �������̵��ؾ� �մϴ�.</param>
			/// <param name="onClick">��ư Ŭ�� �� ���� �Լ��Դϴ�. UniversalFunctor �߻� Ŭ������ ����Ͽ� ����ϸ�, Button ��ü�� �⺻������ ���� ���ڰ� �����ϴ�. nullptr ���� �� �ƹ� �ൿ�� ���� �ʽ��ϴ�.</param>
			/// <param name="normal">�⺻ ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ ��� ���簢���� �������˴ϴ�.</param>
			/// <param name="onOver">���콺 ���� ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ normal ������ �̹����� �ణ ��ο����ϴ�.</param>
			/// <param name="onDown">���콺 ���� ��ư�� ������ ������ ���� ������ ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ normal ������ �̹����� onOver���� ��ο����ϴ�.</param>
			Button(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal = pAnimation(), pAnimation onOver = pAnimation(), pAnimation onDown = pAnimation());
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
			/// ��ư�� ���簢�� ������ �����մϴ�.
			/// </summary>
			/// <param name="newLDWH">��-��-��-���� ���� ���簢���Դϴ�.</param>
			void move(const vec4& newLDWH);
			inline vec4 clickbox() { return ldwh; }
			/// <summary>
			/// ��ư�� Ŭ������ ���� �Լ��� ȣ���մϴ�.
			/// </summary>
			inline void click() { if (onClick)(*onClick)(); }
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
		/// 1ȸ Ŭ���ϸ� ���°� ���ϰ� �ٽ� Ŭ���ϸ� ���� ���·� ���ƿɴϴ�. ���α׷� �������δ� �ʱ� ���°� off, ���� ���°� on�Դϴ�.
		/// �ִϸ��̼��� ���� �ʰ� ������ ��� SCTR �Ű������� vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)���� �־�� �մϴ�.
		/// �⺻�����δ� ���콺 �ܿ� ������ ���� ������ ���� �Լ�(�ִϸ��̼� Ʈ����)�� public���� �ξ� ������ Ű����� ������ �� �ְ� ������ �����մϴ�.
		/// </summary>
		class ToggleButton : public UIEntity {
		public:
			/// <summary>
			/// ��� ��ư�� �����մϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� ��ü�� �̸��Դϴ�.</param>
			/// <param name="baseSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�. 3D�� ��ư�� ������� ��� ����� ������ ��� ���� �����ؾ� �մϴ�. �� 2������ x,y scale�̸� �� 2������ x,y �̵��Դϴ�.</param>
			/// <param name="ldwh">��ư�� ����� ���簢�� ����(LDWH)�Դϴ�. ���簢���� �ƴ� ������ �ν� ������ �ϰ� �ʹٸ� ����Ͽ� Update()�� �������̵��ؾ� �մϴ�.</param>
			/// <param name="onClick">Ŭ������ �� ���� �Լ��Դϴ�. UniversalFunctor �߻� Ŭ������ ����Ͽ� ����ϸ�, ToggleButton ��ü�� �⺻������ ��ȭ ���� ���°� on�̸� � bool �����Ͱ� ���޵Ǹ�, off�̸� nullptr�� ���޵˴ϴ�. �������� ���� �д� ���� �ƴԿ� �����ϼ���.</param>
			/// <param name="normal1">off, �⺻ ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ ��� ���簢���� �������˴ϴ�.</param>
			/// <param name="normal2">on, �⺻ ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ off ������ �̹����� green ���и� ���� �̹����� �������˴ϴ�.</param>
			/// <param name="onOver1">off, ���콺 ���� ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ off, normal ������ �̹����� �ణ ��ο����ϴ�.</param>
			/// <param name="onOver2">on, ���콺 ���� ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ on, normal ������ �̹����� �ణ ��ο����ϴ�.</param>
			/// <param name="onDown1">off, ���콺 �ٿ� ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ off, normal ������ �̹����� onOver���� ��ο����ϴ�.</param>
			/// <param name="onDown2">on, ���콺 �ٿ� ������ �ִϸ��̼�(�̹���)�Դϴ�. �Է����� ������ on, normal ������ �̹����� onOver���� ��ο����ϴ�.</param>
			ToggleButton(const EntityKey& key, const vec4& baseSCTR, const vec4& ldwh, UniversalFunctor* onClick, pAnimation normal1 = pAnimation(), pAnimation normal2 = pAnimation(), pAnimation onOver1 = pAnimation(), pAnimation onOver2 = pAnimation(), pAnimation onDown1 = pAnimation(), pAnimation onDown2 = pAnimation());
			/// <summary>
			/// ���콺 Ŀ���� ��ư ���� ��ġ���� ���� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseOver(bool isOn);
			/// <summary>
			/// ���콺 Ŀ���� ��ư���κ��� ������ ���� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseLeft(bool isOn);
			/// <summary>
			/// ���콺 ���� ��ư�� ������ ������ ���� �������� �ִϸ��̼��� ���ɴϴ�.
			/// </summary>
			virtual void onMouseDown(bool isOn);
			/// <summary>
			/// ���콺 ��ġ �� Ŭ�� ���¸� �ľ��Ͽ� ��ư�� �����մϴ�. (�����Ӵ� 1ȸ �ڵ� ȣ���)
			/// </summary>
			void Update();
			/// <summary>
			/// ��ư�� ���簢�� ������ �����մϴ�.
			/// </summary>
			/// <param name="newLDWH">��-��-��-���� ���� ���簢���Դϴ�.</param>
			void move(const vec4& newLDWH);
			/// <summary>
			/// onClick �Լ��� �����մϴ�.
			/// </summary>
			inline void setOnClick(UniversalFunctor* n) { onClick = n; }
		protected:
			int st = 0;
			vec4 ldwh;
		private:
			UniversalFunctor* onClick = nullptr;
			bool hasNormal1, hasNormal2, hasOver1, hasOver2, hasDown1, hasDown2;
		};

		/// <summary>
		/// ���콺�� �������� �� �� �ִ� ��ư ��ü�Դϴ�. 1�� ������ �� ����ȭ�� �����մϴ�.
		/// ����ȭ���� ���� �������� ���� 0~1�� float�� �����˴ϴ�.
		/// �ִϸ��̼��� ���� �ʰ� ������ ��� �׿� �ش��ϴ� SCTR �Ű������� vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)���� �־�� �մϴ�.
		/// Gauge�� ������ ��� �� �̸��� �ڿ� "_bar"�� ������ �̸����� �ٸ� ��ü�� �Բ� �����˴ϴ�(Ÿ���� �⺻ Entity�Դϴ�). �̴� Gauge�� ����� �� �ڵ����� ���ŵ˴ϴ�.
		/// �� ������, Gauge�� �����Ͽ� ����� �� �� �ڿ� "_bar"�� ������ �̸��� ����� ��� �װ��� ������ ��ü�� �ƴϸ�, ���� �̷��� ������ ��ü�� ���Ƿ� ������ ��� Gauge�� ���� ���׸����̼� ������ �߻��� �� ������ �����ϼ���.
		/// </summary>
		class GaugeH : public UIEntity {
		public:
			/// <summary>
			/// �������� �����մϴ�. ���� ���� �������� ������ ���� �� �ֽ��ϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ����� �̸��Դϴ�.</param>
			/// <param name="barSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� ���� �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�.</param>
			/// <param name="handleSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� ������ �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�.</param>
			/// <param name="barLdwh">�������� ���밡 �����ϴ� ���簢�� �����Դϴ�.</param>
			/// <param name="handleSize">�������� �������� ũ���Դϴ�. ũ�� 1�� ������ ������ ���̿� �����մϴ�.</param>
			/// <param name="length">�������� ���� �� �ִ� ���� ���Դϴ�. 1 ���ϰ� �ԷµǴ� ��� �������� 0�� 1 ������ float ���� ������ �� �̻��� ��� 0���� �����ϴ� int ���� ������ �˴ϴ�. (ex: length 3�� ��� 0,1,2)</param>
			/// <param name="onScroll">�巡���� �� ȣ��Ǵ� �Լ��Դϴ�. length�� 1 ������ ��� �Ű������� float*���Դϴ�. length�� 2 �̻��� ��� �Ű������� int*���Դϴ�.</param>
			/// <param name="margin">�� ���� �� ���� �����Դϴ�. ������ �������� �߽��� �� ���� �������� ���� �ʽ��ϴ�. �� ���� 0���� �۰ų� 0.495���� ũ�� �ڵ����� �߸��ϴ�.</param>
			/// <param name="isContinuous">true�� ��� �������� �����ӿ� ���� �������� ���� �Լ��� ȣ��ǰ� false�� ��� ���콺�� ���� ������ ���� �Լ��� ȣ��˴ϴ�.</param>
			/// <param name="handle">������ �̹���(�ִϸ��̼�)�Դϴ�.</param>
			/// <param name="bar">���� �̹���(�ִϸ��̼�)�Դϴ�.</param>
			GaugeH(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin = 0.05f, bool isContinuous = false, pAnimation handle = pAnimation(), pAnimation bar = pAnimation());
			~GaugeH();
			/// <summary>
			/// ���콺 ��ġ �� Ŭ�� ���¸� �ľ��Ͽ� �������� �����մϴ�. (�����Ӵ� 1ȸ �ڵ� ȣ���)
			/// </summary>
			void Update();
			void render();
			/// <summary>
			/// �������� ���� ���� �����մϴ�. �Ű������� float������ �̻�(discrete) �������� �����ߴ��� �״�� ����ϸ� �˴ϴ�.
			/// ������ �Ѵ� ���� ������ �ڵ����� �߸��ϴ�.
			/// </summary>
			inline void setValue(float v) { if (length != 1)v /= length - 1; onHolding(leftmost + (rightmost - leftmost) * v); }
		protected:
			void onHolding(float x);

			inline float getLength() { return length; }
			inline float getLeft() { return leftmost; }
			inline float getRight() { return rightmost; }
			inline bool isContin() { return isContinuous; }

			vec4 barArea, handleArea;
			const vec4 baseColor;
			const float margin;
			int st = 0;
		private:
			UniversalFunctor* onScroll = nullptr;
			Entity* bar = nullptr;
			union {
				int q;
				float c;
			}value;
			const float length;
			float leftmost, rightmost;
			bool isContinuous;
		};

		/// <summary>
		/// ���콺�� �������� �� �� �ִ� ��ư ��ü�Դϴ�. 1�� ������ �� ����ȭ�� �����մϴ�.
		/// ����ȭ���� ���� �������� ���� 0~1�� float�� �����˴ϴ�.
		/// �ִϸ��̼��� ���� �ʰ� ������ ��� �׿� �ش��ϴ� SCTR �Ű������� vec4(-1.0f / 1024, -1.0f / 1024, 0, 0)���� �־�� �մϴ�.
		/// Gauge�� ������ ��� �� �̸��� �ڿ� "_bar"�� ������ �̸����� �ٸ� ��ü�� �Բ� �����˴ϴ�(Ÿ���� �⺻ Entity�Դϴ�). �̴� Gauge�� ����� �� �ڵ����� ���ŵ˴ϴ�.
		/// �� ������, Gauge�� �����Ͽ� ����� �� �� �ڿ� "_bar"�� ������ �̸��� ����� ��� �װ��� ������ ��ü�� �ƴϸ�, ���� �̷��� ������ ��ü�� ���Ƿ� ������ ��� Gauge�� ���� ���׸����̼� ������ �߻��� �� ������ �����ϼ���.
		/// </summary>
		class GaugeV : public UIEntity {
		public:
			/// <summary>
			/// �������� �����մϴ�. ���� ���� �������� ������ ���� �� �ֽ��ϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ����� �̸��Դϴ�.</param>
			/// <param name="barSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� ���� �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�.</param>
			/// <param name="handleSCTR">2D ��������Ʈ/�ִϸ��̼��� ��� �� ���� ������ �ִϸ��̼� ��ü ���� ��ǥ sctr���� �Է��ϸ� �˴ϴ�.</param>
			/// <param name="barLdwh">�������� ���밡 �����ϴ� ���簢�� �����Դϴ�.</param>
			/// <param name="handleSize">�������� �������� ũ���Դϴ�. ũ�� 1�� ������ ������ ���� �����մϴ�.</param>
			/// <param name="length">�������� ���� �� �ִ� ���� ���Դϴ�. 1 ���ϰ� �ԷµǴ� ��� �������� 0�� 1 ������ float ���� ������ �� �̻��� ��� 0���� �����ϴ� int ���� ������ �˴ϴ�. (ex: length 3�� ��� 0,1,2)</param>
			/// <param name="onScroll">�巡���� �� ȣ��Ǵ� �Լ��Դϴ�. length�� 1 ������ ��� �Ű������� float*���Դϴ�. length�� 2 �̻��� ��� �Ű������� int*���Դϴ�.</param>
			/// <param name="margin">�� ���� �� ���� �����Դϴ�. ������ �������� �߽��� �� ���� �������� ���� �ʽ��ϴ�. �� ���� 0���� �۰ų� 0.495���� ũ�� �ڵ����� �߸��ϴ�.</param>
			/// <param name="isContinuous">true�� ��� �������� �����ӿ� ���� �������� ���� �Լ��� ȣ��ǰ� false�� ��� ���콺�� ���� ������ ���� �Լ��� ȣ��˴ϴ�.</param>
			/// <param name="handle">������ �̹���(�ִϸ��̼�)�Դϴ�.</param>
			/// <param name="bar">���� �̹���(�ִϸ��̼�)�Դϴ�.</param>
			GaugeV(const EntityKey& key, const vec4& barSCTR, const vec4& handleSCTR, const vec4& barLdwh, const vec2& handleSize, short length, UniversalFunctor* onScroll, float margin = 0.05f, bool isContinuous = false, pAnimation handle = pAnimation(), pAnimation bar = pAnimation());
			~GaugeV();
			void onHolding(float y);
			void Update();
			void render();
			/// <summary>
			/// �������� ���� ���� �����մϴ�. �Ű������� float������ �̻�(discrete) �������� �����ߴ��� �״�� ����ϸ� �˴ϴ�.
			/// ������ �Ѵ� ���� ������ �ڵ����� �߸��ϴ�.
			/// </summary>
			inline void setValue(float v) { if (length != 1)v /= length - 1; onHolding(bottommost + (topmost - bottommost) * v); }
		protected:
			inline float getLength() { return length; }
			inline float getTop() { return topmost; }
			inline float getRight() { return bottommost; }
			inline bool isContin() { return isContinuous; }
			vec4 barArea, handleArea;
			const vec4 baseColor;
			const float margin;
			int st = 0;
		private:
			UniversalFunctor* onScroll = nullptr;
			Entity* bar = nullptr;
			union {
				int q;
				float c;
			}value;
			const float length;
			float topmost, bottommost;
			bool isContinuous;
		};
		
		/// <summary>
		/// ���� ������ UI ��ü���� �����ϸ� ��ġ�� �����Ͽ� �����մϴ�. ������ �ʰ� �� ��ü���� Ŭ�� �� �׼��� ������ �ʽ��ϴ�.
		/// </summary>
		class Slider : public UIEntity {
		public:
			/// <summary>
			/// �����̴��� �����մϴ�. �ʱ⿡�� ���� ��ܿ� �� �پ� �ֽ��ϴ�.
			/// </summary>
			/// <param name="key">��ü �̸��Դϴ�.</param>
			/// <param name="area">�����̴��� ������ ���� �����Դϴ�.</param>
			/// <param name="size">�����̴��� ���� ������ ����/���� ũ���Դϴ�.</param>
			/// <param name="component">�����̴��� ���Ե� ��ü���Դϴ�. ���� ���� ��ü�� ����, �� ��ü���� �����̴��� ���ŵ� �� ��� ���ŵ˴ϴ�. �̴� ���� ���� ��ü���� scene�� entities�� ���Ե��� �ʾƾ� �Ѵٴ� �ǹ̿� �����ϴ�.</param>
			Slider(const EntityKey& key, const vec4& area, const vec2& size, const std::vector<UIEntity*>& component);
			inline vec2 getPos() { return pos; }
			inline vec4 clickbox() { return area; }
			/// <summary>
			/// �����̴��� ������ ��ġ�� �����մϴ�. ���� �� ������ ������ �Ѿ�� �ʵ��� �ڵ� �����˴ϴ�.
			/// </summary>
			void setPos(const vec2&);
			void Update();	// ���� �̽�: �����ϱ�
			void render();	// clickbox�� ��ġ�� �͸� render
			~Slider();
		private:
			vec4 area;
			vec2 size;
			vec2 pos;
			std::vector<UIEntity*> component;
		};

		/// <summary>
		/// �ؽ�Ʈ�� �����ϴ� ��Ӵٿ� ����Ʈ�Դϴ�.
		/// </summary>
		class Dropdown :public UIEntity {
			
		};

		/// <summary>
		/// �ؽ�Ʈ�� �Է¹޴� �������̽��Դϴ�.
		/// </summary>
		class InputField : public UIEntity {

		};
	}
}

#endif // !__OA_UI_H__

