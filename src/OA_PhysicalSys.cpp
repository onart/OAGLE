#include "OA_PhysicalSys.h"
#include "OA_Rigidbody.h"
#include "OA_Collider.h"

namespace onart{
	constexpr size_t PARTITION_DEPTH = 6;	// 이분 파티션 최대 깊이
	constexpr size_t SUB_THRESHOLD = 9;		// 한 파티션 내에 이것 이하의 개체가 있는 경우 파티션 거름을 중단, 최대 깊이까지 내려가도 이 이상인 경우라면 그대로 진행
	constexpr float WORLD_SCOPE = 100.0f;	// 상정하는 월드 크기를 모두 감싸는 상자(중심이 영점, 가로, 세로(, 높이)가 이 크기)
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
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] + side)));	// 좌측 상단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] + side)));	// 우측 상단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] - side)));	// 좌측 하단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] - side)));	// 우측 하단
	}

	void HalfPartition2D::push(Collider2D* col) {

	}

	void PhysicalSys2D::Update() {
		if constexpr (USE_DISCRETE) {
			// 이산 충돌 알고리즘: 업데이트 후 겹친 부분을 해결하기
			// 장점: 모듈 분리가 쉬움
			// 단점: 부들부들거리기 쉬움
			for (Rigidbody2D* obj : Rigidbody2D::objs) { 
				obj->UpdateV();
				obj->UpdateP();
			}
			makeCollisions();
		}
		else if constexpr (true) {
			// 연속 충돌 알고리즘: 업데이트 중 만날 시점을 계산하기
			// 장점: 신뢰도가 100%에 가까우며 부들부들거리지 않음
			// 단점: 원형 충돌기 외의 회전 구현이 까다롭고 모듈 분리가 어려움
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