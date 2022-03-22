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
		doing = SceneDoing::UPDATE;
		Update();
		size_t sz = entities.size();
		for (size_t i = 0; i < sz; i++) {
			if (id != currentScene->id) return;
			Entity* e = entities[i];
			if (e)e->update();
		}
		doing = SceneDoing::NOTHING;
	}

	void Scene::render() {
		doing = SceneDoing::RENDER;
#ifdef OAGLE_2DGAME
		for (auto it = renderOrder.cbegin(); it != renderOrder.cend();) {
			Entity* ep = entities[it->second];
			if (ep) {
				ep->render();
				++it;
			}
			else {
				it = renderOrder.erase(it);
			}
		}
#else
		/*
		* �̷�
		* 2D ���ӿ��� ��κ��� ��ü�� translucent
		* �̶� ���� �̵��� ���� ������ ������ multimap���� ������ ���� �ذ� ����
		* ���� �̵��� �ִ� ������ 3D ���Ӱ� ���������� ���� heap sort�Ǵ� �� (��ü�� �߰��� ���ŵ� ���ɼ����� ���� ���̺귯�� ���� ����� ��ٷο�)
		* (3D�� translucent�� ���� ���� heap sort�� ���ɿ� ������ ���� ����)
		* ������ ����� ������Ʈ �ӵ��� ���� �ʴ� ���� ������~�� ���� ������ ������ ǥ��
		* �̿� ���� �Ϲ����� ��� �� ������ ���� ���谡 ũ�� ������ ������ �ƿ� ������ ���赵 �ټ�
		* �׷��ٸ�: ��������/���� ���� �˻�/�������� ���ÿ� �ϰ� ���� �ȵ� �κ��� ���� ���� �������ķ� �ذ��Ѵٸ�(������ ��κ� ���ĵ� ���·� ħ) ��κ��� ���̽����� Omega(n)�� ������ ����?
		* (-> �̹� ������ �� Omega(n) ��ȸ�� �����Ƿ� ���� �� �������)
		* �׷��ٸ�2: �� �������� -> �� ������? �⺻ ������尡 ������ �������ؼ��� ������ �����鼭 ������ ������ ������ ����(������ ���� �ʿ����� �� �� ���ſ���)
		* �̽�
		* 1. ������Ʈ �� ��ü�� �߰��� ��� -> �����Ϳ��� ���� �� ���� (���� private ���º��� �߰�)
		* 2. ��������� ������ �� ��ü�� �߰��� ���ɼ��� ���� -> ������ ���ΰ� ���� ��ġ�� ���� ������ �� �������� (���� �����Ӻ��� ǥ��)
		* 3. �������ķ� ��ȯ�� ���� �����ӿ��� ������ ������ ��Ʋ�� -> xy�� ��ħ ���ɼ����� �����ϸ� ���ɼ��� �ξ� ���� �ſ� ��� ���δٴ� ���� Ʈ���̵����.
		*	������� z��ǥ�� �����̴� ��ü�� ��û ���� ����� �������� ���� ��û �� ���̹Ƿ� ������ �����ϴ� ���� ����
		*/
		if constexpr (false) {	// ���� �ڵ�: �̷л� ��κ� Omega(n)
			size_t sz = entities.size();
			for (size_t i = 0; i < sz; i++) {
				if (id != currentScene->id) return;
				Entity* e = entities[i];
				if (e && !e->isTranslucent) { e->render(); }
			}
			sz = bubble.size();
			if (sz > 0) {
				size_t e1 = 0;
				size_t i = 0;
				for (; i < sz && entities[bubble[i]] == nullptr; i++);
				if (i < sz) {
					e1 = bubble[i];
					float e1z = entities[e1]->zIndex();
					float past = INF;
					for (i += 1; i < sz; i++) {
						if (id != currentScene->id) return;
						Entity* e2 = entities[bubble[i]];
						if (!e2) {
							bubble[i - 1] = bubble[i];
							continue;
						}
						float e2z = e2->zIndex();
						if (e2z > e1z) {	// ���鼭 ������
							if (past < e2z) {
								shouldOrder = true;
							}
							else {
								past = e2z;
							}
							e2->render();
							bubble[i - 1] = bubble[i];
						}
						else {				// �ڸ��� �θ鼭 ������
							past = e1z;
							bubble[i - 1] = e1;
							if (entities[e1]) {
								entities[e1]->render();
							}
							for (; i < sz && entities[bubble[i]] == nullptr; i++);
							if (i < sz) {
								e1 = bubble[i];
								e1z = entities[e1]->zIndex();
							}
						}
					}
					Entity* last = entities[bubble[sz - 1]];
					if (last) last->render();
				}
			}
		}
		else if constexpr (true) {	// �������� �� ������ �ڵ�: �̷л� ��κ� Omega(n)
			size_t sz = entities.size();
			for (size_t i = 0; i < sz; i++) {
				if (id != currentScene->id) return;
				Entity* e = entities[i];
				if (e && !e->isTranslucent) { e->render(); }
			}
			sz = bubble.size();
			for (size_t i = 1; i < sz; i++) {
				size_t j = i;
				Entity* ei = entities[bubble[j]];
				if (!ei) continue;
				float jz = ei->zIndex();
				while (j > 0) {
					Entity* eleft = entities[bubble[j - 1]];
					if (!eleft || jz > eleft->zIndex()) {
						std::swap(bubble[j], bubble[j - 1]);
						j--;
					}
					else break;
				}
			}
			for (size_t i = 0; i < sz; i++) {
				Entity* re = entities[bubble[i]];
				if (re) re->render();
			}
		}
		else {	// �� �ڵ�: Ư���� ���ڶ��� ���� ������ O(nlogn)
			size_t sz = entities.size();
			std::priority_queue<std::pair<float, size_t>> hp;
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
				if (id != currentScene->id) return;
				Entity* e = entities[hp.top().second];
				if (e)e->render();
				hp.pop();
			}
		}
		reap();
