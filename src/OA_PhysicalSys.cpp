#include "OA_PhysicalSys.h"
#include "OA_Rigidbody.h"
#include "OA_Collider.h"

namespace onart{
	constexpr size_t PARTITION_DEPTH = 6;	// 이분 파티션 최대 깊이
	constexpr size_t SUB_THRESHOLD = 9;		// 한 파티션 내에 이것 이하의 개체가 있는 경우 파티션 거름을 중단, 최대 깊이까지 내려가도 이 이상인 경우라면 그대로 진행
	constexpr float WORLD_SCOPE = 100.0f;	// 상정하는 월드 크기를 모두 감싸는 상자(중심이 영점, 가로, 세로(, 높이)가 이 크기의 2배), 이 크기를 넘어가는 경우들은 각각 계층 없이 모두 비교
	constexpr bool USE_DISCRETE = true;

	static class HalfPartition2D {	// 이후 메타프로그래밍 고려
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
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] + side)));	// 좌측 상단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] + side)));	// 우측 상단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] - side, center[y] - side)));	// 좌측 하단
		parts.emplace_back(HalfPartition2D(depth - 1, side, vec2(center[x] + side, center[y] - side)));	// 우측 하단
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
					if (c1->check0(c2)) {	// 개괄 판정 1
						
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
			// 이산 충돌 알고리즘: 업데이트 후 겹친 부분을 해결하기
			// 장점: 모듈 분리가 쉬움
			// 단점: 부들부들거리기 쉬운 걸 명시적으로 막아야 함
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
		for (Collider2D* col : Collider2D::objs) { if(col->isActive) hp2d.push(col); }
		if constexpr (USE_DISCRETE) {
			hp2d.check();
		}
		else if constexpr (true) {

		}
	}
}