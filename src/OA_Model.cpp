#include "OA_Model.h"
#include "OA_Vertex.h"
#include "OA_Shader.h"
#include "OA_Material.h"

namespace onart {
	void Model::reloadMesh() {
		if (*mesh != nullptr) return;
		mesh = Mesh::get(meshName);
	}

	void Model::render(Shader& shader, const unsigned material, const vec4& color) {
		size_t sz = materials.size();	// 당장 1임을 가정
		shader.uniform("color", color);
		shader.uniform("Ka", materials[material]->getAmbient());
		shader.uniform("Kd", materials[material]->getDiffuse());
		shader.uniform("Ks", materials[material]->getSpecular());
			
		shader.texture(materials[material]->getDiffuseTex(), Shader::TexCode::SURFACE0);
		shader.draw(**mesh);
	}
}