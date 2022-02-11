#include "OA_Model.h"
#include "OA_Vertex.h"
#include "OA_Shader.h"
#include "OA_Material.h"

#ifdef _MSC_VER
#ifdef _WIN64
#pragma comment(lib, "assimp/assimp64.lib")
#elif defined(_WIN32)
#pragma comment(lib, "assimp/assimp32.lib")
#endif
#pragma warning(disable: 26495 26812)
#endif

#include "externals/assimp/Importer.hpp"
#include "externals/assimp/scene.h"
#include "externals/assimp/postprocess.h"

#include <filesystem>

namespace onart {
	
	std::map<std::string, Model*> Model::list;
	
	constexpr unsigned int importFlags = aiPostProcessSteps::aiProcess_Triangulate |
		aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
		aiPostProcessSteps::aiProcess_GenSmoothNormals |
		aiPostProcessSteps::aiProcess_ImproveCacheLocality |
		aiPostProcessSteps::aiProcess_FindInstances |
		aiPostProcessSteps::aiProcess_SortByPType;

	unsigned assimpReadTex(const aiMaterial* mtl, const std::filesystem::path& modelDirectory, const aiTextureType type) {
		namespace fs = std::filesystem;
		aiString texpath;
		if (mtl->GetTextureCount(type) == 1) {
			if (mtl->GetTexture(type, 0, &texpath) != aiReturn::aiReturn_SUCCESS) {
				return 0;
			}
			fs::path p = fs::absolute(modelDirectory / texpath.C_Str());
			if (!fs::exists(p)) {
				printf("텍스처 파일\n%s\n를 찾을 수 없습니다. 메모리에서 불러오려는 경우 이후 나오는 인덱스와 유형을 참고하여 호출해 주세요.\n", p.string().c_str());
				return 0;
			}
			else {
				return Material::genTextureFromFile(p.string().c_str()).id;
			}
		}
		return 0;
	}

	Model* Model::load(const unsigned char* data, size_t len, const std::string& meshName, const char* hint) {
		if (Mesh::get(meshName)) return list[meshName];
		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFileFromMemory(data, len, importFlags, hint);
		if (!scn) { printf("Assimp 오류: %s\n", importer.GetErrorString()); return nullptr; }
		list[meshName] = new Model(scn, meshName);
		return list[meshName];
	}

	Model* Model::load(const std::string& file) {
		auto fullPath = std::filesystem::absolute(file).string();
		if (Mesh::get(fullPath)) return list[fullPath];
		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(fullPath, importFlags);
		if (!scn) { printf("Assimp 오류: %s\n", importer.GetErrorString()); return nullptr; }
		return list[file] = new Model(scn, fullPath);
	}

