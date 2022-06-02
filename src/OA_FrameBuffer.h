/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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
		/// ���ٽ� ���� ÷�ι��� �뵵�� �������̹Ƿ�, ����� ��� �ݵ�� ���� ���ۿ� ���� ���·θ� ���Ǹ� NONE�� �ƴ� �ɼ��� �־��� ��� depthBuffer�� �־��� �Ͱ� �����ϰ� �־����ϴ�.
		/// ��, ���� ���۰� NONE�̰� ���ٽ� ���۰� �����ϴ� ���� �����մϴ�.
		/// </summary>
		/// <param name="name">������ �����ӹ��� �̸��Դϴ�.</param>
		/// <param name="width">�����ӹ��� ���Դϴ�.</param>
		/// <param name="height">�����ӹ��� �����Դϴ�.</param>
		/// <param name="colorBuffer">�� ���� ��� �����Դϴ�.</param>
		/// <param name="depthBuffer">���� ���� ��� �����Դϴ�.</param>
		/// <param name="stencilBuffer">���ٽ� ���� ��� �����Դϴ�.</param>
		/// <param name="func">glStencilFunc()�� �μ�(��� ����)</param>
		/// <param name="ref">glStencilFunc()�� �μ�(�� ��)</param>
		/// <param name="mask">glStencilFunc()�� �μ�(��ȿ ��Ʈ ����)</param>
		/// <param name="sfail">glStencilOp()�� �μ�(���ٽ� ���� �� ����)</param>
		/// <param name="dpfail">glStencilOp()�� �μ�(���ٽ� ���, ���� ���� �� ����)</param>
		/// <param name="dppass">glStencilOp()�� �μ�(���ٽǰ� ���� ��� �� ����)</param>
		static void make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer = ATTACHMENT::NONE, unsigned func = 0, int ref = 0, unsigned mask = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		/// <summary>
		/// ����Ʈ �׸��ڸ� ���Ͽ�, ť��� ���� �ؽ�ó�� ���� �����ӹ��۸� �����մϴ�.
		/// </summary>
		/// <param name="name">������ �̸��Դϴ�.</param>
		/// <param name="width">�����ӹ��� ���Դϴ�.</param>
		/// <param name="height">�����ӹ��� �����Դϴ�.</param>
		static void makeCube(const char* name, unsigned width, unsigned height);
		/// <summary>
		/// ���� �����ӹ��۸� �����մϴ�.
		/// </summary>
		static void drop(const char* name);
		/// <summary>
		/// �׸� �����ӹ��۸� ����(���ε�)�ϰ� �װ��� ����ϴ� ���۸� Ŭ�����մϴ�. NULL �Է��� ������ ��� GLFW���� ������ �⺻ �����ӹ���, �� ȭ������ ��ȯ�˴ϴ�.
		/// use() ȣ�� �Ŀ��� render ����� �ش� �����ӹ��ۿ� �׸��� �˴ϴ�.
		/// </summary>
		static void use(const char* name = nullptr);
		/// <summary>
		/// �����ӹ��� �����͸� ���ɴϴ�. �̰��� �ؽ�ó�� ��� �Լ��� ���� ���ɴϴ�.
		/// </summary>
		inline static const FrameBuffer* get(const char* name) { return fbs[name]; }
		/// <summary>
		/// ���� ������ ���ۿ��� �� ������ ���� ���� �� �ִ� ��� �о� �ɴϴ�. ���ϰ��� �����Ҵ�� �迭 �ּ��Դϴ�.
		/// �д� ������ ū ��� glGetTextureSubImage�� �� ���� ������ �˷��� �ֽ��ϴ�. �̴� �⺻������ GL 4.6�� ����ϴ� �� ������ �⺻ ���ÿ� ���ϸ� ��� ����������
		/// �⺻������ ���⼭�� �������� �ʽ��ϴ�.
		/// �ȼ� ���� �ϴ��� ��ǥ�� (0,0)�Դϴ�.
		/// �� ���۸� ������� �ʴ� ���� �˻����� ������ �����ϼ���.
		/// �� �����ʹ� �� ������ RGBA �������� �迭�˴ϴ�. ���� 4���� nvec ������ �迭 ĳ������ �����մϴ�.
		/// ��, �� �����Ҵ�� �ּҴ� new�� ���� ���� ���� �Ҵ��̹Ƿ� ���� �� �����ϼ���.
		/// ������ Ÿ���� (unsigned) char, (unsigned) short, (unsigned) int, float�Դϴ�.
		/// �̰��� �����ϼ���. https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
		/// </summary>
		template <class T> static T* readColorValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// ���� ������ ���ۿ��� ���� ������ ���� ���� �� �ִ� ��� �о� �ɴϴ�. ���� 0�� 1 ������ �Ǽ��Դϴ�.
		/// �ȼ� ���� �ϴ��� ��ǥ�� (0,0)�Դϴ�.
		/// ���ϵǴ� �����Ҵ�� �ּҴ� new�� ���� ���� ���� �Ҵ��̹Ƿ� ���� �� �����ϼ���.
		/// </summary>
		static float* readDepthValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// ���� ������ ���ۿ��� ���� ������ ���� ���� �� �ִ� ��� �о� �ɴϴ�. ���� 0�� 1 ������ �Ǽ��Դϴ�.
		/// �ȼ� ���� �ϴ��� ��ǥ�� (0,0)�Դϴ�.
		/// ���ϵǴ� �����Ҵ�� �ּҴ� new�� ���� ���� ���� �Ҵ��̹Ƿ� ���� �� �����ϼ���.
		/// </summary>
		static unsigned char* readStencilValue(unsigned left, unsigned down, unsigned width = 1, unsigned height = 1);
		/// <summary>
		/// �� �����ӹ����� ���� �ؽ�ó ������Ʈ�� �����մϴ�.
		/// ���� ��� 0�� �����մϴ�.
		/// </summary>
		inline unsigned getColorTex() const { return CDS[0] == ATTACHMENT::RW ? color : 0; }
		/// <summary>
		/// �� �����ӹ����� ���� Ȥ�� ����+���ٽ� �ؽ�ó ������Ʈ�� �����մϴ�.
		/// ���� ��� 0�� �����մϴ�.
		/// </summary>
		inline unsigned getDepthTex() const { return CDS[1] == ATTACHMENT::RW ? depth : 0; }
		/// <summary>
		/// �� �����ӹ����� ���ٽ� �ؽ�ó ������Ʈ�� �����մϴ�.
		/// ���� �ؽ�ó�� ���� ���ٽ� �ؽ�ó�� ���� ������ getDepthTex() ��� �̰��� ����ؾ� �մϴ�.
		/// </summary>
		inline unsigned getStencilTex() const { return CDS[2] == ATTACHMENT::RW ? depth : 0; }
	private:
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth, ATTACHMENT stencil, unsigned func = 0, int ref = 0, unsigned mask = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth);
		FrameBuffer(unsigned width, unsigned height);
		~FrameBuffer();

		void setViewport();

		static std::map<const char*, FrameBuffer*> fbs;

		const unsigned width, height;
		const ATTACHMENT CDS[3];
		unsigned fbo;
		unsigned color, depth;
		unsigned stFunc, stMask, sfail, dpfail, dppass;
		int stRef;
	};
}

#endif // !OA_FRAMEBUFFER_H__

