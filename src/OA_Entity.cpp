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

extern onart::Shader program3;
extern float tp, dt;

namespace onart {

	const float& Entity::tp = ::tp;
	const float& Entity::dt = ::dt;

	std::multimap<Entity::EntityKey, Entity*> Entity::entities;

	Entity::Entity(const EntityKey& k, const Transform& transform) :key(k), transform(transform), localTp(lt), animState(as), animStartTimepoint(lt) {

	}

	Entity::~Entity() {
		auto ub = entities.upper_bound(key);
		for (auto iter = entities.lower_bound(key); iter != ub; iter++) {
			if (iter->second == this) {
				entities.erase(iter);
				return;
			}
		}
	}

	std::vector<Entity*> Entity::gets(const EntityKey& k) {
		auto ub = entities.upper_bound(k);
		std::vector<Entity*> v;
		v.reserve(entities.count(k));
		for (auto iter = entities.lower_bound(k); iter != ub; iter++) {
			v.push_back(iter->second);
		}
		return v;
	}

	Entity* Entity::get(const EntityKey& k) {
		auto e = entities.find(k);
		if (e == entities.end()) return nullptr;
		else return e->second;
	}

	void Entity::render() {
		program3.use();	// 이 부분 수정 필요
		program3.uniform("model", transform.getModel());
		
		if (as >= 0) { 
			anims[as]->go(lt - animStartTimepoint, this, animTps); 
		}
		else {
			program3.uniform("has_bones", false);
		}
		if (model) { 
			program3.uniform("nopiv", true);
			program3.uniform("useFull", true);
			model->render(program3); 
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

	void Entity::act(int kp) {
		animKp = kp;
		Act(kp);
	}

	void Entity::Act(int kp) {

	}

	void Entity::addAnim(const std::string& name) {
		Animation* anim = Animation::get(name);
		if (anim) { anims.push_back(anim); if (as < 0) { as = 0; } }
	}
}