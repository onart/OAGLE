/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "oagle.h"
#include "resources.h"
#include "OA_Shader.h"
#include "OA_Vertex.h"
#include "OA_TestScene.h"
#include "OA_Camera.h"
#include "OA_Audio.h"
#include "OA_Ballpool.h"
#include "externals/stb_image.h"
#include "OA_Game.h"
#include "OA_Entity.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>

namespace onart {
	onart::Camera Game::mainCamera;
	onart::ivec2 Game::windowSize;
	onart::ivec2 Input::mousePos;
	onart::ivec4 Game::vp_ldwh;
	onart::Shader Game::program3;
	onart::Shader Game::program2;
	onart::Shader Game::shadowMap;
	GLFWwindow* Game::window;
	int Game::frame;
	float Game::_tp;
	float Game::_dt;
	float Game::_idt;
	int Input::pressedKey[GLFW_KEY_LAST + 1];
	int Input::pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 3];
	int Input::keyCount = 0;


#ifdef _DEBUG
	/// <summary>
	/// 디버그 모드에 한하여 GL 에러 발생 시 내용을 출력합니다.
	/// </summary>
	static void GLAPIENTRY
		glErrCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL 콜백: %s 유형 = 0x%x, 심각도 = 0x%x, 메시지 = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}
#endif

	void Input::scroll(GLFWwindow* window, double xoffset, double yoffset) {
		if (yoffset > 0) {
			Input::pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 1] = frame;
		}
		else if (yoffset < 0) {
			Input::pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 2] = frame;
		}
	}

	void Game::reshape(GLFWwindow* window, int width, int height) {
		USE_NVEC_LDWH_UPPER;
		float r = mainCamera.ratio.getRatio();
		float dpir = onart::dpiRatio();
		Game::windowSize[0] = width = int(width * dpir);
		Game::windowSize[1] = height = int(height * dpir);
		if (width < height * r) {
			Game::vp_ldwh[LEFT] = 0;
			Game::vp_ldwh[DOWN] = int(height - width / r) / 2;
			Game::vp_ldwh[WIDTH] = width;
			Game::vp_ldwh[HEIGHT] = int(width / r);
			glViewport(0, int(height - width / r) / 2, width, int(width / r));
		}
		else {
			Game::vp_ldwh[LEFT] = int(width - height * r) / 2;
			Game::vp_ldwh[DOWN] = 0;
			Game::vp_ldwh[WIDTH] = int(height * r);
			Game::vp_ldwh[HEIGHT] = height;
			glViewport(int(width - height * r) / 2, 0, int(height * r), height);
		}
	}

	bool Game::init() {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		_idt = (float)mode->refreshRate;
		_dt = 1 / _idt;

		stbi_set_flip_vertically_on_load(true);

		onart::Audio::init();
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnable.xhtml
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);
		program2.initWithMemory(p2vert, p2frag);
		program3.initWithMemory(p3vert, p3frag);
		
		shadowMap.initWithMemory(smvert, smfrag);

		onart::Mesh::rectModel();
		onart::Mesh::circleModel();
		onart::Mesh::sphereModel();
		onart::Mesh::cuboidModel();
		onart::Mesh::iCuboidModel();

		onart::Material::genWhite();
		mainCamera.ratio.setProjMatrix2D();
		onart::Scene::currentScene = new onart::TestScene();
		onart::Scene::currentScene->init();
