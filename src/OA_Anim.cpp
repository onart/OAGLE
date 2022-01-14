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

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

extern onart::Shader program3;

namespace onart {
	std::map<std::string, Animation*> Animation::animations;

	Animation::Animation(bool loop, float duration, int staticTps)
		:loop(loop), duration(duration), staticTps(staticTps) {
		
	}

	Animation* Animation2D::make(const std::string& name, bool loop, const std::map<float, unsigned>& tex, const std::map<float, vec4>& rects, const std::map<float, vec2>& pivots) {
		Animation* anim = get(name);
		if (anim) return anim;
		anim = new Animation2D(loop, tex, rects, pivots);
		push(name, anim);
		return anim;
	}

	Animation2D::Animation2D(bool loop, const std::map<float, unsigned>& tex, const std::map<float, vec4>& rects, const std::map<float, vec2>& pivots)
		: tex(tex), rects(rects), pivots(pivots), Animation(loop, rects.empty() ? 0 : rects.rbegin()->first) {
		hasPivot = pivots.size();
		assert(!hasPivot || pivots.size() == rects.size() && "피벗을 설정하는 경우에는 각 rect에 대하여 피벗이 하나씩 대응해야 합니다.");
		assert(tex.empty() || tex.begin()->first == 0 && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
		assert(rects.empty() || rects.begin()->first == 0 && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
		assert(pivots.empty() || pivots.begin()->first == 0 && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
	}

	void Animation2D::go(float elapsed, Entity* e, float dynamicTps) {
		static Mesh** rect = nullptr;
		if (!rect || !(*rect))rect = Mesh::get("rect");

		float tp = getTp(elapsed * dynamicTps);
		if (tex.empty()) {
			program3.uniform("oneColor", true);
		}
		else {
			program3.uniform("oneColor", false);
			program3.texture(std::prev(tex.upper_bound(tp))->second);
		}
		
		if (rects.empty()) {
			program3.uniform("useFull", true);
		}
		else {
			program3.uniform("useFull", false);
			auto rub = std::prev(rects.upper_bound(tp));
			program3.uniform("ldwh", rub->second);
			int i = (int)std::distance(rects.begin(), rub);
			if (e->getAnimKey() != i)e->act(i);
			if (hasPivot) {
				auto pub = std::prev(pivots.upper_bound(tp));
				// 베이스 직사각형 중심: (0,0), 양극단: +-0.5
				vec2 pivv = -(pub->second); pivv += 0.5f;
				vec2 xy(rub->second.z, rub->second.w);

				/// 중심이동 후 크기변환
				mat4 pivMat(
					xy.x, 0, 0, xy.x * pivv.x,
					0, xy.y, 0, xy.y * pivv.y,
					0, 0, 1, 0,
					0, 0, 0, 1
				);
				program3.uniform("nopiv", false);
				program3.uniform("piv", pivMat);
			}
		}
		if (!hasPivot) {
			program3.uniform("nopiv", true);
		}
		program3.bind(**rect);
		program3.draw();
	}

	void Animation3D::go(float elapsed, Entity* e, float dynamicTps) {
		program3.use();
		float tp = getTp(elapsed * dynamicTps);

		auto slb = std::prev(sigKp.lower_bound(tp));
		if (slb != sigKp.end()) {
			int i = (int)std::distance(sigKp.begin(), slb);
			if (e->getAnimKey() != i)e->act(i);
		}
		
		for (auto& bone : keys) {
			bone.second.setTrans(tp);
		}

		setGlobalTrans(btree);
		int i = 0;
		for (mat4& m : u) {
			program3.uniform(("bones[" + std::to_string(i++) + ']').c_str(), m);
		}
	}

	Animation3D::Animation3D(aiAnimation* anim, float duration, int tps, bool loop, const std::set<float>& sig_kp)
		:Animation(loop, duration, tps), sigKp(sig_kp) {
		for (size_t k = 0; k < anim->mNumChannels; k++) {
			aiNodeAnim* cut = anim->mChannels[k];
			BoneAnim& ba = keys[cut->mNodeName.C_Str()] = BoneAnim();
			for (size_t i = 0; i < cut->mNumPositionKeys; i++) {
				aiVectorKey& kpt = cut->mPositionKeys[i];
				ba.keyPos[(float)kpt.mTime] = vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
			assert((cut->mNumPositionKeys == 0 || ba.keyPos.begin()->first == 0) && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
			for (size_t i = 0; i < cut->mNumScalingKeys; i++) {
				aiVectorKey& kpt = cut->mScalingKeys[i];
				ba.keyScale[(float)kpt.mTime] = vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
			assert((cut->mNumScalingKeys == 0 || ba.keyScale.begin()->first == 0) && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
			for (size_t i = 0; i < cut->mNumRotationKeys; i++) {
				aiQuatKey& kpt = cut->mRotationKeys[i];
				ba.keyRot[(float)kpt.mTime] = Quaternion(kpt.mValue.w, kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
			assert((cut->mRotationKeys == 0 || ba.keyRot.begin()->first == 0) && "첫 번째 키포인트의 시점은 반드시 0이어야 합니다.");
		}

	}

	Animation* Animation3D::load(const std::string& name, const std::string& file, bool loop, const std::set<float>& sig_kp) {
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
			delete scn;
			return nullptr;
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		Animation* ret = new Animation3D(anim0, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);
		
		delete scn;
		push(name, ret);
		return ret;
	}

	Animation* Animation3D::load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::set<float>& sig_kp) {
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
			delete scn;
			return nullptr;
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		Animation* ret = new Animation3D(anim0, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);

		delete scn;
		push(name, ret);
		return ret;
	}

	void Animation3D::BoneAnim::setTrans(float tp) {
		vec3 pos; vec3 scale(1);	Quaternion rotation;
		if (!keyPos.empty()) {
			auto p2 = keyPos.upper_bound(tp);
			if (p2 == keyPos.end()) {
				pos = std::prev(p2)->second;
			}
			else {
				auto p1 = std::prev(p2);
				float interp = (tp - p1->first) / (p2->first - p1->first);
				pos = lerp(p1->second, p2->second, interp);
			}
		}
		if (!keyRot.empty()) {
			auto p2 = keyRot.upper_bound(tp);
			if (p2 == keyRot.end()) {
				rotation = std::prev(p2)->second;
			}
			else {
				auto p1 = std::prev(p2);
				float interp = (tp - p1->first) / (p2->first - p1->first);
				rotation = slerp(p1->second, p2->second, interp);
			}
		}
		if (!keyScale.empty()) {
			auto p2 = keyScale.upper_bound(tp);
			if (p2 == keyScale.end()) {
				scale = std::prev(p2)->second;
			}
			else {
				auto p1 = std::prev(p2);
				float interp = (tp - p1->first) / (p2->first - p1->first);
				scale = lerp(p1->second, p2->second, interp);
			}
		}
		localTransform = mat4::TRS(pos, rotation, scale);
	}

	void Animation3D::setGlobalTrans(BoneTree& t, const mat4& parent) {
		mat4 nodeTransform = t.transformation;
		bool isBone = keys.find(t.name) != keys.end();
		if (isBone) {
			nodeTransform = keys[t.name].localTransform;
		}
		mat4 global = parent * nodeTransform;
		if (isBone) {
			
		}
		for (auto& ch : t.children) {
			setGlobalTrans(t, global);
		}
	}
}