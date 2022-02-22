#ifndef __OA_UI_H__
#define __OA_UI_H__
#include "OA_Entity.h"
#include "OA_Text.h"

namespace onart {
	class Font;
	namespace UI {
		/// <summary>
		/// �ؽ�Ʈ ��ü�Դϴ�. ���ں��� ũ��/������ ������ �� �ֽ��ϴ�.
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y �Ǵ� a. �빮�ڴ� �������� �ʽ��ϴ�.), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�(��, �ִ� ��� ũ��� 9.99/0.01=999��). ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Է��ϸ� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�. a���� x,y�� ��θ� �����մϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�. �� �� �ڵ� �ٹٲ��� ���� �������� �������� �ʽ��ϴ�. ���߿� �������� n�� ������ �������� ������ �����Դϴ�.</para>
		/// </summary>
		class Text: public Entity {
		public:
			/// <summary>
			/// �ؽ�Ʈ ��ü�� �����մϴ�. Ư�� ���簢�� ������ �����˴ϴ�. �̸� �׸� ���� UI�� ����ϱ⿡ �����մϴ�.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� �ؽ�Ʈ ��ü�� �̸��Դϴ�.</param>
			/// <param name="font">�ؽ�Ʈ�� ����Դϴ�. ���� Ȥ�� �޸𸮿��� �ҷ��� �� �ֽ��ϴ�.</param>
			/// <param name="content">�ؽ�Ʈ�� ���� �� ���ں� ũ��/�����Դϴ�. </param>
			/// <param name="ldwh">�ؽ�Ʈ�� �� ���簢��(L-D-W-H)�� �°� ����մϴ�.</param>
			/// <param name="maxWidth">�ִ� ���Դϴ�. ���簢���� ������ ������ �ƴ�, �ҷ��� ��Ʈ�� ���� �ȼ� �����Դϴ�. ���� �����ϸ鼭 ������ ���ߴ� ���� ���ϰ� ����˴ϴ�.</param>
			/// <param name="fullFit">true�� ��� �־��� ���簢���� ��Ȯ�� �°� ����մϴ�. ��, ������ ���� ������ �������� �ʽ��ϴ�.</param>
			/// <param name="align">���� �����Դϴ�. ���/����/������ �ɼǸ� �����մϴ�.</param>
			/// <param name="rowGap">�� �����Դϴ�. ���� ���α׷���� ������ ������ �����Ǵ� ���� �ƴϹǷ� ���� ������ �ֽñ� �ٶ��ϴ�.</param>
			/// <param name="color">��ü�� ���Դϴ�.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec4& ldwh, float maxWidth = INF, bool fullFit = false, Align align = Align::CENTER, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// �ؽ�Ʈ ��ü�� �����մϴ�. ���簢�� ������ �������� �ʰ� Ư�� ������ �߽����� �մϴ�. �̸� �׸� ���� �� ��ü���� �ؽ�Ʈ�� ����� �� ����ϱ⿡ �����մϴ�. �� ��� Camera::world2screen() �Լ��� ������ �ּ���.
			/// </summary>
			/// <param name="key">���α׷� ������ ���� �ؽ�Ʈ ��ü�� �̸��Դϴ�.</param>
			/// <param name="font">�ؽ�Ʈ�� ����Դϴ�. ���� Ȥ�� �޸𸮿��� �ҷ��� �� �ֽ��ϴ�.</param>
			/// <param name="content">�ؽ�Ʈ�� ���� �� ���ں�/ũ��/�����Դϴ�.</param>
			/// <param name="center">�߽��� �Ǵ� ��ǥ�Դϴ�. ���� ȭ�鿡�� ����� (0,0)�̰� ��Ⱦ �� ª�� ���̰� 2�Դϴ�.</param>
			/// <param name="align">���� �����Դϴ�. ���/����/������ �ɼǸ� �����մϴ�. Ư���� ���� ������ ������ �ؽ�Ʈ�� �ƴ� ��� ��� ���İ� �����մϴ�.</param>
			/// <param name="size">�ؽ�Ʈ�� ũ���Դϴ�. �ٸ� ��Ʈ��� ���� ���� ���� ũ��� ������� �ʽ��ϴ�.</param>
			/// <param name="rowGap">�� �����Դϴ�. ���� ���α׷���� ������ ������ �����Ǵ� ���� �ƴϹǷ� ���� ������ �ֽñ� �ٶ��ϴ�.</param>
			/// <param name="color">��ü�� ���Դϴ�.</param>
			Text(const EntityKey& key, Font* font, const oastring& content, const vec2& center, Align align = Align::CENTER, float size = 1, float rowGap = 1, const vec4& color = 1);
			/// <summary>
			/// �־��� ���ڿ��� ������ �ٲߴϴ�. ���ں� ũ��/���� �ɼ��� �����ڸ� �����ϼ���.
			/// </summary>
			void setContent(const oastring&);
			/// <summary>
			/// ���� ������ �����մϴ�. ���/����/�����ʸ� �����մϴ�.
			/// </summary>
			void setAlign(Align);
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
		private:
			Font* font;
			oastring content, plain;
			vec4 color;	// ��ü ��
			vec4 ldwh;	// ���� �⺻ ũ��
			vec2 center;	// ���� �߽�
			mat4 r2r;	// ���簢�� ���� ����
			mat4 additionalTransform;
			std::vector<vec2> linexy;
			Align align;
			float rowGap;
			float size;
			bool rectFixed, hasAdditional = false;
		};
	}
}

#endif // !__OA_UI_H__