#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glErrCallback, 0);
#endif
		reshape(window, 1280, 720);
		return true;
	}

	int Game::start() {
		if (!(window = onart::createWindow(u8"오글오글", 1280, 720))) {
			glfwTerminate();
		}
		if (!onart::initExtensions(window)) {
			glfwTerminate();
		}
		if (!init()) {
			glfwTerminate();
		}

		glfwSetWindowSizeCallback(window, reshape);
		glfwSetKeyCallback(window, Input::keyboard);
		glfwSetMouseButtonCallback(window, Input::mouse);
		glfwSetCursorPosCallback(window, Input::mouseMoved);
		glfwSetScrollCallback(window, Input::scroll);
		
		for (frame = 1; !glfwWindowShouldClose(window); frame++) {
			glfwPollEvents();	// 물리 때문에라도 여기 창끌기 관련 조치 필요함
			if constexpr (!OA_AUDIO_NOTHREAD) onart::Audio::allow(false);
			else if constexpr (OA_AUDIO_WAIT_ON_DRAG) onart::Audio::wait = false;
			update();
			render();
			if constexpr (OA_AUDIO_NOTHREAD) {
				onart::Audio::update();
				if constexpr (OA_AUDIO_WAIT_ON_DRAG) onart::Audio::wait = true;
			}
			else { onart::Audio::allow(true); }
		}

		finalize();
		destroyWindow(window);
		return 0;
	}

	void Game::finalize() {
		Audio::terminate();
	}

	void Game::exit() {
		glfwSetWindowShouldClose(window, 1);
	}

	void Game::defaultFramebufferViewport() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		reshape(window, windowSize[0], windowSize[1]);
	}

	void Game::update() {
		if constexpr (onart::OA_FIX_TIMESCALE) {
			_tp += _dt;
		}
		else {
			float cur = (float)glfwGetTime();
			_dt = cur - _tp;
			_idt = 1.0f / _dt;
			_tp = cur;
		}
		onart::Scene::currentScene->update();
		onart::Ballpool2D::Update();
	}

	void Game::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainCamera.viewUpdate();
		onart::Scene::currentScene->render();
		onart::Ballpool2D::render();	// 디버그용
		glfwSwapBuffers(window);
	}

	void Input::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			pressedKey[key] = frame;
			keyCount++;
			break;
		case GLFW_RELEASE:
			pressedKey[key] = -frame;
			keyCount--;
			break;
		}
	}

	void Input::mouse(GLFWwindow* window, int key, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			pressedMouseKey[key] = frame;
			break;
		case GLFW_RELEASE:
			pressedMouseKey[key] = -frame;
			break;
		}
	}

	void Input::mouseMoved(GLFWwindow* window, double x, double y) {
		mousePos[0] = (int)x;
		mousePos[1] = (int)y;
	}

	std::vector<Input::KeyCode> Input::allKeyInputsForNow() {
		std::vector<KeyCode> v;	v.reserve(6);
		for (int i = 32; i <= 96; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 256; i <= 301; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		for (int i = 320; i <= 347; i++) {
			if (isKeyPressedNow((KeyCode)i)) v.push_back((KeyCode)i);
		}
		return v;
	}

	vec2 Input::relativeCursorPos() {
		USE_NVEC_INDEX;
		vec2 rpos(mousePos);
		rpos[x] -= vp_ldwh[left];	rpos[y] -= vp_ldwh[down];					// 뷰포트 좌측상단 좌표를 (0,0)으로 하도록 평행이동 (일반적인 화면 크기 범위에서는 절대 오차가 생기지 않음)
		rpos[x] /= vp_ldwh[width];	rpos[y] /= vp_ldwh[height];					// 뷰포트 우측하단 좌표를 (1,1)로 하도록 수축
		return rpos;
	}

	vec2 Input::cameraCursorPos() {
		static int fr = 0; static vec2 ret;
		if (fr != frame) {
			fr = frame;
			return ret = mainCamera.mouse2screen(relativeCursorPos());
		}
		return ret;
	}

	class CustomCursor :public Entity {
	public:
		CustomCursor(const EntityKey& k, const Transform& tr, pAnimation& anim, GLFWwindow* window) :Entity(k, tr, anim, true, false, true), window(window) {}
		void Update() {
			transform.setLocalPosition(vec3(std::move(Input::cameraCursorPos()), -1));
		}
		~CustomCursor() {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	private:
		GLFWwindow* window;
	};

	void Window::setCursorImage(const pAnimation& anim, const vec2& scale) {
		pAnimation pa = std::dynamic_pointer_cast<FixedSprite>(anim);
		if (!pa)pa = std::dynamic_pointer_cast<UIAnimation>(anim);
		Entity::destroy(Entity::get<CustomCursor>("__cursor"));
		if (!pa) {
			return;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			Entity* cursor = new CustomCursor("__cursor", Transform(vec3(0, 0, -1), scale), pa, window);
			cursor->preserveOnSceneChange = true;
			Scene::currentScene->addEntity(cursor);
		}
	}

	void Window::setWindowTitle(const char* name)
	{
		// UTF8 인코딩 필요
		glfwSetWindowTitle(window, name);
	}
}