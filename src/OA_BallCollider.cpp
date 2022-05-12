#include "OA_BallCollider.h"
#include "binaryIOvec.h"
#include "OA_Model.h"
#include "OA_Shader.h"

extern onart::Shader program3;

namespace onart {

	std::vector<BallCollider2D*> BallCollider2D::objs;

	BallCollider2D::BallCollider2D(Entity* entity, float radius, const vec2& offset, Rigidbody2D* body, PHYSICAL_SURFACE surface)
		:entity(entity), radius(radius), offset(offset), body(body), surface((int)surface) {
		insort(objs, this);
		range();
	}

	BallCollider2D::~BallCollider2D() {
		removeFromSorted(objs, this);
	}

	void BallCollider2D::range() {
		if (!isActive) return;
		// 절대 위치(원 중심) 계산
		const vec3& pos0 = entity->getTransform()->getGlobalPosition();
		vec3 off = pos0 + entity->getTransform()->getLocalRotation().toMat3() * offset;
		vec2 vel = pos_vel - off;
		pos_vel = off;
		pos_vel[2] = vel[0];
		pos_vel[3] = vel[1];	// 충돌체 중심의 이전 프레임 대비 위치(=선속도)
		// 개괄 영역(직사각형) 계산
		vec4 ldru(off[0], off[1], off[0], off[1]);
		ldru += vec4(radius, radius, -radius, -radius);
		ldru *= INV_ONE_GRID;
		ivec4 ldruIDX;
		f2i(ldru.entry, ldruIDX.entry);
		rangex = rangey = (range_t)(~0);
		if (isInRange<0, PARTS>(ldruIDX[0])) { rangex >>= ldruIDX[0]; }
		if (isInRange<0, PARTS>(ldruIDX[1])) { rangey >>= ldruIDX[1]; }
		if (ldruIDX[2] < PARTS) { rangex -= ((range_t)(1) << (PARTS - 1 - ldruIDX[2])) - 1; }
		if (ldruIDX[3] < PARTS) { rangey -= ((range_t)(1) << (PARTS - 1 - ldruIDX[3])) - 1; }
	}

	void BallCollider2D::render() {
		program3["color"] = vec4(0, 1, 0, 0.3f);
		program3["model"] = mat4::TRS(vec3(pos_vel[0], pos_vel[1], -1), Quaternion(), radius);
		program3["piv"] = mat4();
		program3.texture(Material::get("white1x1")->id);
		program3.draw(**Mesh::get("circ"));
	}
}