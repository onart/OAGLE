#ifndef __OAGLE_H__
#define __OAGLE_H__

#include "gl/glad/glad.h"

#ifdef _WIN32
	#ifdef _WIN64
		#include "gl/glfw/win64/glfw3.h"
		#pragma comment(lib, "win64/glfw3_mt.lib")
	#else
		#include "gl/glfw/win32/glfw3.h"
		#pragma comment(lib, "win32/glfw3_mt.lib")
	#endif // _WIN64
#endif // _WIN32

#ifdef GL_ES_VERSION_2_0
	int VMAJOR = 2, VMINOR = 0;
	constexpr bool IS_ES = true;
#else
	int VMAJOR = 4, VMINOR = 6;
	constexpr bool IS_ES = false;
#endif
	int SLVMAJOR = 0, SLVMINOR = 0;

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>

namespace onart {

	constexpr int R_WIDTH = 16;
	constexpr int R_HEIGHT = 9;
	constexpr float LRPOL = R_WIDTH > R_HEIGHT ? (float)R_WIDTH / R_HEIGHT : 1;
	constexpr float UDPOL = R_WIDTH > R_HEIGHT ? 1 : (float)R_HEIGHT / R_WIDTH;

	/// <summary>
	/// GLFW 초기화 함수를 단 한 번만 호출합니다.
	/// </summary>
	/// <returns>성공 여부</returns>
	inline bool initGLFW() {
		static bool b = false;
		if (b) return true;
		return b = glfwInit();
		// https://www.glfw.org/docs/latest/group__init.html#ga317aac130a235ab08c6db0834907d85e
	}

	/// <summary>
	/// 종료 시 호출되는 함수입니다.
	/// </summary>
	/// <param name="window">닫을 창</param>
	inline void destroyWindow(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	/// <summary>
	/// GLFW 오류 발생 시 내용을 출력하는 콜백입니다.
	/// </summary>
	/// <param name="code"></param>
	/// <param name="desc"></param>
	inline void printError(int code, const char* desc) {
		printf("GLFW ERROR %d: %s\n", code, desc);
	}
	
	/// <summary>
	/// DPI 보정을 위해 현재 DPI 리턴 (최종적으로 값을 곱하기)
	/// </summary>
	/// <returns></returns>
	inline float dpiRatio() {
		initGLFW();
		static float r = 0;
		if (r > 0) return r;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) return 1;
		float xr, yr;
		glfwGetMonitorContentScale(monitor, &xr, &yr);	// 현재 DPI와 플랫폼 기본 DPI 비율
		r = std::max(xr, yr);
		if (r < 1.01f || r > 16.0f) r = 1;
		return r;
	}

