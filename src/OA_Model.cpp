#include "OA_Model.h"
#include "OA_Vertex.h"
#include "OA_Shader.h"
#include "OA_Material.h"

namespace onart {
	void Model::reloadMesh() {
		if (*mesh != nullptr) return;
		mesh = Mesh::get(meshName);
	}

	void Model::render(const Shader& shader) {
		size_t sz = materials.size();	// 당장 1임을 가정
		shader.uniform("color", color);
		for (size_t i = 0; i < sz; i++) {
			shader.uniform("Ka", materials[i]->getAmbient());
			shader.uniform("Kd", materials[i]->getDiffuse());
			shader.uniform("Ks", materials[i]->getSpecular());
			
			shader.texture(materials[i]->getDiffuseTex(), Shader::TexCode::SURFACE0);
			shader.draw(**mesh);
		}
	}
}