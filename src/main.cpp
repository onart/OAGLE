#include <cstdio>
#include <cstdlib>

#include "resources.h"
#include "oagle.h"
#include "oaglem.h"
#include "OA_Shader.h"
#include "OA_Vertex.h"
#include "OA_TestScene.h"
#include "OA_Input.h"
#include "OA_Camera.h"
#include "OA_Audio.h"

// 창 관련
GLFWwindow* window = nullptr;
onart::ivec2 windowSize;
onart::ivec4 vp_ldwh;	// 좌측 하단이 (0,0)
onart::Camera mainCamera;

void setRatio(unsigned rw, unsigned rh) {
	mainCamera.ratio.setRatio(rw, rh);
}

// 프레임/시간
int frame = 1;
float tp = 0, dt;

// 셰이더 프로그램
onart::Shader program3;		// 뷰-프로젝션 행렬 및 셰이딩의 영향을 받는 셰이더 프로그램입니다.
onart::Shader program2;		// 뷰-프로젝션 행렬 및 셰이딩의 영향을 받지 않는 셰이더 프로그램입니다. 객체는 표준 뷰 볼륨에 그려집니다.

// 입력

/// <summary>
/// 키가 눌렸거나 떼인 최근의 프레임 넘버를 저장합니다.
/// <para>0인 경우 아무런 의미도 없으며, 양수 N인 경우 N프레임째에 키가 눌렸다는 의미이며, 음수 -N인 경우 N프레임째에 키가 떼였다는 의미입니다.</para>
/// <para>프레임 넘버가 아닌 키가 눌린 시간을 이용하려면 응용 측에서 값을 저장하는 편이 좋습니다.</para>
/// </summary>
int pressedKey[GLFW_KEY_LAST + 1] = { 0, };
int pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 3] = { 0, };
int keyCount = 0;

onart::ivec2 mousePos;	// 마우스의 위치를 저장합니다.


// 주기 호출 함수 (게임 루프)
void update() {
	float cur = (float)glfwGetTime();
	dt = cur - tp;
	tp = cur;
	onart::Scene::currentScene->update();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	onart::Scene::currentScene->render();
	onart::Audio::update();
	glfwSwapBuffers(window);
}

// 이벤트 콜백 함수

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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

void mouse(GLFWwindow* window, int key, int action, int mods) {
	switch (action) {
		case GLFW_PRESS:
			pressedMouseKey[key] = frame;
			break;
		case GLFW_RELEASE:
			pressedKey[key] = -frame;
			break;
	}
}

void mouseMoved(GLFWwindow* window, double x, double y) {
	mousePos.x = (int)x;
	mousePos.y = (int)y;
}

void reshape(GLFWwindow* window, int width, int height) {
	float r = mainCamera.ratio.getRatio();
	float dpir = onart::dpiRatio();
	windowSize.x = width = int(width * dpir);
	windowSize.y = height = int(height * dpir);
	if (width < height * r) {
		vp_ldwh[0] = 0;
		vp_ldwh[1] = int(height - width / r) / 2;
		vp_ldwh[2] = width;
		vp_ldwh[3] = int(width / r);
		glViewport(0, int(height - width / r) / 2, width, int(width / r));
	}
	else {
		vp_ldwh[0] = int(width - height * r) / 2;
		vp_ldwh[1] = 0;
		vp_ldwh[2] = int(height * r);
		vp_ldwh[3] = height;
		glViewport(int(width - height * r) / 2, 0, int(height * r), height);
	}
}

/// <summary>
/// 마우스 스크롤 콜백. 일반 마우스 기준 yoffset만 +1(휠 위로) 혹은 -1의 값을 가집니다.
/// </summary>
void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 1] = frame;
	}
	else if (yoffset < 0) {
		pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 2] = frame;
	}
}

/// <summary>
/// 게임을 종료합니다.
/// </summary>
void exitGame() {
	glfwSetWindowShouldClose(window, 1);
}


#ifdef _DEBUG
/// <summary>
/// 디버그 모드에 한하여 GL 에러 발생 시 내용을 출력합니다.
/// </summary>
void GLAPIENTRY
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

// 초기화 함수
bool init() {

	//program2.initWithMemory();
	program3.initWithMemory(p3vert, p3frag);
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);

	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnable.xhtml
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	onart::Mesh::rectModel();
	onart::Mesh::circleModel();
	onart::Mesh::sphereModel();
	onart::Mesh::cuboidModel();
	onart::Mesh::iCuboidModel();

	onart::Scene::currentScene = new onart::TestScene();
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glErrCallback, 0);
#endif
	reshape(window, 1280, 720);

	onart::Audio::init();

	return true;
}

void finalize() {
	onart::Audio::terminate();
}

int main(int argc, char* argv[]) {
	if (!(window = onart::createWindow("OAGLE", 1280, 720))) { 
		glfwTerminate(); 
		return 1;
	}
	if (!onart::initExtensions(window)) {
		glfwTerminate();
		return 2;
	}
	if (!init()) {
		return 3;
	}
	
	glfwSetWindowSizeCallback(window, reshape);
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, mouseMoved);
	glfwSetScrollCallback(window, scroll);
	//glfwSetDropCallback(window, callback);	 // 파일을 드래그하여 창에 드롭할 경우 경로가 전달됩니다. void callback(GLFWwindow* window, int count, const char** paths)
	//glfwSetWindowCloseCallback(window, callback)	 // 윈도우 종료 신호 발생 시 콜백을 변경합니다. void callback(GLFWwindow* window)

	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		glfwPollEvents();
		update();
		render();
	}

	finalize();
	onart::destroyWindow(window);
	return 0;
}
