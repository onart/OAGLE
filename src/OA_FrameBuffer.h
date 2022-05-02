#ifndef __OA_FRAMEBUFFER_H__
#define __OA_FRAMEBUFFER_H__

#include "oaglem.h"
#include <map>

namespace onart {
	/// <summary>
	/// ȭ�� �ܿ� ���� �׸� �����ӹ��۸� �����մϴ�.
	/// �����ӹ��۴� �̸��� ���ؼ� ����/�����ϴµ�, string::c_str()�� ���� ������ ���ڿ��� ������ �� ������ �����Ͻñ� �ٶ��ϴ�.
	/// </summary>
	class FrameBuffer {
	public:
		/// <summary>
		/// �����ӹ��� ÷�ι��� �Ӽ��� �����մϴ�.
		/// NONE: ÷�ι��� ������� �ʽ��ϴ�. ��� ���� NONE�� �� �����ϴ�.
		/// RW: �ؽ�ó�� ����ϸ�, �׸� �����͸� �о�� �� �ֽ��ϴ�.
		/// W: �������۸� ����ϸ�, �׸� �����͸� �о�� �� �����ϴ�. �Ϲ������� �ؽ�ó���� �����Ƿ� ����ϵ� ���� ���� ���� ���� ��� W�� ����մϴ�.
		/// </summary>
		enum class ATTACHMENT { NONE, RW, W };
		/// <summary>
		/// �����ӹ��۸� �����մϴ�. �̸��� ��ġ�� ��� �ƹ��͵� ���� �ʽ��ϴ�.
		/// �����ӹ��۸� ������ ���Ŀ��� �ڵ����� �⺻ �����ӹ��۰� ���ε�ǹǷ� ȥ���� ���Ϸ��� �ʿ��� �����ӹ��۴� ������ �� �ٷ� ����ñ� �ٶ��ϴ�.
		/// </summary>
		/// <param name="name">������ �����ӹ��� �̸��Դϴ�.</param>
		/// <param name="width">�����ӹ��� ���Դϴ�.</param>
		/// <param name="height">�����ӹ��� �����Դϴ�.</param>
		/// <param name="colorBuffer">�� ���� ��� �����Դϴ�.</param>
		/// <param name="depthBuffer">���� ���� ��� �����Դϴ�.</param>
		/// <param name="stencilBuffer">���ٽ� ���� ��� �����Դϴ�.</param>
		static void make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer = ATTACHMENT::NONE, unsigned func = 0, unsigned ref = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		/// <summary>
		/// ���� �����ӹ��۸� �����մϴ�.
		/// </summary>
		static void drop(const char* name);
		/// <summary>
		/// �׸� �����ӹ��۸� ����(���ε�)�ϰ� �װ��� ����ϴ� ���۸� Ŭ�����մϴ�. NULL �Է��� ������ ��� GLFW���� ������ �⺻ �����ӹ���, �� ȭ������ ��ȯ�˴ϴ�.
		/// use() ȣ�� �Ŀ��� render ����� �ش� �����ӹ��ۿ� �׸��� �˴ϴ�.
		/// </summary>
		static void use(const char* name = nullptr);
	private:
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth, ATTACHMENT stencil);
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth);
		~FrameBuffer();

		void setViewport();

		static std::map<const char*, FrameBuffer*> fbs;

		const unsigned width, height;
		const ATTACHMENT CDS[3];
		unsigned fbo;
		unsigned color, depth, stencil;
	};
}

#endif // !OA_FRAMEBUFFER_H__

