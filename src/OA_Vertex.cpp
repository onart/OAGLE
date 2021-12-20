#include "OA_Vertex.h"
#include "gl/glad/glad.h"
#include <map>
#include <string>

namespace onart {

	std::map<std::string, Model> models;

	void Model::rectModel(bool reset) {
		static unsigned VB = 0, IB = 0, VAO = 0;
		if (VAO) {
			if (!reset) return;
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}

		Vertex rect[] = {
			Vertex{vec3(0.5f,0.5f,0.0f),vec3(1,1,1),vec2(1,1)},
			Vertex{vec3(0.5f,-0.5f,0.0f),vec3(1,1,1),vec2(1,0)},
			Vertex{vec3(-0.5f,-0.5f,0.0f),vec3(1,1,1),vec2(0,0)},
			Vertex{vec3(-0.5f,0.5f,0.0f),vec3(1,1,1),vec2(0,1)},
		};

		unsigned indices[] = { 0,3,1,1,3,2 };
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

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
		models.erase("rect");
		models.insert({ "rect", Model(VAO, 36) });
	}

	void Model::circleModel(bool reset) {
		static unsigned VB = 0, IB = 0, VAO = 0;
		constexpr unsigned N = 36;
		if (VAO) {
			if (!reset) return;
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}

		Vertex* circ = new Vertex[N + 1];
		unsigned* indices = new unsigned[N * 3];
		circ[0] = { vec3(0), vec3(0,0,-1.0f), vec2(0.5f) };
		for (unsigned k = 0; k < N; k++)
		{
			float t = PI * 2.0f * k / float(N), c = cos(t), s = sin(t);
			circ[k + 1] = { vec3(c,s,0), vec3(0,0,-1.0f), vec2(c,s) * 0.5f + 0.5f };
		}
		for (unsigned k = 0; k < N; k++) {
			unsigned tk = k * 3;
			indices[tk] = 0;
			indices[tk + 1] = k + 1;
			indices[tk + 2] = k + 2;
		}
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (N + 1), circ, GL_STATIC_DRAW);

		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * N * 3, indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

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
		models.erase("circ");
		models.insert({ "circ", Model(VAO, N * 3) });
		delete[] circ, indices;
	}

	void Model::sphereModel(bool reset) {
		static unsigned VB = 0, IB = 0, VAO = 0;
		constexpr unsigned N = 36;
		if (VAO) {
			if (!reset) return;
			glDeleteBuffers(1, &VB);
			glDeleteBuffers(1, &IB);
			glDeleteVertexArrays(1, &VAO);
		}
		Vertex* sphr = new Vertex[(N + 1) * (2 * N + 1)];
		unsigned* indices = new unsigned[N * N * 12];
		int idx = 0;
		for (unsigned i = 0; i < N + 1; i++) {
			float th = i * PI / N;
			float sinth = sinf(th), costh = cosf(th);
			for (unsigned j = 0; j < N * 2 + 1; j++) {
				float phi = j * PI / N;
				vec3 pos(sinth * cosf(phi), sinth * sinf(phi), costh);
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
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * N * (N - 2), sphr, GL_STATIC_DRAW);

		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * N * N * 12, indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

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
		models.erase("sphr");
		models.insert({ "sphr", Model(VAO, N * N * 12) });
		delete[] sphr, indices;
	}

	void Model::cuboidModel(bool reset) {
		static unsigned VB = 0, IB = 0, VAO = 0;
		Vertex cube[] = {
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
		unsigned indices[] = {
		0,6,20,0,20,14,
		18,21,15,18,15,12,
		23,3,17,9,3,23,
		7,10,22,7,22,19,
		2,11,8,2,5,11,
		1,16,4,1,13,16
		};
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

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
		models.erase("cubo");
		models.insert({ "cubo", Model(VAO, 36) });
	}

}