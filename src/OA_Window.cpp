/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifdef _WIN32
	#ifdef _WIN64
		#include "externals/gl/glfw/win64/glfw3.h"
		#pragma comment(lib, "externals/gl/glfw/win64/glfw3_mt.lib")
	#else
		#include "externals/gl/glfw/win32/glfw3.h"
		#pragma comment(lib, "externals/gl/glfw/win32/glfw3_mt.lib")
	#endif // _WIN64
#endif // _WIN32

#include "OA_Window.h"
#include "OA_Anim.h"
#include "OA_Entity.h"
#include "OA_Scene.h"

extern GLFWwindow* window;

namespace onart::window {
	class CustomCursor :public Entity {
	public:
		CustomCursor(const EntityKey& k, const Transform& tr, pAnimation& anim) :Entity(k, tr, anim, true, false, true) {}
		void Update() {
			transform.setLocalPosition(vec3(std::move(Input::cameraCursorPos()), -1));
		}
		~CustomCursor() {
			glfwSetInputMode(::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	};

	void setCursorImage(const pAnimation& anim, const vec2& scale) {
		pAnimation pa = std::dynamic_pointer_cast<FixedSprite>(anim);
		if (!pa)pa = std::dynamic_pointer_cast<UIAnimation>(anim);
		Entity::destroy(Entity::get<CustomCursor>("__cursor"));
		if (!pa) {
			return;
		}
		else {
			glfwSetInputMode(::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			Entity* cursor = new CustomCursor("__cursor", Transform(0, scale), pa);
			cursor->preserveOnSceneChange = true;
			Scene::currentScene->addEntity(cursor);
		}
	}
}
