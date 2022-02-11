#include "OA_Anim.h"
#include "OA_Shader.h"
#include "OA_Entity.h"
#include "OA_Vertex.h"

#ifdef _MSC_VER
#ifdef _WIN64
	#pragma comment(lib, "assimp/assimp64.lib")
#else
	#pragma comment(lib, "assimp/assimp32.lib")
#endif
	#pragma warning(disable: 26495 26812)
#endif

#include "externals/assimp/Importer.hpp"
#include "externals/assimp/scene.h"
#include "externals/assimp/postprocess.h"

#include <algorithm>

extern onart::Shader program3;

namespace onart {
	std::map<std::string, Animation*> Animation::animations;

	/// <summary>
	/// 정렬된 벡터에 대하여 현재 키포인트를 가리키는 반복자를 리턴합니다.
	/// <para>* 현재 시각이 첫 키포인트보다 작은 경우(정상적 상황이라면 있어서는 안 될 일) 첫 키포인트를 리턴합니다.</para>
	/// <para>* 3D 애니메이션의 경우 리턴값은 보간 대상 중 앞쪽을 뜻합니다.</para>
	/// <para>* (루프가 있는 애니메이션에 대하여 마지막, 즉 end-1 반복자가 리턴되는 경우는 정상적 상황이라면 있어서는 안 됨)</para>
	/// </summary>
	/// <param name="ar">정렬된 키포인트 벡터</param>
	/// <param name="tp">현재 시점</param>
	/// <returns></returns>
	template <class T>
	inline auto kpNow(const std::vector<Keypoint<T>>& ar, float tp) {
		auto i = std::upper_bound(ar.begin(), ar.end(), tp);
		return i == ar.begin() ? i : i - 1;
	}

	inline mat4 oam4(const aiMatrix4x4& m) { return mat4(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4); }

	Animation::Animation(bool loop, float duration, int staticTps)
		:loop(loop), duration(duration), staticTps(staticTps) {
		
	}

	Animation* Animation2D::make(const std::string& name, bool loop, const std::vector<Keypoint<Texture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots) {
		Animation* anim = get(name);
		if (anim) return anim;
		if (!pivots.empty() && pivots.size() != rects.size()) {
			fprintf(stderr,"pivots가 비어 있지 않은 경우 rects와 pivots의 길이는 같아야 합니다.\n");
			return nullptr;
		}
		// 시점순으로 정렬
		auto tx(tex);	auto rcts(rects);
		std::sort(tx.begin(), tx.end());
		std::sort(rcts.begin(), rcts.end());
		if (memcmp(rects.data(), rcts.data(), sizeof(Keypoint<vec4>) * rects.size()) != 0) {
			fprintf(stderr, "rects는 반드시 시점순으로 정렬된 상태로 입력되어야 합니다.\n");
			return nullptr;
		}
		// 텍스처 id만 시점순 저장
		size_t sz = tx.size(); std::vector<Keypoint<unsigned>> texz(sz);
		for (size_t i = 0; i < sz; i++) { texz[i] = { tx[i].tp,tx[i].value.id}; }
		
		// rect를 상대값으로 변형하여 저장
		const bool pv = !pivots.empty();
		sz = rcts.size(); std::vector<Keypoint<vec4>> rctz(sz);
		std::vector<vec4> sctrz(pivots.size());
		for (size_t i = 0; i < sz; i++) { 
			float timepoint = rcts[i].tp;
			const ivec2& wh = kpNow(tx, timepoint)->value.size;
			rctz[i] = { rcts[i].tp, rcts[i].value / vec4((float)wh.x, (float)wh.y, (float)wh.x, (float)wh.y) };
			if (pv) {
				// pivots를 가지고 원본 프레임 사이즈에 비례하게 크기 조절하는 벡터 추가
				vec2 pivv = vec2(0.5f) - pivots[i] / vec2(rects[i].value.z, rects[i].value.w);
				vec2 xy(rctz[i].value.z, rctz[i].value.w);
				xy *= vec2((float)wh.x, (float)wh.y) / 1024.0f;
				pivv *= xy;
				sctrz[i] = vec4(xy.x, xy.y, pivv.x, pivv.y);
			}
		}
		anim = new Animation2D(loop, texz, rctz, sctrz);
		push(name, anim);
		return anim;
	}

