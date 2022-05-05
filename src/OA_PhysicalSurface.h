#ifndef __OA_PHYSICALSURFACE_H__
#define __OA_PHYSICALSURFACE_H__

namespace onart {
	// 정지와 이동 마찰, 반발값 정의
	constexpr size_t N_TYPE=10;
	constexpr float STATIC_FRICTIONS[N_TYPE][N_TYPE];
	constexpr float DYNAMIC_FRICTIONS[N_TYPE][N_TYPE];
	constexpr float ELASIC_VALUES[N_TYPE][N_TYPE];
}

#endif // !__OA_PHYSICALSURFACE_H__

