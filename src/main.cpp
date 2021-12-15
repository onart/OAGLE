#include <cstdio>
#include <cstdlib>
#include "oagle.h"

GLFWwindow* window = nullptr;

int frame = 1;
float tp = 0, dt;

/// <summary>
/// 키가 눌렸거나 떼인 최근의 프레임 넘버를 저장합니다.
/// <para>0인 경우 아무런 의미도 없으며, 양수 N인 경우 N프레임째에 키가 눌렸다는 의미이며, 음수 -N인 경우 N프레임째에 키가 떼였다는 의미입니다.</para>
/// <para>프레임 넘버가 아닌 키가 눌린 시간을 이용하려면 응용 측에서 값을 저장하는 편이 좋습니다.</para>
/// </summary>
int pressedKey[GLFW_KEY_LAST + 1] = { 0, };
int pressedMouseKey[GLFW_MOUSE_BUTTON_LAST + 1] = { 0, };

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

void motion(GLFWwindow* window, double x, double y) {

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
	glfwSetMouseButtonCallback(window, mouse);

	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		glfwPollEvents();
		update();
		render();
	}
}