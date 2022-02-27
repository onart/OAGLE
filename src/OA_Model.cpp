/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Model.h"
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

extern onart::Shader program3;

namespace onart {
	
	std::map<std::string, pModel> Model::list;
	
	constexpr unsigned int importFlags = aiPostProcessSteps::aiProcess_Triangulate |
		aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
		aiPostProcessSteps::aiProcess_GenSmoothNormals |
		aiPostProcessSteps::aiProcess_ImproveCacheLocality |
		aiPostProcessSteps::aiProcess_FindInstances |
		aiPostProcessSteps::aiProcess_SortByPType;

	constexpr int MAX_BONE_COUNT = 64;

	pTexture assimpReadTex(const aiMaterial* mtl, const std::filesystem::path& modelDirectory, const aiTextureType type) {
		namespace fs = std::filesystem;
		aiString texpath;
		if (mtl->GetTextureCount(type) == 1) {
			if (mtl->GetTexture(type, 0, &texpath) != aiReturn::aiReturn_SUCCESS) {
				return pTexture();
			}
			fs::path p = fs::absolute(modelDirectory / texpath.C_Str());
			if (!fs::exists(p)) {
				printf("텍스처 파일\n%s\n를 찾을 수 없습니다. 메모리에서 불러오려는 경우 이후 나오는 인덱스와 유형을 참고하여 호출해 주세요.\n", p.string().c_str());
				return pTexture();
			}
			else {
				return Material::genTextureFromFile(p.string().c_str());
			}
		}
		return 0;
	}

	pModel Model::load(const unsigned char* data, size_t len, const std::string& meshName, const char* hint) {
		if (Mesh::get(meshName)) return list[meshName];
		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFileFromMemory(data, len, importFlags, hint);
		if (!scn) { printf("Assimp 오류: %s\n", importer.GetErrorString()); return pModel(); }
		struct mdl :public Model { mdl(const aiScene* _1, const std::string& _2) :Model(_1, _2) {} };
		pModel& pm = list[meshName];
		pm.reset(new mdl(scn, meshName));
		return pm;
	}

	pModel Model::load(const std::string& file) {
		auto fullPath = std::filesystem::absolute(file).string();
		if (Mesh::get(fullPath)) return list[fullPath];
		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(fullPath, importFlags);
		if (!scn) { printf("Assimp 오류: %s\n", importer.GetErrorString()); return pModel(); }
		struct mdl :public Model { mdl(const aiScene* _1, const std::string& _2) :Model(_1, _2) {} };
		pModel& pm = list[fullPath];
		pm.reset(new mdl(scn, fullPath));
		return pm;
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
			for (Vertex& v : vList) {
				clamp4(v.boneIDs.entry, 0, MAX_BONE_COUNT);	// 셰이더 분기 제거를 위해 (-1에는 weight 0이 대응)
			}
#endif
			// 매터리얼
			if (materials[m->mMaterialIndex] == nullptr) {
				auto& material = materials[m->mMaterialIndex] = std::make_unique<Material>();

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
				pTexture difftex = assimpReadTex(mtl, dir, aiTextureType::aiTextureType_DIFFUSE);
				pTexture normtex = assimpReadTex(mtl, dir, aiTextureType::aiTextureType_NORMALS);
				if (difftex) material->setDiffuseTex(difftex);
				if (normtex) material->setBumpTex(normtex);
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

	void Model::addTex(unsigned index, std::shared_ptr<Texture>& tex, TexType typ) {
		if (index >= materials.size()) { fprintf(stderr, "%s: 인덱스가 잘못되었습니다.\n", __func__); return; }
		auto& mtl = materials[index];
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

	void Model::render(const vec4& color) const {
		program3.bind(**mesh);
		program3["nopiv"] = true;
		program3["is2d"] = false;
		program3["useFull"] = true;
		program3["color"] = color;
		for (auto& g : geom) {
			auto& mtl = materials[g.material];
			if (mtl) {
				program3["Ka"] = mtl->getAmbient();
				program3["Ks"] = mtl->getSpecular();
				program3["Kd"] = mtl->getDiffuse();
				program3["shininess"] = mtl->getShininess();

				unsigned df = mtl->getDiffuseTex();
				if (df) {
					program3.texture(df);
					program3["oneColor"] = false;
				}
				else {
					program3["oneColor"] = true;
				}
			}
			else {
				program3["oneColor"] = true;
			}
			program3.draw(g.start, g.count);
		}
	}
}