﻿/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Vertex.h"
#include "externals/gl/glad/glad.h"
#pragma warning(disable: 26451)

namespace onart {

	std::map<std::string, ppMesh> Mesh::list;
	const char* RESERVED[] = { "circ", "clnd", "rect", "cubo", "sphr","icubo", "" };

	bool isReserved(const std::string& name) {
		for (char** p = (char**)RESERVED; (*p)[0]; p++) {
			if (name == (*p)) return true;
		}
		return false;
	}

	ppMesh Mesh::get(const std::string& name) {
		auto m = list.find(name);
		if (m == list.end()) return ppMesh();
		else return m->second;
	}

	Mesh::~Mesh() {
		glDeleteBuffers(1, &vb);
		glDeleteBuffers(1, &ib);
		glDeleteVertexArrays(1, &vao);
	}

	bool Mesh::add(const std::string& name, const std::vector<Vertex>& v, const std::vector<unsigned>& i) {
		if (isReserved(name))return false;
		unsigned vb, ib, vao;
		vao = createVAO(v, i, &vb, &ib);
		list[name].reset(new std::unique_ptr<Mesh>(new Mesh(vb, ib, vao, unsigned(i.size()))));
		return true;
	}

	void Mesh::drop(const std::string& name) {
		if (isReserved(name)) return;
		auto m = list.find(name);
		if (m != list.end()) {
			list.erase(m);
		}
	}

	void Mesh::rectModel() {
		static unsigned VB = 0, IB = 0, VAO = 0;
		
		if (VAO) {
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}
		// 공통적으로 tangent (1,0,0), bitangent (0,1,0)
		std::vector<Vertex> rect = {
			Vertex{vec3(0.5f,0.5f,0.0f),vec3(1,1,1),vec2(1,1)},
			Vertex{vec3(0.5f,-0.5f,0.0f),vec3(1,1,1),vec2(1,0)},
			Vertex{vec3(-0.5f,-0.5f,0.0f),vec3(1,1,1),vec2(0,0)},
			Vertex{vec3(-0.5f,0.5f,0.0f),vec3(1,1,1),vec2(0,1)},
		};

		std::vector<unsigned> indices = { 0,3,1,1,3,2,0,1,3,1,2,3 };

		VAO = createVAO(rect, indices, &VB, &IB);
		list["rect"].reset(new std::unique_ptr<Mesh>(new Mesh(VB, IB, VAO, 12)));
	}

	void Mesh::circleModel() {
		static unsigned VB = 0, IB = 0, VAO = 0;
		constexpr unsigned N = 36;
		if (VAO) {
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}

		// 공통적으로 tangent (1,0,0), bitangent (0,1,0)
		std::vector<Vertex> circ(N + 1);
		std::vector<unsigned> indices(N * 3);
		circ[0] = { vec3(0.f), vec3(0,0,-1.0f), vec2(0.5f) };
		for (unsigned k = 0; k < N; k++)
		{
			float t = PI * 2.0f * k / float(N);
			float c = cos(t), s = sin(t);
			circ[k + 1] = { vec3(c,s,0), vec3(0,0,-1.0f), vec2(c,s) * 0.5f + 0.5f };
		}
		for (unsigned k = 0; k < N; k++) {
			unsigned tk = k * 3;
			indices[tk] = 0;
			indices[tk + 1] = k + 1;
			indices[tk + 2] = k + 2;
		}
		indices[N * 3 - 1] = 1;
		VAO = createVAO(circ, indices, &VB, &IB);
		list["circ"].reset(new std::unique_ptr<Mesh>(new Mesh(VB, IB, VAO, N * 3)));
	}

	void Mesh::sphereModel() {
		static unsigned VB = 0, IB = 0, VAO = 0;
		constexpr unsigned N = 36;
		if (VAO) {
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}
		std::vector<Vertex> sphr((N + 1) * (2 * N + 1));
		std::vector<unsigned> indices(N * N * 12);
		int idx = 0;
		for (unsigned i = 0; i < N + 1; i++) {
			float th = i * PI / N;
			float sinth = sinf(th), costh = cosf(th);
			for (unsigned j = 0; j < N * 2 + 1; j++) {
				float phi = j * PI / N;
				vec3 pos(sinth * cosf(phi), sinth * sinf(phi), costh);
				// tangent (-sin(phi),0,cos(phi)), bitangent는 외적으로
				sphr[idx++] = Vertex{ pos, pos, vec2((float)j / (N * 2),1 - (float)i / N) };
			}
		}
		idx = 0;
#define rc(r,c) ((r)*(N*2+1)+c)
		for (unsigned i = 0; i < N; i++) {
			for (unsigned j = 0; j < N; j++) {
				indices[idx++] = rc(i, j);
				indices[idx++] = rc(i + 1, j + 1);
				indices[idx++] = rc(i, j + 1);
				indices[idx++] = rc(i, j);
				indices[idx++] = rc(i + 1, j);
				indices[idx++] = rc(i + 1, j + 1);
			}
		}
#undef rc
		VAO = createVAO(sphr, indices, &VB, &IB);
		list["sphr"].reset(new std::unique_ptr<Mesh>(new Mesh(VB, IB, VAO, N * N * 12)));
	}