	Animation2D::Animation2D(bool loop, const std::vector<Keypoint<unsigned>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec4>& sctrs)
		: tex(tex), rects(rects), sctrs(sctrs), Animation(loop, rects.empty() ? 0 : rects.rbegin()->tp), hasRect(!rects.empty()), hasTex(!tex.empty()), hasPiv(!sctrs.empty()) {

	}

	void Animation2D::go(float elapsed, Entity* e, float dynamicTps) {
		static Mesh** rect = nullptr;
		if (!rect || !(*rect))rect = Mesh::get("rect");

		float tp = getTp(elapsed * dynamicTps);
		if (hasTex) {
			program3.uniform("oneColor", false);
			program3.texture(kpNow(tex, tp)->value);
		}
		else {
			program3.uniform("oneColor", true);
		}

		if (hasRect) {
			program3.uniform("useFull", false);
			auto l = std::upper_bound(rects.begin(), rects.end(), tp);
			int kp = 0;
			if (l == rects.begin()) { program3.uniform("ldwh", rects[0].value); }
			else { 
				l--;
				program3.uniform("ldwh", l->value);
				kp = int(l - rects.begin());
				if (e->getAnimKey() != kp) { e->act(kp); }
			}
			
			if (hasPiv) {
				program3.uniform("nopiv", false);
				vec4& sctr = sctrs[kp];
				mat4 pivMat(
					sctr.x, 0, 0, sctr.z,
					0, sctr.y, 0, sctr.w,
					0, 0, 1, 0,
					0, 0, 0, 1);
				program3.uniform("piv", pivMat);
			}
			else {
				program3.uniform("nopiv", true);
			}
		}
		else {
			program3.uniform("useFull", true);
			program3.uniform("nopiv", true);
		}
		program3.uniform("has_bones", false);
		program3.bind(**rect);
		program3.draw();
	}

	void Animation3D::go(float elapsed, Entity* e, float dynamicTps) {
		program3.use();
		float tp = getTp(elapsed * dynamicTps);
		auto sub = std::upper_bound(sigKp.begin(), sigKp.end(), tp);
		if (sub != sigKp.begin()) {
			int kp = int(sub - sigKp.begin() - 1);
			if (e->getAnimKey() != kp)e->act(kp);
		}
		
		for (auto& bone : keys) {
			bone.second.setTrans(tp);
		}

		setGlobalTrans(btree);
		int i = 0;
		for (Bone& m : u) {
			program3.uniform(("bones[" + std::to_string(i++) + ']').c_str(), m.uni);
		}
	}

	void Animation3D::readHierarchy(aiNode* root, BoneTree& tree) {
		if (n2i.find(root->mName.data) != n2i.end()) { tree.id = n2i[root->mName.data]; }
		else { tree.id = -int(n2i.size()); n2i[root->mName.data] = tree.id; }
		
		tree.transformation = oam4(root->mTransformation);
		for (size_t k = 0; k < root->mNumChildren; k++) {
			tree.children.push_back(BoneTree());
			readHierarchy(root->mChildren[k], tree.children[k]);
		}
	}

