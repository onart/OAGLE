#ifndef __OA_PHYSICALSYS_H__
#define __OA_PHYSICALSYS_H__

#include <vector>
#include "oaglem.h"

namespace onart {
	
	class Collider2D;

	class PhysicalSys2D {
	public:
		static void Update();
	private:
		static void makeCollisions();
	};
}

#endif // !__OA_PHYSICALSYS_H__
