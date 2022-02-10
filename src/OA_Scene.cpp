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
			if (e) e->update();	// �� �̵� �� �Ҹ��ڸ� �ٷ� ȣ���ϴµ� �̶� update()�� ����
		}
	}

	void Scene::render() {
		for (auto e : entities) {
			// +program3�� ����(�ִ� ?��)
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