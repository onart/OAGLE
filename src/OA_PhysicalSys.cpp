#include "OA_PhysicalSys.h"
#include "OA_Rigidbody.h"
#include "OA_Collider.h"

namespace onart{
	constexpr size_t PARTITION_DEPTH = 6;	// �̺� ��Ƽ�� �ִ� ����
	constexpr size_t SUB_THRESHOLD = 9;		// �� ��Ƽ�� ���� �̰� ������ ��ü�� �ִ� ��� ��Ƽ�� �Ÿ��� �ߴ�, �ִ� ���̱��� �������� �� �̻��� ����� �״�� ����
	constexpr float WORLD_SCOPE = 100.0f;	// �����ϴ� ���� ũ�⸦ ��� ���δ� ����(�߽��� ����, ����, ����(, ����)�� �� ũ��)
	constexpr bool USE_DISCRETE = true;

	static class HalfPartition2D {
	public:		
		HalfPartition2D(size_t depth = PARTITION_DEPTH, float side = WORLD_SCOPE, const vec2& center = 0.0f);
		void push(Collider2D*);
		inline void clear() { 
			content.clear();
			for (HalfPartition2D& part : parts) part.clear();
		}
		inline size_t size() { return content.size(); }
	private:
		vec2 center;
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

	}

	void PhysicalSys2D::Update() {
		if constexpr (USE_DISCRETE) {
			// �̻� �浹 �˰���: ������Ʈ �� ��ģ �κ��� �ذ��ϱ�
			// ����: ��� �и��� ����
			// ����: �ε�ε�Ÿ��� ����
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
		for (Collider2D* col : Collider2D::objs) {
			hp2d.push(col);
		}
		if constexpr (USE_DISCRETE) {
			
		}
		else if constexpr (true) {

		}
	}
}