#include "OA_Entity.h"
#include "OA_Shader.h"
#include "OA_Model.h"
#include "OA_Anim.h"

extern float tp, dt;
extern onart::Shader program3;

namespace onart {

	const float& Entity::tp = ::tp;
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
		anims[as]->go(lt - animStartTimepoint, this, animTps);
		model->render(program3);
		
		//program3.draw(model);
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
}