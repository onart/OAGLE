/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Entity.h"
#include "OA_Shader.h"
#include "OA_Model.h"
#include "OA_Anim.h"
#include "OA_Scene.h"
#include "OA_Camera.h"

#include <algorithm>
#include <iterator>

extern onart::Shader program2, program3;
extern float tp, dt;

namespace onart {

	const float& Entity::tp = ::tp;
	const float& Entity::dt = ::dt;

	std::multimap<Entity::EntityKey, Entity*> Entity::entities;

	float Entity::zIndex() {
		return (mainCamera.getViewMatrix() * transform.getModel())._34;
	}

	Entity::Entity(const EntityKey& k, const Transform& transform, bool isFixed, bool rc, bool isTranslucent)
		:key(k), transform(transform), localTp(lt), animState(as), animStartTimepoint(lt), isFixed(isFixed), responseContinuously(rc), isTranslucent(isTranslucent) {
		entities.insert({ k,this });
	}

	Entity::Entity(const EntityKey& k, const Transform& transform, pAnimation& anim0, bool isFixed, bool rc, bool isTranslucent)
		:key(k), transform(transform), localTp(lt), animState(as), animStartTimepoint(lt), isFixed(isFixed), responseContinuously(rc), isTranslucent(isTranslucent) {
		entities.insert({ k,this });
		if (anim0)addAnim(anim0);
	}
	
	Entity::Entity(const EntityKey& k, const Transform& transform, std::shared_ptr<Model>& model, bool isFixed, bool rc, bool isTranslucent)
		:key(k), transform(transform), localTp(lt), animState(as), animStartTimepoint(lt), isFixed(isFixed), responseContinuously(rc), isTranslucent(isTranslucent) {
		entities.insert({ k,this });
		if (model)setModel(model);
	}

	Entity::~Entity() {
		auto ub = entities.upper_bound(key);
		for (auto iter = entities.lower_bound(key); iter != ub; ++iter) {
			if (iter->second == this) {
				entities.erase(iter);
				return;
			}
		}
	}	

	Entity::__dropfromscene::~__dropfromscene() { 
		Scene::__dropentity::dropEntity(__this);
	}

	std::vector<Entity*> Entity::gets(const EntityKey& k) {
		auto ub = entities.upper_bound(k);
		std::vector<Entity*> v;
		v.reserve(entities.count(k));
		std::transform(entities.lower_bound(k), entities.upper_bound(k), std::back_inserter(v), [](std::pair<EntityKey, Entity*> p) {return p.second;});
		return v;
	}

	template <class T>
	std::vector<T*> Entity::gets(const EntityKey& k) {
		auto ub = entities.upper_bound(k);
		std::vector<Entity*> v;
		v.reserve(entities.count(k));
		std::transform(entities.lower_bound(k), entities.upper_bound(k), std::back_inserter(v), [](std::pair<EntityKey, Entity*> p) {return dynamic_cast<T*>(p.second); });
		v.erase(std::remove(v.begin(), v.end(), nullptr), v.end());
		return v;
	}

	Entity* Entity::get(const EntityKey& k) {
		auto e = entities.find(k);
		if (e == entities.end()) return nullptr;
		else return e->second;
	}

	template <class T>
	T* Entity::get2(const EntityKey& k) {
		auto ub = entities.upper_bound(k);
		for (auto iter = entities.lower_bound(k); iter != ub; ++iter) {
			auto x = dynamic_cast<T*>(iter->second);
			if (x)return x;
		}
		return nullptr;
	}

	void Entity::render() {
		if (!isRendered) return;
		if (isFixed) {
			if (model) {
				program3["fixed"] = true;
				program3["model"] = transform.getModel();
			}
			else {
				program2["transform"] = transform.getModel();
				program2["color"] = color;
			}
		}
		else {
			program3["fixed"] = false;
			program3["model"] = transform.getModel();
			program3["color"] = color;
		}
		if (!model) {
			program3["is2d"] = true;
		}
		if (as >= 0) { 
			anims[as]->go(lt - animStartTimepoint, this, animTps);
		}
		else {
			program3["has_bones"] = false;
		}
		if (model) { 
			model->render();
		}
	}

	void Entity::update() {
		if (isActive) {
			lt += dt;
			Update();
		}
	}

	void Entity::Update() {
		
	}

	void Entity::act(int kp, float progress) {
		animKp = kp;
		Act(kp, progress);
	}

	void Entity::Act(int kp, float progress) {
		
	}

	void Entity::addAnim(const std::string& name) {
		pAnimation anim = Animation::get(name);
		if (anim) { anims.push_back(anim); if (as < 0) { as = 0; } }
	}

	void Entity::addAnim(pAnimation& p) {
		if (p) { anims.push_back(p); if (as < 0) { as = 0; } }
	}
}