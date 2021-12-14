#include <cstdio>
#include <cstdlib>
#include "oagle.h"

GLFWwindow* window = nullptr;

int frame = 1;
float tp = 0, dt;

/// <summary>
/// Ű�� ���Ȱų� ���� �ֱ��� ������ �ѹ��� �����մϴ�.
/// <para>0�� ��� �ƹ��� �ǹ̵� ������, ��� N�� ��� N������°�� Ű�� ���ȴٴ� �ǹ��̸�, ���� -N�� ��� N������°�� Ű�� �����ٴ� �ǹ��Դϴ�.</para>
/// <para>������ �ѹ��� �ƴ� Ű�� ���� �ð��� �̿��Ϸ��� ���� ������ ���� �����ϴ� ���� �����ϴ�.</para>
/// </summary>
int pressedKey[GLFW_KEY_LAST + 1] = { 0, };
/// <summary>
/// ���콺 Ű�� ���Ȱų� ���� �ֱ��� ������ �ѹ��� �����մϴ�.
/// <para>0�� ��� �ƹ��� �ǹ̵� ������, ��� N�� ��� N������°�� Ű�� ���ȴٴ� �ǹ��̸�, ���� -N�� ��� N������°�� Ű�� �����ٴ� �ǹ��Դϴ�.</para>
/// <para>������ �ѹ��� �ƴ� Ű�� ���� �ð��� �̿��Ϸ��� ���� ������ ���� �����ϴ� ���� �����ϴ�.</para>
/// </summary>
int pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 1] = { 0, };	// 0: ����, ���: Ű�� ���� ������ ��ȣ, ����: Ű�� ���� ������ ��ȣ

void update() {
	float cur = (float)glfwGetTime();
	dt = cur - tp;
	tp = cur;
}

void render() {

}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action) {
		case GLFW_PRESS:
			pressedKey[key] = frame;
			break;
		case GLFW_RELEASE:
			pressedKey[key] = -frame;
			break;
	}
}

void reshape(GLFWwindow* window, int width, int height) {
	float r = onart::vp.getRatio();
	float dpir = onart::dpiRatio();
	width = int(width * dpir);
	height = int(height * dpir);
	if (width < height * onart::vp.getRatio()) {
		glViewport(0, int(height - width / r) / 2, width, int(width / r));
	}
	else {
		glViewport(int(width - height * r) / 2, 0, int(height * r), height);
	}
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
	glfwSetWindowSizeCallback(window, reshape);
	glfwSetKeyCallback(window, keyboard);

	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		glfwPollEvents();
		update();
		render();
	}
}