#endif // OAGLE_2DGAME
		doing = SceneDoing::NOTHING;
	}

	void Scene::reap() {
		if (!shouldReap && !shouldOrder) return;
#ifdef OAGLE_2DGAME
		std::set<size_t> ridx;
		for (auto it = renderOrder.cbegin(); it != renderOrder.cend();) {
			if (!entities[it->second]) {
				ridx.insert(it->second);
				it = renderOrder.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto& i : renderOrder) {
			size_t minus = 0;
			for (auto j : ridx) {
				if (i.second > j) ++minus;
				else break;
			}
			i.second -= minus;
		}
#else
		std::set<size_t> ridx;
		constexpr size_t DNE = -1;
		for (size_t& i : bubble) {
			if (!entities[i]) { 
				ridx.insert(i);
				i = DNE;
			}
		}
		if (!ridx.empty()) {	// translucent���� ���� ��ü�� ���ŵ� ���
			bubble.erase(std::remove(bubble.begin(), bubble.end(), DNE), bubble.end());
			for (size_t& i : bubble) {
				size_t minus = 0;
				for (auto j : ridx) {
					if (i > j)++minus;
					else break;
				}
				i -= minus;
			}
		}
		if (shouldOrder) {
			size_t sz = bubble.size();
			for (size_t i = 1; i < sz; i++) {
				size_t j = i;
				float jz = entities[bubble[j]]->zIndex();
				while (j > 0 && jz > entities[bubble[j - 1]]->zIndex()) {
					std::swap(bubble[j], bubble[j - 1]);
					j--;
				}
			}
			shouldOrder = false;
		}
#endif // OAGLE_2DGAME
		entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());
		shouldReap = false;
	}

	void Scene::change(Scene* other) {
		if (other == this) return;
		currentScene = other;
		for (auto& e : entities) {
			if (e->preserveOnSceneChange) {
				other->addEntity(e);
			}
			else {
				Entity::destroy(e);
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
			renderOrder.insert({ -e->zIndex(),entities.size() - 1 });
#else
			if (e->isTranslucent) {
				if (doing != SceneDoing::RENDER) {
					auto it = bubble.crbegin();
					auto ed = bubble.crend();
					float zz = e->zIndex();
					while (it != ed) {
						Entity* comp = entities[*it];
						if (comp == nullptr || comp->zIndex() < zz) { ++it; continue; }
						break;
					}
					bubble.insert(it.base(), entities.size() - 1);
				}
				else {
					bubble.push_back(entities.size() - 1);
					shouldOrder = true;
				}
			}
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