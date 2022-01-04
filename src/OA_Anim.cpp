#include "OA_Anim.h"
#include "OA_Shader.h"
#include "OA_Entity.h"

extern onart::Shader program3;

namespace onart {
	std::map<std::string, Animation*> Animation::animations;

	Animation::Animation(bool loop, float duration, int staticTps, float dynamicTps) :loop(loop), duration(duration), staticTps(staticTps), dynamicTps(dynamicTps) {
		
	}

	Animation2D::Animation2D(bool loop, const std::map<float, unsigned>& tex, const std::map<float, vec4>& rects, const std::map<float, vec2>& pivots)
		: tex(tex), rects(rects), pivots(pivots), Animation(loop, rects.empty() ? 0 : rects.rbegin()->first) {
		hasPivot = pivots.size();
		if (!rects.empty()) {
			auto v = rects.begin()->second;
			wh0.x = v.z; wh0.y = v.w;
		}
	}

	void Animation2D::go(float elapsed, Entity* e) {
		float tp = getTp(elapsed);

		auto tlb = std::prev(tex.upper_bound(tp));
		if (tlb != tex.end()) {
			program3.texture(tlb->second);
		}

		auto rlb = std::prev(rects.upper_bound(tp));
		if (rlb != rects.end()) {
			program3.uniform("useFull", false);
			program3.uniform("ldwh", rlb->second);
			int i = (int)std::distance(rects.begin(), rlb);
			e->act(i);
		}
		else {
			program3.uniform("useFull", true);
		}

		if (hasPivot) {
			auto plb = std::prev(pivots.upper_bound(tp));
			// 베이스 직사각형 중심: (0,0), 양극단: +-0.5
			vec2 pivv = -(plb->second); pivv += 0.5f;
			vec2 xy(rlb->second.z, rlb->second.w);
			
			/// 중심이동 후 크기변환
			mat4 pivMat(
				xy.x, 0, 0, xy.x * pivv.x,
				0, xy.y, 0, xy.y * pivv.y,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			program3.uniform("nopiv", false);
			program3.uniform("piv", pivMat);
		}
		else {
			program3.uniform("nopiv", true);
		}
	}
}