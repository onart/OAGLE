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
		program3.use();
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