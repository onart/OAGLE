#ifndef __OA_FRAMEBUFFER_H__
#define __OA_FRAMEBUFFER_H__

#include "oaglem.h"
#include <map>

namespace onart {
	/// <summary>
	/// 화면 외에 무언가 그릴 프레임버퍼를 생성합니다.
	/// 프레임버퍼는 이름을 통해서 접근/제거하는데, string::c_str()과 같이 동적인 문자열은 인정될 수 없으니 주의하시기 바랍니다.
	/// </summary>
	class FrameBuffer {
	public:
		/// <summary>
		/// 프레임버퍼 첨부물의 속성을 결정합니다.
		/// NONE: 첨부물을 사용하지 않습니다. 모든 값이 NONE일 수 없습니다.
		/// RW: 텍스처를 사용하며, 그린 데이터를 읽어올 수 있습니다.
		/// W: 렌더버퍼를 사용하며, 그린 데이터를 읽어올 수 없습니다. 일반적으로 텍스처보다 빠르므로 사용하되 직접 읽을 일이 없는 경우 W를 사용합니다.
		/// </summary>
		enum class ATTACHMENT { NONE, RW, W };
		/// <summary>
		/// 프레임버퍼를 생성합니다. 이름이 겹치는 경우 아무것도 하지 않습니다.
		/// 프레임버퍼를 생성한 직후에는 자동으로 기본 프레임버퍼가 바인드되므로 혼란을 피하려면 필요한 프레임버퍼는 시작할 때 바로 만드시기 바랍니다.
		/// </summary>
		/// <param name="name">접근할 프레임버퍼 이름입니다.</param>
		/// <param name="width">프레임버퍼 폭입니다.</param>
		/// <param name="height">프레임버퍼 높이입니다.</param>
		/// <param name="colorBuffer">색 버퍼 사용 여부입니다.</param>
		/// <param name="depthBuffer">깊이 버퍼 사용 여부입니다.</param>
		/// <param name="stencilBuffer">스텐실 버퍼 사용 여부입니다.</param>
		static void make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer = ATTACHMENT::NONE, unsigned func = 0, unsigned ref = 0, unsigned sfail = 0, unsigned dpfail = 0, unsigned dppass = 0);
		/// <summary>
		/// 만든 프레임버퍼를 제거합니다.
		/// </summary>
		static void drop(const char* name);
		/// <summary>
		/// 그릴 프레임버퍼를 결정(바인드)하고 그것이 사용하는 버퍼를 클리어합니다. NULL 입력이 들어오는 경우 GLFW에서 생성한 기본 프레임버퍼, 즉 화면으로 전환됩니다.
		/// use() 호출 후에는 render 명령이 해당 프레임버퍼에 그리게 됩니다.
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