	void Mesh::cuboidModel() {
		static unsigned VB = 0, IB = 0, VAO = 0;
		std::vector<Vertex> cube = {
		{vec3(-1,-1,-1),vec3(0,0,-1),vec2(1.0f / 2, 0)},
		{vec3(-1,-1,-1),vec3(0,-1,0),vec2(3.0f / 4, 1.0f / 3)},
		{vec3(-1,-1,-1),vec3(-1,0,0),vec2(3.0f / 4, 1.0f / 3)},

		{vec3(-1,-1,+1),vec3(0,0,+1),vec2(1.0f / 2, 1)},
		{vec3(-1,-1,+1),vec3(0,-1,0),vec2(3.0f / 4, 2.0f / 3)},
		{vec3(-1,-1,+1),vec3(-1,0,0),vec2(3.0f / 4, 2.0f / 3)},

		{vec3(-1,+1,-1),vec3(0,0,-1),vec2(1.0f / 4, 0)},
		{vec3(-1,+1,-1),vec3(0,1,0), vec2(0, 1.0f / 3)},
		{vec3(-1,+1,-1),vec3(-1,0,0),vec2(1, 1.0f / 3)},

		{vec3(-1,+1,+1),vec3(0,0,1),vec2(1.0f / 4, 1)},
		{vec3(-1,+1,+1),vec3(0,1,0),vec2(0, 2.0f / 3)},
		{vec3(-1,+1,+1),vec3(-1,0,0),vec2(1, 2.0f / 3)},

		{vec3(+1,-1,-1),vec3(1,0,0),vec2(1.0f / 2, 1.0f / 3)},
		{vec3(+1,-1,-1),vec3(0,-1,0),vec2(1.0f / 2, 1.0f / 3)},
		{vec3(+1,-1,-1),vec3(0,0,-1),vec2(1.0f / 2, 1.0f / 3)},

		{vec3(+1,-1,+1),vec3(1,0,0),vec2(1.0f / 2, 2.0f / 3)},
		{vec3(+1,-1,+1),vec3(0,-1,0),vec2(1.0f / 2, 2.0f / 3)},
		{vec3(+1,-1,+1),vec3(0,0,1),vec2(1.0f / 2, 2.0f / 3)},

		{vec3(+1,+1,-1),vec3(1,0,0),vec2(1.0f / 4, 1.0f / 3)},
		{vec3(+1,+1,-1),vec3(0,1,0),vec2(1.0f / 4, 1.0f / 3)},
		{vec3(+1,+1,-1),vec3(0,0,-1),vec2(1.0f / 4, 1.0f / 3)},

		{vec3(+1,+1,+1),vec3(1,0,0),vec2(1.0f / 4, 2.0f / 3)},
		{vec3(+1,+1,+1),vec3(0,1,0),vec2(1.0f / 4, 2.0f / 3)},
		{vec3(+1,+1,+1),vec3(0,0,1),vec2(1.0f / 4, 2.0f / 3)},
		};
		std::vector<unsigned> indices = {
		0,6,20,0,20,14,
		18,21,15,18,15,12,
		23,3,17,9,3,23,
		7,10,22,7,22,19,
		2,11,8,2,5,11,
		1,16,4,1,13,16
		};
		VAO = createVAO(cube, indices, &VB, &IB);
		list["cubo"].reset(new std::unique_ptr<Mesh>(new Mesh(VB, IB, VAO, 36)));
	}