	Model::Model(const aiScene* scn, const std::string& name) {
		size_t vCount = 0, iCount = 0;
		
		for (unsigned mid = 0; mid < scn->mNumMeshes; mid++) {
			const aiMesh* m = scn->mMeshes[mid];
			vCount += m->mNumVertices;
			if (m->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE) {
				for (size_t fid = 0; fid < m->mNumFaces; fid++) {
					iCount += m->mFaces[fid].mNumIndices;
				}
			}
		}
		
		std::vector<Vertex> vList;
		std::vector<unsigned> iList;
		std::map<std::string, int> bList;
		materials.resize(scn->mNumMaterials, nullptr);
		vList.reserve(vCount);
		iList.reserve(iCount);
		
		for (unsigned mid = 0; mid < scn->mNumMeshes; mid++) {
			const aiMesh* m = scn->mMeshes[mid];
			if (m->mPrimitiveTypes != aiPrimitiveType::aiPrimitiveType_TRIANGLE) continue;
			
			const size_t v0 = vList.size();
			const size_t i0 = iList.size();
			// 인덱스 저장
			for (size_t fid = 0; fid < m->mNumFaces; fid++) {
				const aiFace& face = m->mFaces[fid];
				auto ind = iList.insert(iList.end(), face.mIndices, face.mIndices + face.mNumIndices);
				const auto& ed = iList.end();
				for (; ind < ed; ind++) *ind += (unsigned)v0;
			}
			
			// 정점 데이터 저장
			if (m->HasTextureCoords(0)) {
				for (size_t vid = 0; vid < m->mNumVertices; vid++) {
					const aiVector3D& pos = m->mVertices[vid];
					const aiVector3D& norm = m->mNormals[vid];
					const aiVector3D& tc = m->mTextureCoords[0][vid];
					vList.push_back(std::move(Vertex{ vec3(pos.x,pos.y,pos.z),vec3(norm.x,norm.y,norm.z),vec2(tc.x,tc.y) }));
				}
			}
			else {
				for (size_t vid = 0; vid < m->mNumVertices; vid++) {
					const aiVector3D& pos = m->mVertices[vid];
					const aiVector3D& norm = m->mNormals[vid];
					vList.push_back(std::move(Vertex{ vec3(pos.x,pos.y,pos.z),vec3(norm.x,norm.y,norm.z) }));
				}
			}
#ifdef USE_BUMP
			if (m->HasTangentsAndBitangents()) {
				for (size_t vid = 0; vid < m->mNumVertices; vid++) {
					const aiVector3D& tan = m->mTangents[vid];
					const aiVector3D& bitan = m->mBitangents[vid];
					vList[vid + v0].tan = vec3(tan.x, tan.y, tan.z);
					vList[vid + v0].bitan = vec3(bitan.x, bitan.y, bitan.z);
				}
			}
#endif
#ifdef USE_ANIM
			for (unsigned bid = 0; bid < m->mNumBones; bid++) {
				aiBone* bk = m->mBones[bid];
				int bone;
				if (bList.find(bk->mName.C_Str()) == bList.end()) {
					bone = int(bList.size());
					bList[bk->mName.C_Str()] = bone;
				}
				else {
					bone = bList[m->mBones[bid]->mName.C_Str()];
				}
				for (unsigned wid = 0; wid < bk->mNumWeights; wid++) {
					size_t vid = bk->mWeights[wid].mVertexId + v0;
					ivec4& inf = vList[vid].boneIDs;
					vec4& ws = vList[vid].boneWeights;
					for (int kk = 0; kk < 4; kk++) {
						if (inf[kk] == -1) {
							inf[kk] = bone;
							ws[kk] = bk->mWeights[wid].mWeight;
							break;
						}
						else if (inf[kk] == bone) {
							return;
						}
					}
				}
			}
#endif
			// 매터리얼
			Material* material = nullptr;
			if (materials[m->mMaterialIndex] == nullptr) {
				material = materials[m->mMaterialIndex] = new Material();

				const aiMaterial* mtl = scn->mMaterials[m->mMaterialIndex];
				
				vec4 buf;
				mtl->Get(AI_MATKEY_COLOR_AMBIENT, reinterpret_cast<aiColor4D&>(buf));	material->setAmbient(buf);
				mtl->Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor4D&>(buf));	material->setDiffuse(buf);
				mtl->Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor4D&>(buf));	material->setSpecular(buf);
				mtl->Get(AI_MATKEY_COLOR_EMISSIVE, reinterpret_cast<aiColor4D&>(buf));	material->setEmissive(buf);
				float fbu;
				mtl->Get(AI_MATKEY_SHININESS, fbu);	material->setShininess(fbu);
				mtl->Get(AI_MATKEY_OPACITY, fbu);	material->setAlpha(fbu);
				mtl->Get(AI_MATKEY_REFRACTI, fbu);	material->setRefractiveIndex(fbu);
				auto dir = std::filesystem::path(name).parent_path();
				unsigned difftex = assimpReadTex(mtl, dir, aiTextureType::aiTextureType_DIFFUSE);
				unsigned normtex = assimpReadTex(mtl, dir, aiTextureType::aiTextureType_NORMALS);
				material->setDiffuseTex(difftex);
				material->setBumpTex(normtex);
#ifdef _DEBUG
				if (difftex == 0) {
					printf("모델 [%s]의 %d번 diffuse 텍스처를 불러올 수 없었습니다. 메모리에서 불러오고자 하는 경우, 생성한 객체에 대하여 addTex() 함수를 호출해 주시기 바랍니다.\n", name.c_str(), m->mMaterialIndex);
				}
				if (normtex == 0) {
					printf("모델 [%s]의 %d번 normal 텍스처를 불러올 수 없었습니다. 메모리에서 불러오고자 하는 경우, 생성한 객체에 대하여 addTex() 함수를 호출해 주시기 바랍니다.\n", name.c_str(), m->mMaterialIndex);
				}
#endif
			}
			geom.push_back(Geometry{ (unsigned)i0,unsigned(iList.size() - i0),m->mMaterialIndex });
		}
		Mesh::add(name, vList, iList);
		mesh = Mesh::get(name);
	}

	void Model::reloadMesh() {
		if (*mesh != nullptr) return;
		mesh = Mesh::get(meshName);
	}

	void Model::addTex(unsigned index, unsigned tex, TexType typ) {
		if (index >= materials.size()) { printf("%s: 인덱스가 잘못되었습니다.\n", __func__); return; }
		Material* mtl = materials[index];
		if (!mtl) { return; }
		switch (typ)
		{
		case onart::Model::TexType::AMBIENT:
			mtl->setAmbientTex(tex);
			break;
		case onart::Model::TexType::DIFFUSE:
			mtl->setDiffuseTex(tex);
			break;
		case onart::Model::TexType::SPECULAR:
			mtl->setSpecularTex(tex);
			break;
		case onart::Model::TexType::NORMAL:
			mtl->setBumpTex(tex);
			break;
		default:
			break;
		}
	}

	void Model::render(Shader& shader, const int material, const vec4& color) const {
		shader.use();
		shader.bind(**mesh);
		shader.uniform("nopiv", true);
		shader.uniform("useFull", true);
		shader.uniform("is2d", false);
		shader.uniform("color", color);
		for (auto& g : geom) {
			Material* mtl = materials[g.material];
			if (mtl) {
				shader.uniform("Ka", mtl->getAmbient());
				shader.uniform("Kd", mtl->getDiffuse());
				shader.uniform("Ks", mtl->getSpecular());
				shader.uniform("shininess", mtl->getShininess());

				unsigned df = mtl->getDiffuseTex();
				if (df) {
					shader.texture(df);
					shader.uniform("oneColor", false);
				}
				else {
					shader.uniform("oneColor", true);
				}
			}
			else {
				shader.uniform("oneColor", true);
			}
			shader.draw(g.start, g.count, (**mesh).getIB());
		}
	}
}