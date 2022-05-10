#include "OA_PhysicalSys.h"
#include "OA_Rigidbody.h"
#include "OA_Collider.h"

namespace onart{
	constexpr size_t PARTITION_DEPTH = 6;	// �̺� ��Ƽ�� �ִ� ����
	constexpr size_t SUB_THRESHOLD = 9;		// �� ��Ƽ�� ���� �̰� ������ ��ü�� �ִ� ��� ��Ƽ�� �Ÿ��� �ߴ�, �ִ� ���̱��� �������� �� �̻��� ����� �״�� ����
	constexpr float WORLD_SCOPE = 100.0f;	// �����ϴ� ���� ũ�⸦ ��� ���δ� ����(�߽��� ����, ����, ����(, ����)�� �� ũ���� 2��), �� ũ�⸦ �Ѿ�� ������ ���� ���� ���� ��� ��
	constexpr bool USE_DISCRETE = true;

	static class HalfPartition2D {	// ���� ��Ÿ���α׷��� ���
	public:		
		HalfPartition2D(size_t depth = PARTITION_DEPTH, float side = WORLD_SCOPE, const vec2& center = 0.0f);
		void push(Collider2D*);
		inline void clear() { 
			if (content.size()) {
				content.clear();
				for (HalfPartition2D& part : parts) part.clear();
			}
		}
		inline size_t size() { return content.size(); }
		void check();
	private:
		void push(Collider2D*, const vec4& ldru);
		const vec2 center;
		std::vector<HalfPartition2D> parts;
		std::vector<Collider2D*> content;
	}hp2d;

	HalfPartition2D::HalfPartition2D(size_t depth, float side, const vec2& center) :center(center) {
		USE_NVEC_XYZW;
		if (depth == 0) return;
		side *= 0.5f;
		parts.reserve(4);
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] + side)));	// ���� ���
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] + side)));	// ���� ���
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] - side)));	// ���� �ϴ�
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] - side)));	// ���� �ϴ�
	}

	void HalfPartition2D::push(Collider2D* col) {
		USE_NVEC_XYZW;
		
		vec2 pos = col->getPos();
		vec4 ldru(pos[x] - col->baseRadius, pos[y] - col->baseRadius, pos[x] + col->baseRadius, pos[y] + col->baseRadius);
		push(col, ldru);
	}

	void HalfPartition2D::push(Collider2D* col, const vec4& ldru) {
		content.push_back(col);
		if (!parts.empty()) {
			USE_NVEC_XYZW;
			if (ldru[0] < center[x]) {
				if (ldru[1] < center[y]) parts[2].push(col, ldru);
				if (ldru[3] > center[y]) parts[0].push(col, ldru);
			}
			if (ldru[2] < center[x]) {
				if (ldru[1] < center[y]) parts[3].push(col, ldru);
				if (ldru[3] > center[y]) parts[1].push(col, ldru);
			}
		}
	}

	void HalfPartition2D::check() {
		size_t sz = size();
		if (sz <= SUB_THRESHOLD) {
			for (size_t i = 0; i < sz; i++) {
				Collider2D* c1 = content[i];
				for (size_t j = i + 1; j < sz; j++) {
					Collider2D* c2 = content[j];
					if (c1->check0(c2)) {	// ���� ���� 1
						
					}
				}
			}
		}
		else{
			for (HalfPartition2D& part : parts) part.check();
		}
	}

	void PhysicalSys2D::Update() {
		if constexpr (USE_DISCRETE) {
			// �̻� �浹 �˰���: ������Ʈ �� ��ģ �κ��� �ذ��ϱ�
			// ����: ��� �и��� ����
			// ����: �ε�ε�Ÿ��� ���� �� ��������� ���ƾ� ��
			for (Rigidbody2D* obj : Rigidbody2D::objs) { 
				obj->UpdateV();
				obj->UpdateP();
			}
			makeCollisions();
		}
		else if constexpr (true) {
			// ���� �浹 �˰���: ������Ʈ �� ���� ������ ����ϱ�
			// ����: �ŷڵ��� 100%�� ������ �ε�ε�Ÿ��� ����
			// ����: ���� �浹�� ���� ȸ�� ������ ��ٷӰ� ��� �и��� �����
			for (Rigidbody2D* obj : Rigidbody2D::objs) {
				obj->UpdateV();
			}
			makeCollisions();
			for (Rigidbody2D* obj : Rigidbody2D::objs) {
				obj->UpdateP();
			}
		}
	}

	void PhysicalSys2D::makeCollisions() {
		for (Collider2D* col : Collider2D::objs) { if(col->isActive) hp2d.push(col); }
		if constexpr (USE_DISCRETE) {
			hp2d.check();
		}
		else if constexpr (true) {

		}
	}
}