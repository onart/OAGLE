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

constexpr float OA_FONT_RESOLUTION = 48;	// ��Ʈ �ؽ�ó�� ���� ����(px)�Դϴ�. ũ�� ���� �� ���� ȭ���� ������ �ǰ�����, ó�� �ӵ� �� GPU �޸� ��뷮�� �����ϹǷ� �ʿ��� ��ŭ�� ����ϴ� ���� �����ϴ�.

/// <summary>
/// �� ����� �⺻������ UTF-16 ���ڿ�, �� std::u16string�� ����� ���Դϴ�. �ٽ� ����, �ƽ�Ű ���� ��/���� ���� ��� ��Ʈ������ �� �ڸ��� �����մϴ�.
/// ���ͷ��� u"���ڿ�" Ȥ�� u'��'�̴�, �� ��ȯ ������ ���� �� ������ �ּ���.
/// OA_NO_UNICODE16 ��ũ�θ� �����ϸ� std::string�� ����� �� �ֽ��ϴ�. ��, �̶� �ƽ�Ű ���� �� ���ڰ� �����ϴ� ��� �ؽ�Ʈ ����� ���ϴ� ��� ���� ���� ���Դϴ�.
/// </summary>
namespace onart {
	/// <summary>
	/// Ʈ��Ÿ�Ը� ��� �����մϴ�.
	/// </summary>
	class Font
	{
	public:
		enum class Align { CENTER, LEFT, RIGHT };
		/// <summary>
		/// ��Ʈ ������ �ҷ��ɴϴ�. �ҷ��� ������ �̸��� ���ؼ� ������ �� �ֽ��ϴ�. �� ���α׷����� ����ϴ� ����� stb_truetype�� ���� ���̸� ���� �ʽ��ϴ�.
		/// ����, �� ��� �κ��� �� �ٸ������� ������ ��ȿ�� Ʈ��Ÿ�� ������ �ƴ� ��� ������ �߻��� �� �����Ƿ� ������� ���ñ� �ٶ��ϴ�.
		/// </summary>
		/// <param name="file">�ҷ��� ���� �̸��Դϴ�.</param>
		/// <param name="v">������ ����� ������ �����Դϴ�. ��ġ�� �� �־ ���� �� �ε����� ������ ��� ��Ʈ���� �״�� ���ļ� �־ �˴ϴ�.</param>
		/// <param name="name">���α׷� ������ ����� �̸��Դϴ�. �Է����� ���� ��� ���� �̸��� ����մϴ�.</param>
		static Font* load(const std::string& file, const std::vector<oachar>& v, const std::string& name = "");
		/// <summary>
		/// ��Ʈ ���Ͽ� �ش��ϴ� �����͸� �޸𸮿��� �ҷ��ɴϴ�. �ҷ��� ������ �̸��� ���ؼ� ������ �� �ֽ��ϴ�. �� ���α׷����� ����ϴ� ����� stb_truetype�� ���� ���̸� ���� �ʽ��ϴ�.
		/// ����, �� ��� �κ��� �� �ٸ������� ������ ��ȿ�� Ʈ��Ÿ�� ������ �ƴ� ��� ������ �߻��� �� �����Ƿ� ������� ���ñ� �ٶ��ϴ�.
		/// </summary>
		/// <param name="dat">�޸� ���� �ּ�</param>
		/// <param name="name">���α׷� ������ ����� �̸��Դϴ�.</param>
		/// /// <param name="v">������ ����� ������ �����Դϴ�. ��ġ�� �� �־ ���� �� �ε����� ������ ��� ��Ʈ���� �״�� ���ļ� �־ �˴ϴ�.</param>
		static Font* load(const unsigned char* dat, const std::string& name, const std::vector<oachar>& v);
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
		/// �־��� ������ �ؽ�Ʈ�� �׸��ϴ�.
		/// <para>ũ��: ũ��� ���簢���� �µ��� �����µ�, �ؽ�Ʈ �ȿ��� ����� ũ�⸦ �ٸ��� �����ϰ��� �Ѵٸ� \a�� ������(x �Ǵ� y), �׸��� ���� �κ� 1�ڸ� �Ҽ� �κ� 2�ڸ� �Ǽ��� �Է��մϴ�. ���� ���,
		/// u"�ȳ�\ax2.00�ϼ�\ay0.40��"��� �Ԏǰ��� "�ȳ�"�� �� ���ڰ� ũ�� 1x1�̶�� �� �� "�ϼ�"�� �� ���� ũ��� 2x1, "��"�� ũ��� 2x0.4�� �˴ϴ�. \a �� 5�ڸ��� ��ȿ�� ���� �ƴ� ��� ���� ũ��� ������ ������
		/// 5�ڸ���ŭ�� ���õ˴ϴ�.</para>
		/// <para>��: �⺻������ ����̸�, content ���뿡�� \b�� 16�� �ڵ�(RGBA ��)�� �̿��Ͽ� ���� ������ �� �ֽ��ϴ�. ���� ���, u"�ȳ�\bff0000ff�ϼ�\b00ff00ff��"��� �Է��ϸ�
		/// "�ȳ�"�� �Ͼ�, "�ϼ�"�� ����, "��"�� �ʷϻ����� ��µ˴ϴ�. ���� \b �� 8�ڸ��� ��ȿ���� ���� ���� ������ ��� ���� ������ ������ ������ �����մϴ�.</para>
		/// <para>�ٹٲ�: \n�� ���� �ٲߴϴ�. �� �� �ڵ� �ٹٲ��� ���� �������� �������� �ʽ��ϴ�. ���߿� �������� n�� ������ �������� ������ �����Դϴ�.</para>
		/// </summary>
		/// <param name="content">ǥ���� �����Դϴ�.</param>
		/// <param name="ldwh">ǥ���� ���簢�� ����(����-�Ʒ�-��-����)�Դϴ�. ������ ȭ�� ����� (0,0)�̸� ǥ�� ȭ�� ��/���� �� ª�� ���� ���̰� 2�� �˴ϴ�.</param>
		/// <param name="align">������ ���� �����Դϴ�.</param>
		/// <param name="fullFit">true�� ������ ��� ������ ������ �������� �ʰ� ���簢�� ������ ���� ä��ϴ�. �� ���� ��� ������ ������ �����ϸ鼭 �ִ��� ä��ϴ�.</param>
		void draw(const oastring& content, const vec4& ldwh, Align align = Align::CENTER, bool fullFit = false);
	private:
		struct charTex {
			unsigned id;
			int size[2];
			int bearing[2];
			int advance;
		};
		Font(void* info, const std::set<oachar>& vset);
		~Font();
		std::map<oachar, charTex> txs;
		static std::map<std::string, Font*> list;
	};

}

#endif // !__OA_TEXT_H__
