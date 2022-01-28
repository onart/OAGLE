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

// â ����
GLFWwindow* window = nullptr;
onart::ivec2 windowSize;
onart::ivec4 vp_ldwh;	// ���� �ϴ��� (0,0)
onart::Camera mainCamera;

void setRatio(unsigned rw, unsigned rh) {
	mainCamera.ratio.setRatio(rw, rh);
}

// ������/�ð�
int frame = 1;
float tp = 0, dt;

// ���̴� ���α׷�
onart::Shader program3;		// ��-�������� ��� �� ���̵��� ������ �޴� ���̴� ���α׷��Դϴ�.
onart::Shader program2;		// ��-�������� ��� �� ���̵��� ������ ���� �ʴ� ���̴� ���α׷��Դϴ�. ��ü�� ǥ�� �� ������ �׷����ϴ�.

// �Է�

/// <summary>
/// Ű�� ���Ȱų� ���� �ֱ��� ������ �ѹ��� �����մϴ�.
/// <para>0�� ��� �ƹ��� �ǹ̵� ������, ��� N�� ��� N������°�� Ű�� ���ȴٴ� �ǹ��̸�, ���� -N�� ��� N������°�� Ű�� �����ٴ� �ǹ��Դϴ�.</para>
/// <para>������ �ѹ��� �ƴ� Ű�� ���� �ð��� �̿��Ϸ��� ���� ������ ���� �����ϴ� ���� �����ϴ�.</para>
/// </summary>
int pressedKey[GLFW_KEY_LAST + 1] = { 0, };
int pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 3] = { 0, };
int keyCount = 0;

onart::ivec2 mousePos;	// ���콺�� ��ġ�� �����մϴ�.


// �ֱ� ȣ�� �Լ� (���� ����)
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

// �̺�Ʈ �ݹ� �Լ�

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
/// ���콺 ��ũ�� �ݹ�. �Ϲ� ���콺 ���� yoffset�� +1(�� ����) Ȥ�� -1�� ���� �����ϴ�.
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
/// ������ �����մϴ�.
/// </summary>
void exitGame() {
	glfwSetWindowShouldClose(window, 1);
}


#ifdef _DEBUG
/// <summary>
/// ����� ��忡 ���Ͽ� GL ���� �߻� �� ������ ����մϴ�.
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
	fprintf(stderr, "GL �ݹ�: %s ���� = 0x%x, �ɰ��� = 0x%x, �޽��� = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}
#endif

// �ʱ�ȭ �Լ�
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
	//glfwSetDropCallback(window, callback);	 // ������ �巡���Ͽ� â�� ����� ��� ��ΰ� ���޵˴ϴ�. void callback(GLFWwindow* window, int count, const char** paths)
	//glfwSetWindowCloseCallback(window, callback)	 // ������ ���� ��ȣ �߻� �� �ݹ��� �����մϴ�. void callback(GLFWwindow* window)

	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		glfwPollEvents();
		update();
		render();
	}

	finalize();
	onart::destroyWindow(window);
	return 0;
}
