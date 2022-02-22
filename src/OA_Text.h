/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_TEXT_H__
#define __OA_TEXT_H__

#include <vector>
#include <string>
#include <set>
#include <map>
#include "oaglem.h"

#ifndef OA_NO_UNICODE16
using oastring = std::u16string;
using oachar = char16_t;
#else
using oastring = std::string;
using oachar = char;
#endif

/// <summary>
/// �� ����� �⺻������ UTF-16 ���ڿ�, �� std::u16string�� ����� ���Դϴ�. �ٽ� ����, �ƽ�Ű ���� ��/���� ���� ��� ��Ʈ������ �� �ڸ��� �����մϴ�.
/// ���ͷ��� u"���ڿ�" Ȥ�� u'��'�̴�, �� ��ȯ ������ ���� �� ������ �ּ���.
/// OA_NO_UNICODE16 ��ũ�θ� �����ϸ� std::string�� ����� �� �ֽ��ϴ�. ��, �̶� �ƽ�Ű ���� �� ���ڰ� �����ϴ� ��� �ؽ�Ʈ ����� ���ϴ� ��� ���� ���� ���Դϴ�.
/// </summary>
namespace onart {
	enum class Align { CENTER, LEFT, RIGHT };
	/// <summary>
	/// Ʈ��Ÿ�Ը� ��� �����մϴ�.
	/// </summary>
	class Font
	{
	public:
		/// <summary>
		/// ��Ʈ ������ �ҷ��ɴϴ�. �ҷ��� ������ �̸��� ���ؼ� ������ �� �ֽ��ϴ�. �� ���α׷����� ����ϴ� ����� stb_truetype�� ���� ���̸� ���� �ʽ��ϴ�.
		/// ����, �� ��� �κ��� �� �ٸ������� ������ ��ȿ�� Ʈ��Ÿ�� ������ �ƴ� ��� ������ �߻��� �� �����Ƿ� ������� ���ñ� �ٶ��ϴ�.
		/// </summary>
		/// <param name="file">�ҷ��� ���� �̸��Դϴ�.</param>
		/// <param name="v">������ ����� ������ �����Դϴ�. ��ġ�� �� �־ ���� �� �ε����� ������ ��� ��Ʈ���� �״�� ���ļ� �־ �˴ϴ�.</param>
		/// <param name="resolution">�ؽ�ó ��Ʈ���� �ػ��Դϴ�. ���� Ŭ���� �۾��� �� �Ų����� ��µǰ����� �׸�ŭ �׷��� �뷮�� ���� �����ϰ� �������Ƿ� �ʿ��� ��ŭ�� ����ϴ� ���� �����ϴ�.</param>
		/// <param name="name">���α׷� ������ ����� �̸��Դϴ�. �Է����� ���� ��� ���� �̸��� ����մϴ�.</param>
		static Font* load(const std::string& file, const std::vector<oachar>& v, float resolution = 48, const std::string& name = "");
		/// <summary>
		/// ��Ʈ ���Ͽ� �ش��ϴ� �����͸� �޸𸮿��� �ҷ��ɴϴ�. �ҷ��� ������ �̸��� ���ؼ� ������ �� �ֽ��ϴ�. �� ���α׷����� ����ϴ� ����� stb_truetype�� ���� ���̸� ���� �ʽ��ϴ�.
		/// ����, �� ��� �κ��� �� �ٸ������� ������ ��ȿ�� Ʈ��Ÿ�� ������ �ƴ� ��� ������ �߻��� �� �����Ƿ� ������� ���ñ� �ٶ��ϴ�.
		/// </summary>
		/// <param name="dat">�޸� ���� �ּ�</param>
		/// <param name="name">���α׷� ������ ����� �̸��Դϴ�.</param>
		/// <param name="v">������ ����� ������ �����Դϴ�. ��ġ�� �� �־ ���� �� �ε����� ������ ��� ��Ʈ���� �״�� ���ļ� �־ �˴ϴ�.</param>
		/// <param name="resolution">�ؽ�ó ��Ʈ���� �ػ��Դϴ�. ���� Ŭ���� �۾��� �� �Ų����� ��µǰ����� �׸�ŭ �׷��� �뷮�� ���� �����ϰ� �������Ƿ� �ʿ��� ��ŭ�� ����ϴ� ���� �����ϴ�.</param>
		static Font* load(const unsigned char* dat, const std::string& name, const std::vector<oachar>& v, float resolution = 48);
		/// <summary>
		/// �ҷ��� ��Ʈ ���Ͽ� ���� ���� �޸𸮿��� �����մϴ�.
		/// </summary>
		/// <param name="name">������ �̸�</param>
		static void drop(const std::string& name);
		/// <summary>
		/// ��Ʈ ������ �����͸� �����մϴ�.
		/// </summary>
		static Font* get(const std::string& name);
		/// <summary>
		/// ���ϴ� ������ �ؽ�Ʈ�� �׸��ϴ�.
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y �Ǵ� a. �빮�ڴ� �������� �ʽ��ϴ�.), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�(��, �ִ� ��� ũ��� 9.99/0.01=999��). ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Է��ϸ� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�. a���� x,y�� ��θ� �����մϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�. �� �� �ڵ� �ٹٲ��� ���� �������� �������� �ʽ��ϴ�. ���߿� �������� n�� ������ �������� ������ �����Դϴ�.</para>
		/// </summary>
		/// <param name="content">ǥ���� �����Դϴ�.</param>
		/// <param name="group">��ü ���ڿ��� ��ġ�� �ȼ� ������ �ٷ����µ�, �̸� ���ϴ� ��ġ/ũ��� ��ȯ�ϴ� ����Դϴ�. getRectNLine()���� ���� ���簢�� ������ ��ǥ ���簢�� ������ �̿��Ͽ� ���մϴ�.</param>
		/// <param name="lineXY">�� ������ �������Դϴ�. getRectNLine()���� �ڵ����� ���˴ϴ�.</param>
		/// <param name="color">��ü �ؽ�Ʈ�� ������ �����մϴ�. �ؽ�Ʈ ��ü���� ������ ������� ���к� ������ ����˴ϴ�.</param>
		void draw(const oastring& content, const mat4& group, const std::vector<vec2>& lineXY, const vec4& color = 1);
		/// <summary>
		/// �־��� ��ġ�� �߽����� ������ ũ���� �ؽ�Ʈ�� �׸��ϴ�. (�ؽ�Ʈ�� ���̿� ���� ���� �� ������ ũ�Ⱑ ������)
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y �Ǵ� a. �빮�ڴ� �������� �ʽ��ϴ�.), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�. 
		/// ����� ũ���� ������ ���ϴ��� �⺻���� 1.0�� ũ��� ������ �ʽ��ϴ�. ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Է��ϸ� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�. a���� x,y�� ��θ� �����մϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�. �� �� �ڵ� �ٹٲ��� ���� �������� �������� �ʽ��ϴ�. ���߿� �������� n�� ������ �������� ������ �����Դϴ�.</para>
		/// </summary>
		/// <param name="content">ǥ���� �����Դϴ�.</param>
		/// <param name="group">������ �⺻ ũ��(���簢�� LDWH)�Դϴ�. getRectNLine()���� �ڵ����� ���˴ϴ�.</param>
		/// <param name="lineXY">�� ������ �������Դϴ�. getRectNLine()���� �ڵ����� ���˴ϴ�.</param>
		/// <param name="center">�߽� ��ǥ�Դϴ�.</param>
		/// <param name="size">������ ũ���Դϴ�. �ٸ� ��Ʈ�� ���ؼ��� �� ���� ���ٰ� �Ͽ� ���� ũ�Ⱑ ������� �ʽ��ϴ�.</param>
		/// <param name="color">��ü �ؽ�Ʈ�� ������ �����մϴ�. �ؽ�Ʈ ��ü���� ������ ������� ���к� ������ ����˴ϴ�.</param>
		void draw(const oastring& content, const vec4& group, const std::vector<vec2>& lineXY, const vec2& center = 0, float size = 1, const vec4& color = 1);
		/// <summary>
		/// �־��� ��ġ�� �߽����� ������ ũ���� �ؽ�Ʈ�� �׸���, �߰������� ��ȯ�� �����մϴ�. (�ؽ�Ʈ�� ���̿� ���� ���� �� ������ ũ�Ⱑ ������)
		/// </summary>
		/// <param name="content">ǥ���� �����Դϴ�.</param>
		/// <param name="group">������ �⺻ ũ��(���簢�� LDWH)�Դϴ�. getRectNLine()���� �ڵ����� ���˴ϴ�.</param>
		/// <param name="lineXY">�� ������ �������Դϴ�. getRectNLine()���� �ڵ����� ���˴ϴ�.</param>
		/// <param name="transform">�߰��� ������ ��ȯ�Դϴ�. ȸ����ȯ�̳� ũ�⺯ȯ ���� �����Ѵٸ�, �߽���ǥ�� �̵����� �ʵ��� �ϴ� ���� �� �Լ� �ۿ��� �ؾ� �� ���Դϴ�. (mat4�� ���� ��ȯ�� �ִ� ���� �ƴϱ� ������ �Լ� �ȿ��� ������ �� ����)</param>
		/// <param name="center">�߽� ��ǥ�Դϴ�.</param>
		/// <param name="size">������ ũ���Դϴ�. �ٸ� ��Ʈ�� ���ؼ��� �� ���� ���ٰ� �Ͽ� ���� ũ�Ⱑ ������� �ʽ��ϴ�.</param>
		/// <param name="color">��ü �ؽ�Ʈ�� ������ �����մϴ�. �ؽ�Ʈ ��ü���� ������ ������� ���к� ������ ����˴ϴ�.</param>
		void draw(const oastring& content, const vec4& group, const std::vector<vec2>& lineXY, const mat4& transform, const vec2& center = 0, float size = 1, const vec4& color = 1);

		/// <summary>
		/// �� �ҷ��� ��Ʈ�� ���Ͽ� �ش� ������ �����ϴ� ����� ����, �׸��� �� ���� ��� ���������� �����մϴ�. �� ���� draw()���� ���˴ϴ�.
		/// ���簢���� ���� ����� (0,0)���� �����˴ϴ�.
		/// ���� �� �� ũ�� ���濡 ���ؼ��� draw() ������ ������ �ּ���.
		/// </summary>
		/// <param name="content">������ ����</param>
		/// <param name="lineXY">�� ���� ������(���� ����/�Ʒ���)�� ���ϴ�. ��� ���� �ʾҴ� ��� �ڵ����� ���� ���� ���ϴ�.</param>
		/// <param name="align">���� ���� �����Դϴ�. ���/����/�������� �ֽ��ϴ�.</param>
		/// <param name="rowGap">�ణ�� �����մϴ�. �� ���� ���� ���α׷����� �����ϴ� �Ͱ� ������ ������ �ǹ����� �ʽ��ϴ�. ��Ʈ�� ���� �ٸ� �� �����Ƿ�, ��ȣ�ϴ� ��ŭ ���� ������ �ֽô� ���� �����ϴ�.</param>
		/// <returns>������ �����ϴ� ���簢��(L-D-W-H)�Դϴ�. draw()���� ���˴ϴ�.</returns>
		vec4 getRectNLine(const oastring& content, std::vector<vec2>& lineXY, Align align = Align::CENTER, float rowGap = 1);
	private:
		struct charTex {
			unsigned id;
			float size[2];
			float bearing[2];
			float advance;
		};
		Font(void* info, const std::set<oachar>& vset, float resolution);
		~Font();
		std::map<oachar, charTex> txs;
		const float resolution;
		static std::map<std::string, Font*> list;

		static float parseSize(const oastring&, int start);
		static vec4 parseColor(const oastring&, int start);

		void cdraw(const oastring& content, const std::vector<vec2>& lineXY, const vec4& color);
	};

}

#endif // !__OA_TEXT_H__