	void Mesh::iCuboidModel() {
		static unsigned VB = 0, IB = 0, VAO = 0;
		std::vector<Vertex> cube = {
		{vec3(-1,-1,-1),vec3(0,0,-1),vec2(1.0f / 2, 0)},
		{vec3(-1,-1,-1),vec3(0,-1,0),vec2(3.0f / 4, 1.0f / 3)},
		{vec3(-1,-1,-1),vec3(-1,0,0),vec2(3.0f / 4, 1.0f / 3)},

		{vec3(-1,-1,+1),vec3(0,0,+1),vec2(1.0f / 2, 1)},
		{vec3(-1,-1,+1),vec3(0,-1,0),vec2(3.0f / 4, 2.0f / 3)},
		{vec3(-1,-1,+1),vec3(-1,0,0),vec2(3.0f / 4, 2.0f / 3)},

		{vec3(-1,+1,-1),vec3(0,0,-1),vec2(1.0f / 4, 0)},
		{vec3(-1,+1,-1),vec3(0,1,0), vec2(0, 1.0f / 3)},
		{vec3(-1,+1,-1),vec3(-1,0,0),vec2(1, 1.0f / 3)},

		{vec3(-1,+1,+1),vec3(0,0,1),vec2(1.0f / 4, 1)},
		{vec3(-1,+1,+1),vec3(0,1,0),vec2(0, 2.0f / 3)},
		{vec3(-1,+1,+1),vec3(-1,0,0),vec2(1, 2.0f / 3)},

		{vec3(+1,-1,-1),vec3(1,0,0),vec2(1.0f / 2, 1.0f / 3)},
		{vec3(+1,-1,-1),vec3(0,-1,0),vec2(1.0f / 2, 1.0f / 3)},
		{vec3(+1,-1,-1),vec3(0,0,-1),vec2(1.0f / 2, 1.0f / 3)},

		{vec3(+1,-1,+1),vec3(1,0,0),vec2(1.0f / 2, 2.0f / 3)},
		{vec3(+1,-1,+1),vec3(0,-1,0),vec2(1.0f / 2, 2.0f / 3)},
		{vec3(+1,-1,+1),vec3(0,0,1),vec2(1.0f / 2, 2.0f / 3)},

		{vec3(+1,+1,-1),vec3(1,0,0),vec2(1.0f / 4, 1.0f / 3)},
		{vec3(+1,+1,-1),vec3(0,1,0),vec2(1.0f / 4, 1.0f / 3)},
		{vec3(+1,+1,-1),vec3(0,0,-1),vec2(1.0f / 4, 1.0f / 3)},

		{vec3(+1,+1,+1),vec3(1,0,0),vec2(1.0f / 4, 2.0f / 3)},
		{vec3(+1,+1,+1),vec3(0,1,0),vec2(1.0f / 4, 2.0f / 3)},
		{vec3(+1,+1,+1),vec3(0,0,1),vec2(1.0f / 4, 2.0f / 3)},
		};
		std::vector<unsigned> indices = {
		0,20,6,0,14,20,
		18,15,21,18,12,15,
		23,17,3,9,23,3,
		7,22,10,7,19,22,
		2,8,11,2,11,5,
		1,4,16,1,16,13
		};
		VAO = createVAO(cube, indices, &VB, &IB);
		list["icubo"].reset(new std::unique_ptr<Mesh>(new Mesh(VB, IB, VAO, 36)));
	}

	unsigned Mesh::createVAO(const std::vector<Vertex>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib) {
		glGenBuffers(1, vb);
		glBindBuffer(GL_ARRAY_BUFFER, *vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * v.size(), &v[0], GL_STATIC_DRAW);

		glGenBuffers(1, ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * i.size(), &i[0], GL_STATIC_DRAW);

		return createVAO(*vb, *ib);
	}

	unsigned Mesh::generateVBIBVAO(void* vertData, unsigned* idxData, size_t vSize, size_t icount, unsigned* vb, unsigned* ib) {
		glGenBuffers(1, vb);
		glBindBuffer(GL_ARRAY_BUFFER, *vb);
		glBufferData(GL_ARRAY_BUFFER, vSize, vertData, GL_STATIC_DRAW);

		glGenBuffers(1, ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * icount, idxData, GL_STATIC_DRAW);

		unsigned vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, *vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib);

		return vao;
	}

	void Mesh::unbindVAO() {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned Mesh::createVAO(unsigned vb, unsigned ib) {
		unsigned vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

		glEnableVertexAttribArray(0);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glEnableVertexAttribArray(1);	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
		glEnableVertexAttribArray(2);	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tc));
#ifdef USE_BUMP
		glEnableVertexAttribArray(3);	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan));
		glEnableVertexAttribArray(4);	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitan));
#ifdef USE_ANIM
		glEnableVertexAttribArray(5);	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
		glEnableVertexAttribArray(6);	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));
#endif // USE_ANIM
#else
#ifdef USE_ANIM
		glEnableVertexAttribArray(3);	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
		glEnableVertexAttribArray(4);	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));
#endif // USE_ANIM
#endif
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return vao;
	}

	void Mesh::collect(bool removeUsing) {
		if (removeUsing) {
			std::map<std::string, ppMesh> nextList;
			for (size_t i = 0; i < sizeof(RESERVED) / sizeof(char*); i++) {
				auto ms = list.find(RESERVED[i]);
				if (ms != list.end()) {
					nextList[RESERVED[i]].reset(ms->second.get());
				}
			}
			list.swap(nextList);
			return;
		}
		for (auto it = list.cbegin(); it != list.cend();) {
			if (isReserved(it->first)) {
				++it;
			}
			else if (it->second.use_count() == 1) {
				list.erase(it++);
			}
			else {
				++it;
			}
		}
	}

	void floatAttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr); }
	void doubleAttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribLPointer(index, size, GL_DOUBLE, stride, ptr); }
	void int8AttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribIPointer(index, size, GL_BYTE, stride, ptr); }
	void int16AttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribIPointer(index, size, GL_SHORT, stride, ptr); }
	void int32AttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribIPointer(index, size, GL_INT, stride, ptr); }
	void uint8AttribPointer(unsigned index, int size, int stride, const void* ptr){ glVertexAttribIPointer(index, size, GL_UNSIGNED_BYTE, stride, ptr); }
	void uint16AttribPointer(unsigned index, int size, int stride, const void* ptr){ glVertexAttribIPointer(index, size, GL_UNSIGNED_SHORT, stride, ptr); }
	void uint32AttribPointer(unsigned index, int size, int stride, const void* ptr) { glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride, ptr); }
}