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
	/// ���ĵ� ���Ϳ� ���Ͽ� ���� Ű����Ʈ�� ����Ű�� �ݺ��ڸ� �����մϴ�.
	/// <para>* ���� �ð��� ù Ű����Ʈ���� ���� ���(������ ��Ȳ�̶�� �־�� �� �� ��) ù Ű����Ʈ�� �����մϴ�.</para>
	/// <para>* 3D �ִϸ��̼��� ��� ���ϰ��� ���� ��� �� ������ ���մϴ�.</para>
	/// <para>* (������ �ִ� �ִϸ��̼ǿ� ���Ͽ� ������, �� end-1 �ݺ��ڰ� ���ϵǴ� ���� ������ ��Ȳ�̶�� �־�� �� ��)</para>
	/// </summary>
	/// <param name="ar">���ĵ� Ű����Ʈ ����</param>
	/// <param name="tp">���� ����</param>
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
			fprintf(stderr,"pivots�� ��� ���� ���� ��� rects�� pivots�� ���̴� ���ƾ� �մϴ�.\n");
			return nullptr;
		}
		// ���������� ����
		auto tx(tex);	auto rcts(rects);
		std::sort(tx.begin(), tx.end());
		std::sort(rcts.begin(), rcts.end());
		if (memcmp(rects.data(), rcts.data(), sizeof(Keypoint<vec4>) * rects.size()) != 0) {
			fprintf(stderr, "rects�� �ݵ�� ���������� ���ĵ� ���·� �ԷµǾ�� �մϴ�.\n");
			return nullptr;
		}
		// �ؽ�ó id�� ������ ����
		size_t sz = tx.size(); std::vector<Keypoint<unsigned>> texz(sz);
		for (size_t i = 0; i < sz; i++) { texz[i] = { tx[i].tp,tx[i].value.id}; }
		
		// rect�� ��밪���� �����Ͽ� ����
		const bool pv = !pivots.empty();
		sz = rcts.size(); std::vector<Keypoint<vec4>> rctz(sz);
		std::vector<vec4> sctrz(pivots.size());
		for (size_t i = 0; i < sz; i++) { 
			float timepoint = rcts[i].tp;
			const ivec2& wh = kpNow(tx, timepoint)->value.size;
			rctz[i] = { rcts[i].tp, rcts[i].value / vec4((float)wh.x, (float)wh.y, (float)wh.x, (float)wh.y) };
			if (pv) {
				// pivots�� ������ ���� ������ ����� ����ϰ� ũ�� �����ϴ� ���� �߰�
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
		program3.uniform("has_bones", true);
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
		// ������ ������ �ִ� ����
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

		// �� Ʈ��
		globalInverse = oam4(scn->mRootNode->mTransformation.Inverse());
		readHierarchy(scn->mRootNode, btree);

		// �ִϸ��̼� Ű������
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
		const aiScene* scn = importer.ReadFile(file, 0);
		if (!scn) {
			printf("\nAssimp ����: %s\n", importer.GetErrorString());
			return nullptr;
		}
		if (!scn->HasAnimations()) {
			printf("\n���Ͽ� �ִϸ��̼��� �����ϴ�.\n");
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
			printf("\nAssimp ����: %s\n", importer.GetErrorString());
			return nullptr;
		}
		if (!scn->HasAnimations()) {
			printf("\n���Ͽ� �ִϸ��̼��� �����ϴ�\n");
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

				if (interp < 0) { pos = p1->value; /* �ִϸ��̼� Ű����Ʈ�� �߸� ������ (0���� �������� �ʴ�) ���̽� */ }
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
				if (interp < 0) { rotation = p1->value; /* �ִϸ��̼� Ű����Ʈ�� �߸� ������ (0���� �������� �ʴ�) ���̽� */ }
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
				if (interp < 0) { scale = p1->value; /* �ִϸ��̼� Ű����Ʈ�� �߸� ������ (0���� �������� �ʴ�) ���̽� */ }
				else { scale = lerp(p1->value, p2->value, interp); }
			}
		}
		localTransform = mat4::TRS(pos, rotation, scale);
	}

	void Animation3D::setGlobalTrans(BoneTree& t, const mat4& parent) {
		mat4 nodeTransform = t.transformation;
		bool isMoving = keys.find(t.id) != keys.end();	// �ִϸ��̼� Ű����Ʈ �ȿ� �ִ°�?
		if (isMoving) {	
			nodeTransform = keys[t.id].localTransform;
		}
		mat4 global = parent * nodeTransform;
		bool isBone = t.id >= 0;	// ������ ������ ������ �ִ°�?
		if (isBone) {
			Bone& b = u[t.id];
			b.uni = globalInverse * global * b.offset;
		}
		for (auto& ch : t.children) {
			setGlobalTrans(ch, global);
		}
	}
}