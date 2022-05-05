#ifndef __OA_PHYSICALSYS_H__
#define __OA_PHYSICALSYS_H__

namespace onart {
	class PhysicalSys2D {
	public:
		void Update();
	private:
		void makeCollisions();
	};
}

#endif // !__OA_PHYSICALSYS_H__
