#ifndef __OA_PHYSICALSURFACE_H__
#define __OA_PHYSICALSURFACE_H__

namespace onart {
	// ������ �̵� ����, �ݹ߰� ����
	enum class PHYSICAL_SURFACE { DEFAULT_SURFACE, NEVERUSETHIS };
	constexpr size_t NTYPE = (size_t)PHYSICAL_SURFACE::NEVERUSETHIS;

	constexpr float STATIC_FRICTIONS[NTYPE][NTYPE] = {	// ���� ���� ���
		{1}
	};
	constexpr float DYNAMIC_FRICTIONS[NTYPE][NTYPE] = {	// � ���� ���
		{1}
	};
	constexpr float ELASTIC_VALUES[NTYPE][NTYPE] = {	// �ݹ� ���
		{0}
	};
}

#endif // !__OA_PHYSICALSURFACE_H__
