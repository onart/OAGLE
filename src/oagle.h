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
	/// GLFW �ʱ�ȭ �Լ��� �� �� ���� ȣ���մϴ�.
	/// </summary>
	/// <returns>���� ����</returns>
	inline bool initGLFW() {
		static bool b = false;
		if (b) return true;
		return b = glfwInit();
		// https://www.glfw.org/docs/latest/group__init.html#ga317aac130a235ab08c6db0834907d85e
	}

	/// <summary>
	/// ���� �� ȣ��Ǵ� �Լ��Դϴ�.
	/// </summary>
	/// <param name="window">���� â</param>
	inline void destroyWindow(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	/// <summary>
	/// GLFW ���� �߻� �� ������ ����ϴ� �ݹ��Դϴ�.
	/// </summary>
	/// <param name="code"></param>
	/// <param name="desc"></param>
	inline void printError(int code, const char* desc) {
		printf("GLFW ERROR %d: %s\n", code, desc);
	}
	
	/// <summary>
	/// DPI ������ ���� ���� DPI ���� (���������� ���� ���ϱ�)
	/// </summary>
	/// <returns></returns>
	inline float dpiRatio() {
		initGLFW();
		static float r = 0;
		if (r > 0) return r;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) return 1;
		float xr, yr;
		glfwGetMonitorContentScale(monitor, &xr, &yr);	// ���� DPI�� �÷��� �⺻ DPI ����
		r = std::max(xr, yr);
		if (r < 1.01f || r > 16.0f) r = 1;
		return r;
	}

	/// <summary>
	/// â�� ������ �ɼ����� �����մϴ�. �ٸ� �ɼ��� GLFW ������ ���� �����Ͻñ� �ٶ��ϴ�.
	/// </summary>
	/// <param name="name">â�� �̸����� ǥ�õ� ���ڿ��Դϴ�. UTF-8 ���ڵ�</param>
	/// <param name="width">â�� ���� �����Դϴ�.</param>
	/// <param name="height">â�� ���� �����Դϴ�.</param>
	/// <returns>������ window�� ������</returns>
	inline GLFWwindow* createWindow(std::string name, unsigned width, unsigned height) {
		initGLFW();
		glfwSetErrorCallback(printError);
		/*
		* https://www.glfw.org/docs/latest/window_guide.html#window_hints
		* GLFW_RESIZABLE:		����ڿ� ���� â ũ�� ���� ���� ����. false�� ��쿡�� ���α׷� ���� glfwSetWindowSize�� ������. �⺻ TRUE
		* GLFW_VISIBLE:			ó���� â�� ������. �⺻ TRUE
		* GLFW_DECORATED:		���, �ݱ� ��ư �� ���ڷ��̼��� �ִ��� (false�� ��� ����� ũ�� ���� �Ұ���). �⺻ TRUE
		* GLFW_FOCUSED:			â ���� �� ��Ŀ�� ������ ����, �⺻ TRUE
		* GLFW_AUTO_ICONIFY:	��üȭ�� â�� ������ ���� ���� ��� â�� �ڵ����� �ּ�ȭ/��Ŀ�� ������ ����. �⺻ TRUE
		* GLFW_FLOATING:		â��� â�� �׻� ���� �տ� ��ġ. �⺻ FALSE
		* GLFW_MAXIMIZED:		���� �� �ִ�ȭ ����. �⺻ FALSE
		* GLFW_CENTER_CURSOR:	��üȭ�� �� Ŀ���� ����� �̵�. TRUE
		* GLFW_TRANSPARENT_FRAMEBUFFER:	â �����ӹ��� ���� ����. �⺻ FALSE
		* GLFW_FOCUS_ON_SHOW:	glfwShowWindow() ȣ�� �� â�� �Է� ��Ŀ���� ����. �⺻ TRUE
		* GLFW_SCALE_TO_MONIOTR:â�� ����� ���� �Ը� ���� �ڵ����� ������. ȭ�� ��ǥ�� �ȼ��� �ϴ��� �����ϴ� ��쿡�� ����. �⺻ FALSE
		* GLFW_(RED | GREEN | BLUE | ALPHA | STENCIL)_BITS: ������ ��Ʈ ���е�.  �⺻ 8
		* GLFW_DEPTH_BITS:		�⺻ 24
		* GLFW_ACCUM_(RED | GREEB | BLUE | ALPHA)_BITS:	accumulation ������ ��Ʈ ���е�. �⺻ 0
		* GLFW_AUX_BUFFERS:		�ӽ� ������ ��. �⺻ 0
		* GLFW_SAMPLES:			��Ƽ���ø� �ִ� ��. �⺻ 0
		* GLFW_REFRESH_RATE:	����� �ֻ���. �⺻ GLFW_DONT_CARE (����� �ִ�ġ��)
		* GLFW_STEREO:			OPENGL ���׸��������� ������ ��� ����. �⺻ FALSE
		* GLFW_SRGB_CAPABLE:	�����ӹ��۰� sRGB�� Ŀ�� ��������. �⺻ FALSE
		* GLFW_DOUBLEBUFFER:	������۸� �������. �⺻ TRUE
		* GLFW_CLIENT_API:		Ŭ���̾�Ʈ API. �⺻ GLFW_OPENGL_API. GLFW_OPENGL_ES_API, GLFW_NO_API�� �ٸ� �ɼ����� ����
		* GLFW_CONTEXT_CREATION_API:	����� ���ؽ�Ʈ ���� API. �⺻ GLFW_NATIVE_CONTEXT_API. GLFW_EGL_CONTEXT_API, GLFW_OSMESA_CONTEXT_API�� ����
		* GLFW_CONTEXT_VERSION_MAJOR:	Ŭ���̾�Ʈ API ����. �⺻ 1
		* GLFW_CONTEXT_VERSION_MINOR:	�⺻ 0
		* GLFW_CONTEXT_ROBUSTNESS:		���� ����(?). �⺻ GLFW_NO_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION, GLFW_LOSE_CONTEXT_ON_RESET�� �߰��� ����
		* GLFW_CONTEXT_RELEASE_BEHAVIOR:������ �� �ൿ�� ����. �⺻ GLFW_ANY_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_(FLUSH | NONE)�� ����
		* GLFW_OPENGL_FORWARD_COMPAT:	OPENGL ���ؽ�Ʈ�� ���Ŀ� ȣȯ���� ������ ����. �⺻ FALSE
		* GLFW_OPENGL_DEBUG_CONTEXT:	OPENGL�� ����� ���� ���ư�. �⺻ FALSE
		* GLFW_OPENGL_PROFILE:			OPENGL ���ؽ�Ʈ ������. �⺻ GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_(CORE | COMPAT | ANY)_PROFILE ����
		* �Ʒ��� MAC, X11 ��� �� ����Ʈ���� ����
		* GLFW_COCOA_RETINA_FRAMEBUFFER: �⺻ TRUE
		* GLFW_COCOA_FRAME_NAME: �⺻ ""
		* GLFW_COCOA_GRAPHICS_SWITCHING: �⺻ FALSE
		* GLFW_X11_GRAPHICS_SWITCHING: �⺻ FALSE
		* GLFW_X11_CLASS_NAME: �⺻ ""
		* GLFW_X11_INSTANCE_NAME: �⺻ ""
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
		
		// ȭ�� ���
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
	/// OpenGL Ȯ���� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <param name="window"></param>
	/// <returns></returns>
	inline bool initExtensions(GLFWwindow* window) {
		
		// �־��� â�� ���� ��
		glfwMakeContextCurrent(window);
#ifdef GL_ES_VERSION_2_0
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): gladLoadGLES2Loader() ����\n"); glfwTerminate(); return false; }
#else
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): gladLoadGLLoader() ����\n"); glfwTerminate(); return false; }
#endif // GL_ES_VERSION_2_0
		
		glGetIntegerv(GL_MAJOR_VERSION, &VMAJOR);
		glGetIntegerv(GL_MINOR_VERSION, &VMINOR);
		while (VMINOR > 10) VMINOR /= 10;
		
		if (VMAJOR <= 2) {
			printf("���: �� ���α׷��� OpenGL 3 �̸� ������ ȣȯ�� �� ���� ���� �� �ֽ��ϴ�.\n");
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
		if (ext.find("GL_ARB_vertex_buffer_object") == ext.end()) printf("VBO�� �������� �ʽ��ϴ�.\n");
		if (ext.find("GL_ARB_vertex_array_object") == ext.end()) printf("VAO�� �������� �ʽ��ϴ�.\n");
		if (ext.find("GL_ARB_vertex_shader") == ext.end()) printf("���� ���̴��� �������� �ʽ��ϴ�.\n");
		if (ext.find("GL_ARB_fragment_shader") == ext.end()) printf("���� ���̴��� �������� �ʽ��ϴ�.\n");
		if (ext.find("GL_ARB_shader_objects") == ext.end()) printf("���̴� ������Ʈ�� �������� �ʽ��ϴ�.\n");
		
#endif	// GL_ES_VERSION_2_0
		return true;
	}
}


#endif // !__OAGLE_H__
