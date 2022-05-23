/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Scene.h"
#include "OA_Shader.h"
#include "OA_Entity.h"
#include "OA_FrameBuffer.h"

#include <queue>

namespace onart {
	
	Scene* Scene::currentScene;
	
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
		* 이론
		* 2D 게임에서 대부분의 객체는 translucent
		* 이때 전후 이동이 없는 게임은 현재의 multimap으로 렌더링 순서 해결 가능
		* 전후 이동이 있는 게임은 3D 게임과 마찬가지로 현재 heap sort되는 중 (개체가 중간에 제거될 가능성으로 인해 라이브러리 정렬 사용이 까다로움)
		* (3D는 translucent가 별로 없어 heap sort가 성능에 영향이 거의 없음)
		* 게임은 모니터 업데이트 속도에 따라 초당 수십 프레임~백 수십 프레임 가량을 표시
		* 이에 따라 일반적인 경우 매 프레임 전후 관계가 크게 변하지 않으며 아예 고정된 관계도 다수
		* 그렇다면: 버블정렬/정렬 여부 검사/렌더링을 동시에 하고 정렬 안된 부분이 있을 때만 삽입정렬로 해결한다면(여전히 대부분 정렬된 상태로 침) 대부분의 케이스에서 Omega(n)의 정렬이 가능?
		* (-> 이미 렌더링 시 Omega(n) 순회가 있으므로 거의 노 오버헤드)
		* 그렇다면2: 선 삽입정렬 -> 후 렌더링? 기본 오버헤드가 붙지만 어지간해서는 힙보다 빠르면서 버블보다 균일한 성능이 예상(버블은 정렬 필요해질 때 꽤 무거워짐)
		* 이슈
		* 1. 업데이트 중 개체가 추가된 경우 -> 버블벡터에는 삽입 시 정렬 (씬에 private 상태변수 추가)
		* 2. 희박하지만 렌더링 중 개체가 추가될 가능성이 있음 -> 현재의 리핑과 같은 이치로 버블 렌더링 후 삽입정렬 (다음 프레임부터 표시)
		* 3. 삽입정렬로 전환될 때의 프레임에는 렌더링 순서가 뒤틀림 -> xy의 겹침 가능성까지 포함하면 가능성이 훨씬 낮고 매우 잠깐만 보인다는 점의 트레이드오프.
		*	마음대로 z좌표를 움직이는 개체가 엄청 많은 경우라면 가려짐이 눈에 엄청 띌 것이므로 버블을 포기하는 편이 나음
		*/
		if constexpr (false) {	// 버블 코드: 이론상 대부분 Omega(n)
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
					float past = INFINITY;
					for (i += 1; i < sz; i++) {
						if (id != currentScene->id) return;
						Entity* e2 = entities[bubble[i]];
						if (!e2) {
							bubble[i - 1] = bubble[i];
							continue;
						}
						float e2z = e2->zIndex();
						if (e2z > e1z) {	// 당기면서 렌더링
							if (past < e2z) {
								shouldOrder = true;
							}
							else {
								past = e2z;
							}
							e2->render();
							bubble[i - 1] = bubble[i];
						}
						else {				// 자리에 두면서 렌더링
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
		else if constexpr (true) {	// 삽입정렬 후 렌더링 코드: 이론상 대부분 Omega(n)
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
		else {	// 힙 코드: 특별히 모자라지 않은 성능의 O(nlogn)
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

	void Scene::renderWithShadow() {
		USE_SHADER_UNIFORM;
		FrameBuffer::use("shadow");
		if (lightSource[3] == 1.0f) {
			// 보류: perspective와 lookat에서 실제 카메라에 들어가는 영역을 커버하도록 해야 함
			Game::shadowMap["pv"] = mat4::perspective(PI / 2, 1, 0.0f, 1000.0f) * mat4::lookAt(this->lightSource, 0.0f, vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			// 보류: perspective와 lookat에서 실제 카메라에 들어가는 영역을 커버하도록 해야 함
			Game::shadowMap["pv"] = mat4::lookAt(this->lightSource, 0.0f, vec3(0.0f, 1.0f, 0.0f));
		}
		size_t sz = entities.size();
		for (size_t i = 0; i < sz; i++) {
			if (id != currentScene->id) return;
			Entity* e = entities[i];
			if(e) e->render();
		}
		unsigned zmap = FrameBuffer::get("shadow")->getDepthTex();
		FrameBuffer::use();
		Animation::actSwitch = false;
		Game::program3.texture(zmap, Shader::TexCode::SHADOW);
		render();
		Animation::actSwitch = true;
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
		if (!ridx.empty()) {	// translucent하지 않은 개체만 제거된 경우
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
		if (other == currentScene) return;
		for (Entity*& e : currentScene->entities) {
			if (e->preserveOnSceneChange) {
				other->addEntity(e);
			}
			else {
				Entity::destroy(e);
				e = nullptr;
			}
		}
		other->init();
		delete currentScene;
		currentScene = other;
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

	void Scene::setDirectionalLight(const vec3& dir) {
		USE_SHADER_UNIFORM;
		this->lightSource = vec4(dir, 0);
		Game::program3[lightSource] = this->lightSource;
	}

	void Scene::setPointLight(const vec3& pos) {
		USE_SHADER_UNIFORM;
		this->lightSource = vec4(pos, 1);
		Game::program3[lightSource] = this->lightSource;
	}

	void Scene::setAmbientLight(const vec4& amb) {
		USE_SHADER_UNIFORM;
		Game::program3[Ia] = amb;
	}

	void Scene::setDiffuseLight(const vec4& diff) {
		USE_SHADER_UNIFORM;
		Game::program3[Id] = diff;
	}

	void Scene::setSpecularLight(const vec4& spec) {
		USE_SHADER_UNIFORM;
		Game::program3[Is] = spec;
	}
}