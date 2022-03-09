/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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
