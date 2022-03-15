/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Scene.h"
#include "OA_Entity.h"

#include <queue>

extern float tp, dt;

namespace onart {
	
	Scene* Scene::currentScene;
	const float& Scene::dt = ::dt;
	const float& Scene::tp = ::tp;
	
	void Scene::update() {
		Update();
		size_t sz = entities.size();
		for (size_t i = 0; i < sz; i++) {
			if (id != currentScene->id) return;
			Entity* e = entities[i];
			if (e)e->update();
		}
	}

	void Scene::render() {
#ifdef OAGLE_2DGAME
		for (auto it = renderOrder.cbegin(); it != renderOrder.cend();) {
			Entity* ep = *(it->second);
			if (ep) {
				ep->render();
				++it;
			}
			else {
				it = renderOrder.erase(it);
			}
		}
#else
		std::priority_queue<std::pair<float, size_t>> hp;
		size_t sz = entities.size();
		for (size_t i = 0; i < sz; i++) {
			if (id != currentScene->id) return;
			Entity* e = entities[i];
			if (e) {
				if (e->isTranslucent) {
					hp.push({ e->zIndex(),i });
				}
				else {
					e->render();
				}
			}
		}
		while (!hp.empty()) {
			Entity* e = entities[hp.top().second];
			if (e)e->render();
			hp.pop();
		}
		reap();
#endif // OAGLE_2DGAME
	}

	void Scene::reap() {
		if (!shouldReap) return;
#ifdef OAGLE_2DGAME
		std::set<size_t> ridx;
		for (auto it : renderOrder.cbegin(); it != renderOrder.cend();) {
			if (!entities[it->second]) {
				ridx.insert(it->second);
				it = renderOrder.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto& i : renderOrder) {
			for (auto& j : ridx) {
				if (i.second > j) --i.second;
				else break;
			}
		}
#endif // OAGLE_2DGAME
		entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());
	}

	void Scene::change(Scene* other) {
		if (other == this) return;
		currentScene = other;
		for (auto& e : entities) {
			if (e->preserveOnSceneChange) {
				other->addEntity(e);
			}
			else {
				delete e;
				e = nullptr;
			}
		}
		other->init();
		delete this;
	}

	void Scene::addEntity(Entity* e) {
		if (e) {
			e->__scene.__this = e;
			e->__scene.relatedScene = this;
			e->__scene.index = (int)entities.size();
			entities.push_back(e);
#ifdef OAGLE_2DGAME
			renderOrder.insert({ e->zIndex(),entities.size() - 1 });
#endif // OAGLE_2DGAME
		}
	}

	void Scene::__dropentity::dropEntity(Entity* e) {
		if (e && e->__scene.relatedScene == currentScene) {
			for (int i = e->__scene.index; i >= 0; i--) {
				if (currentScene->entities[i] == e) {
					currentScene->entities[i] = nullptr;
					currentScene->shouldReap = true;
					break;
				}
			}
		}
	}

	void Scene::actEvent(int idx) {
		
	}

	vec3 Scene::constrainCamera(const vec3& currentCameraPos, const vec3& desiredCameraPos) {
		return desiredCameraPos;
	}
}