#ifndef __OA_PHYSICALSURFACE_H__
#define __OA_PHYSICALSURFACE_H__

namespace onart {
	// 정지와 이동 마찰, 반발값 정의
	enum class PHYSICAL_SURFACE { DEFAULT_SURFACE, NEVERUSETHIS };
	constexpr size_t NTYPE = (size_t)PHYSICAL_SURFACE::NEVERUSETHIS;

	constexpr float STATIC_FRICTIONS[NTYPE][NTYPE] = {	// 정지 마찰 계수
		{1}
	};
	constexpr float DYNAMIC_FRICTIONS[NTYPE][NTYPE] = {	// 운동 마찰 계수
		{1}
	};
	constexpr float ELASTIC_VALUES[NTYPE][NTYPE] = {	// 반발 계수
		{0}
	};
}

#endif // !__OA_PHYSICALSURFACE_H__
