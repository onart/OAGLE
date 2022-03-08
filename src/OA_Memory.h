#ifndef __OA_MEMORY_H__
#define __OA_MEMORY_H__

#include "OA_Anim.h"
#include "OA_Audio.h"
#include "OA_Model.h"
#include "OA_Vertex.h"
#include "OA_Material.h"

namespace onart {
	namespace Memory
	{
		/// <summary>
		/// collect(false)를 적절한 순서로 호출합니다.
		/// collect()들은 엄밀히 말해 garbage collection은 아닙니다.
		/// </summary>
		inline void collect() {
			Audio::Source::collect();
			Animation::collect();
			Model::collect();
			Material::collect();
			Mesh::collect();
		}
	};
}

#endif // !__OA_MEMORY_H__
