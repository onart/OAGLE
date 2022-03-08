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
		/// collect(false)�� ������ ������ ȣ���մϴ�.
		/// collect()���� ������ ���� garbage collection�� �ƴմϴ�.
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