	/// <summary>
	/// 창을 정해진 옵션으로 생성합니다. 다른 옵션은 GLFW 문서를 보고 수정하시기 바랍니다.
	/// </summary>
	/// <param name="name">창의 이름으로 표시될 문자열입니다. UTF-8 인코딩</param>
	/// <param name="width">창의 가로 길이입니다.</param>
	/// <param name="height">창의 세로 길이입니다.</param>
	/// <returns>생성한 window의 포인터</returns>
	inline GLFWwindow* createWindow(std::string name, unsigned width, unsigned height) {
		initGLFW();
		glfwSetErrorCallback(printError);
		/*
		* https://www.glfw.org/docs/latest/window_guide.html#window_hints
		* GLFW_RESIZABLE:		사용자에 의한 창 크기 조절 가능 여부. false인 경우에도 프로그램 측은 glfwSetWindowSize로 가능함. 기본 TRUE
		* GLFW_VISIBLE:			처음에 창을 보일지. 기본 TRUE
		* GLFW_DECORATED:		경계, 닫기 버튼 등 데코레이션이 있는지 (false인 경우 사용자 크기 조정 불가능). 기본 TRUE
		* GLFW_FOCUSED:			창 생성 시 포커스 가져옴 여부, 기본 TRUE
		* GLFW_AUTO_ICONIFY:	전체화면 창이 열리면 기존 비디오 모드 창이 자동으로 최소화/포커스 나가면 복구. 기본 TRUE
		* GLFW_FLOATING:		창모드 창이 항상 가장 앞에 위치. 기본 FALSE
		* GLFW_MAXIMIZED:		생성 시 최대화 상태. 기본 FALSE
		* GLFW_CENTER_CURSOR:	전체화면 시 커서가 가운데로 이동. TRUE
		* GLFW_TRANSPARENT_FRAMEBUFFER:	창 프레임버퍼 투명성 여부. 기본 FALSE
		* GLFW_FOCUS_ON_SHOW:	glfwShowWindow() 호출 시 창이 입력 포커스를 얻음. 기본 TRUE
		* GLFW_SCALE_TO_MONIOTR:창이 모니터 내용 규모에 따라 자동으로 재조정. 화면 좌표가 픽셀과 일대일 대응하는 경우에만 동작. 기본 FALSE
		* GLFW_(RED | GREEN | BLUE | ALPHA | STENCIL)_BITS: 색상의 비트 정밀도.  기본 8
		* GLFW_DEPTH_BITS:		기본 24
		* GLFW_ACCUM_(RED | GREEB | BLUE | ALPHA)_BITS:	accumulation 버퍼의 비트 정밀도. 기본 0
		* GLFW_AUX_BUFFERS:		임시 버퍼의 수. 기본 0
		* GLFW_SAMPLES:			멀티샘플링 최대 수. 기본 0
		* GLFW_REFRESH_RATE:	모니터 주사율. 기본 GLFW_DONT_CARE (모니터 최대치로)
		* GLFW_STEREO:			OPENGL 스테리오스코픽 렌더링 사용 여부. 기본 FALSE
		* GLFW_SRGB_CAPABLE:	프레임버퍼가 sRGB를 커버 가능한지. 기본 FALSE
		* GLFW_DOUBLEBUFFER:	더블버퍼를 사용할지. 기본 TRUE
		* GLFW_CLIENT_API:		클라이언트 API. 기본 GLFW_OPENGL_API. GLFW_OPENGL_ES_API, GLFW_NO_API가 다른 옵션으로 가능
		* GLFW_CONTEXT_CREATION_API:	사용할 컨텍스트 생성 API. 기본 GLFW_NATIVE_CONTEXT_API. GLFW_EGL_CONTEXT_API, GLFW_OSMESA_CONTEXT_API도 가능
		* GLFW_CONTEXT_VERSION_MAJOR:	클라이언트 API 버전. 기본 1
		* GLFW_CONTEXT_VERSION_MINOR:	기본 0
		* GLFW_CONTEXT_ROBUSTNESS:		강건 전략(?). 기본 GLFW_NO_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION, GLFW_LOSE_CONTEXT_ON_RESET이 추가로 가능
		* GLFW_CONTEXT_RELEASE_BEHAVIOR:릴리즈 시 행동을 지정. 기본 GLFW_ANY_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_(FLUSH | NONE)도 가능
		* GLFW_OPENGL_FORWARD_COMPAT:	OPENGL 컨텍스트가 이후와 호환성을 갖출지 설정. 기본 FALSE
		* GLFW_OPENGL_DEBUG_CONTEXT:	OPENGL이 디버그 모드로 돌아감. 기본 FALSE
		* GLFW_OPENGL_PROFILE:			OPENGL 컨텍스트 프로필. 기본 GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_(CORE | COMPAT | ANY)_PROFILE 가능
		* 아래는 MAC, X11 사용 시 사이트에서 참고
		* GLFW_COCOA_RETINA_FRAMEBUFFER: 기본 TRUE
		* GLFW_COCOA_FRAME_NAME: 기본 ""
		* GLFW_COCOA_GRAPHICS_SWITCHING: 기본 FALSE
		* GLFW_X11_GRAPHICS_SWITCHING: 기본 FALSE
		* GLFW_X11_CLASS_NAME: 기본 ""
		* GLFW_X11_INSTANCE_NAME: 기본 ""
		*/
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VMAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VMINOR);
		if (IS_ES) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);				// core profile (>=3.3) allow only forward-compatible profile
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// create core profile; all legacy deprecated
		}
		GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		
		// 화면 가운데
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int x = (mode->width - width) / 2;
		int y = (mode->height - height) / 2;
		glfwSetWindowPos(window, x, y);
		glfwSetWindowSize(window, width, height);
		glfwMakeContextCurrent(window);
		glfwShowWindow(window);
		return window;
	}

	/// <summary>
	/// OpenGL 확장을 초기화합니다.
	/// </summary>
	/// <param name="window"></param>
	/// <returns></returns>
	inline bool initExtensions(GLFWwindow* window) {
		
		// 주어진 창에 대한 것
		glfwMakeContextCurrent(window);
#ifdef GL_ES_VERSION_2_0
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): gladLoadGLES2Loader() 오류\n"); glfwTerminate(); return false; }
#else
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): gladLoadGLLoader() 오류\n"); glfwTerminate(); return false; }
#endif // GL_ES_VERSION_2_0
		
		glGetIntegerv(GL_MAJOR_VERSION, &VMAJOR);
		glGetIntegerv(GL_MINOR_VERSION, &VMINOR);
		while (VMINOR > 10) VMINOR /= 10;
		
		if (VMAJOR <= 2) {
			printf("경고: 이 프로그램은 OpenGL 3 미만 버전과 호환이 잘 되지 못할 수 있습니다.\n");
		}
		char* slv = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		char* p = slv;
		while (*p) {
			if (isdigit(*p)) {
				long val = strtol(p, &p, 10);
				if (SLVMAJOR == 0) {
					SLVMAJOR = val;
				}
				else if (SLVMINOR == 0) {
					SLVMINOR = val;
					while (SLVMINOR > 10) SLVMINOR /= 10;
					break;
				}
			}
			else {
				p++;
			}
		}

		std::set<std::string> ext;
#ifdef GL_ES_VERSION_2_0
		size_t len = strlen((const char*)glGetString(GL_EXTENSIONS));
		std::vector<char> extv((char*)glGetString(GL_EXTENSIONS), (char*)glGetString(GL_EXTENSIONS) + len + 2);
		for (char* t = strtok(&extv[0], " \t\n"); t; t = strtok(nullptr, " \t\n")) ext.insert(t);
#else
		int next; glGetIntegerv(GL_NUM_EXTENSIONS, &next);
		for (int k = 0; k < next; k++) ext.insert((const char*)glGetStringi(GL_EXTENSIONS, k));
		if (ext.find("GL_ARB_vertex_buffer_object") == ext.end()) printf("VBO가 지원되지 않습니다.\n");
		if (ext.find("GL_ARB_vertex_array_object") == ext.end()) printf("VAO가 지원되지 않습니다.\n");
		if (ext.find("GL_ARB_vertex_shader") == ext.end()) printf("정점 셰이더가 지원되지 않습니다.\n");
		if (ext.find("GL_ARB_fragment_shader") == ext.end()) printf("조각 셰이더가 지원되지 않습니다.\n");
		if (ext.find("GL_ARB_shader_objects") == ext.end()) printf("셰이더 오브젝트가 지원되지 않습니다.\n");
		
#endif	// GL_ES_VERSION_2_0
		return true;
	}
}


#endif // !__OAGLE_H__
