/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Scene.h"
#include "OA_Entity.h"

extern float tp, dt;

namespace onart {
	
	Scene* Scene::currentScene;
	std::set<EntityKey> Scene::nonstop;
	const float& Scene::dt = ::dt;
	const float& Scene::tp = ::tp;

	void Scene::update() {
		if (isPaused) {
			for (auto& k : nonstop) {
				for (auto& e : Entity::gets(k)) {
					e->update();
				}
			}
			return;
		}
		Update();
		for (auto e : entities) {
			if (id != currentScene->id) return;
			if (e) e->update();	// 씬 이동 시 소멸자를 바로 호출하는데 이때 update()가 남음
		}
	}

	void Scene::render() {
		for (auto e : entities) {
			// +program3에 광원(최대 ?개)
			for (auto e : entities) {
				if (e) e->render();
			}
		}
	}

	void Scene::change(Scene* other) {
		for (auto& e : entities) {
			delete e;
			e = nullptr;
		}
		currentScene = other;
		other->init();
		delete this;
	}

	void Scene::actEvent(int idx) {

	}

	vec3 Scene::constrainCamera(const vec3& currentCameraPos, const vec3& desiredCameraPos) {
		return desiredCameraPos;
	}
}