	Animation3D::Animation3D(const aiScene* scn, float duration, int tps, bool loop, const std::vector<float>& sig_kp)
		:Animation(loop, duration, tps), sigKp(sig_kp) {
		// 정점에 영향을 주는 뼈들
		for (unsigned k = 0; k < scn->mNumMeshes; k++) {
			aiMesh* m = scn->mMeshes[k];
			for (unsigned i = 0; i < m->mNumBones; i++) {
				aiBone* b = m->mBones[i];
				if (n2i.find(b->mName.data) == n2i.end()) {
					n2i[b->mName.C_Str()] = int(n2i.size());
					u.push_back(Bone(oam4(b->mOffsetMatrix)));
				}
			}
		}

		// 뼈 트리
		globalInverse = oam4(scn->mRootNode->mTransformation.Inverse());
		readHierarchy(scn->mRootNode, btree);

		// 애니메이션 키프레임
		aiAnimation* anim = scn->mAnimations[0];
		for (unsigned k = 0; k < anim->mNumChannels; k++) {
			aiNodeAnim* cut = anim->mChannels[k];
			BoneAnim& ba = keys[n2i[cut->mNodeName.C_Str()]] = BoneAnim();
			for (unsigned i = 0; i < cut->mNumPositionKeys; i++) {
				aiVectorKey& kpt = cut->mPositionKeys[i];
				ba.keyPos.push_back({ (float)kpt.mTime, vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z) });
			}
			for (unsigned i = 0; i < cut->mNumScalingKeys; i++) {
				aiVectorKey& kpt = cut->mScalingKeys[i];
				ba.keyScale.push_back({ (float)kpt.mTime, vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z) });
			}
			for (unsigned i = 0; i < cut->mNumRotationKeys; i++) {
				aiQuatKey& kpt = cut->mRotationKeys[i];
				ba.keyRot.push_back({ (float)kpt.mTime, Quaternion(kpt.mValue.w, kpt.mValue.x, kpt.mValue.y, kpt.mValue.z) });
			}	
		}
		std::sort(sigKp.begin(), sigKp.end());
	}

	Animation* Animation3D::load(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp) {
		Animation* anim = get(name);
		if (anim) return anim;

		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(name, 0);
		if (!scn) {
			printf("\nAssimp 오류: %s\n", importer.GetErrorString());
			return nullptr;
		}
		if (!scn->HasAnimations()) {
			printf("\n파일에 애니메이션이 없습니다.\n");
			return nullptr;
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		Animation* ret = new Animation3D(scn, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);
		
		push(name, ret);
		return ret;
	}

	Animation* Animation3D::load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::vector<float>& sig_kp) {
		Animation* anim = get(name);
		if (anim) return anim;
		Assimp::Importer importer;

		const aiScene* scn = importer.ReadFileFromMemory(dat, len, 0, ".dae");
		if (!scn) {
			printf("\nAssimp 오류: %s\n", importer.GetErrorString());
			return nullptr;
		}
		if (!scn->HasAnimations()) {
			printf("\n파일에 애니메이션이 없습니다\n");
			return nullptr;
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		Animation* ret = new Animation3D(scn, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);

		push(name, ret);
		return ret;
	}

	void Animation3D::BoneAnim::setTrans(float tp) {
		vec3 pos; vec3 scale(1); Quaternion rotation;
		if (!keyPos.empty()) {
			auto p1 = kpNow(keyPos, tp);
			if (p1 == keyPos.end() - 1 || keyPos.size() == 1) {
				pos = p1->value;
			}
			else {
				auto p2 = p1 + 1;
				float interp = (tp - p1->tp) / (p2->tp - p1->tp);
				if (interp < 0) { pos = p1->value; /* 애니메이션 키포인트가 잘못 지정된 (0부터 시작하지 않는) 케이스 */ }
				else { pos = lerp(p1->value, p2->value, interp); }
			}
		}
		if (!keyRot.empty()) {
			auto p1 = kpNow(keyRot, tp);
			if (p1 == keyRot.end() - 1 || keyRot.size() == 1) {
				rotation = p1->value;
			}
			else {
				auto p2 = p1 + 1;
				float interp = (tp - p1->tp) / (p2->tp - p1->tp);
				if (interp < 0) { rotation = p1->value; /* 애니메이션 키포인트가 잘못 지정된 (0부터 시작하지 않는) 케이스 */ }
				else { rotation = slerp(p1->value, p2->value, interp); }
			}
		}
		if (!keyScale.empty()) {
			auto p1 = kpNow(keyScale, tp);
			if (p1 == keyScale.end() - 1 || keyScale.size() == 1) {
				pos = p1->value;
			}
			else {
				auto p2 = p1 + 1;
				float interp = (tp - p1->tp) / (p2->tp - p1->tp);
				if (interp < 0) { scale = p1->value; /* 애니메이션 키포인트가 잘못 지정된 (0부터 시작하지 않는) 케이스 */ }
				else { scale = lerp(p1->value, p2->value, interp); }
			}
		}
		localTransform = mat4::TRS(pos, rotation, scale);
	}

	void Animation3D::setGlobalTrans(BoneTree& t, const mat4& parent) {
		mat4 nodeTransform = t.transformation;
		bool isMoving = keys.find(t.id) != keys.end();	// 애니메이션 키포인트 안에 있는가?
		if (isMoving) {	
			nodeTransform = keys[t.id].localTransform;
		}
		mat4 global = parent * nodeTransform;
		bool isBone = t.id >= 0;	// 정점에 직접적 연관이 있는가?
		if (isBone) {
			Bone b = u[t.id];
			b.uni = globalInverse * global * b.offset;
		}
		for (auto& ch : t.children) {
			setGlobalTrans(t, global);
		}
	}
}