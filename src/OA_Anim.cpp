#include "OA_Anim.h"
#include "OA_Shader.h"
#include "OA_Entity.h"
#include "OA_Vertex.h"

#ifdef _MSC_VER
#pragma comment(lib, "assimp/assimp.lib")
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
		if (!rects.empty()) {
			auto v = rects.begin()->second;
		}
	}

	void Animation2D::go(float elapsed, Entity* e, float dynamicTps) {
		static Mesh** rect = nullptr;
		if (!(*rect))rect = Mesh::get("rect");

		float tp = getTp(elapsed * dynamicTps);

		auto tlb = std::prev(tex.upper_bound(tp));
		if (tlb != tex.end()) {
			program3.texture(tlb->second);
		}

		auto rlb = std::prev(rects.upper_bound(tp));
		if (rlb != rects.end()) {
			program3.uniform("useFull", false);
			program3.uniform("ldwh", rlb->second);
			int i = (int)std::distance(rects.begin(), rlb);
			if (e->getAnimKey() != i)e->act(i);
		}
		else {
			program3.uniform("useFull", true);
		}

		if (hasPivot) {
			auto plb = std::prev(pivots.upper_bound(tp));
			// 베이스 직사각형 중심: (0,0), 양극단: +-0.5
			vec2 pivv = -(plb->second); pivv += 0.5f;
			vec2 xy(rlb->second.z, rlb->second.w);
			
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
		else {
			program3.uniform("nopiv", true);
		}
		program3.bind(**rect);
		program3.draw();
	}

	void Animation3D::go(float elapsed, Entity* e, float dynamicTps) {
		program3.use();
		float tp = getTp(elapsed * dynamicTps);
	}

	Animation3D::Animation3D(aiAnimation* anim, float duration, int tps, bool loop)
	:Animation(loop, duration, tps) {
		for (size_t k = 0; k < anim->mNumChannels; k++) {
			aiNodeAnim* cut = anim->mChannels[k];
			BoneAnim& ba = keys[cut->mNodeName.C_Str()] = BoneAnim();
			for (size_t i = 0; i < cut->mNumPositionKeys; i++) {
				aiVectorKey& kpt = cut->mPositionKeys[i];
				ba.keyPos[(float)kpt.mTime] = vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
			for (size_t i = 0; i < cut->mNumScalingKeys; i++) {
				aiVectorKey& kpt = cut->mScalingKeys[i];
				ba.keyScale[(float)kpt.mTime] = vec3(kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
			for (size_t i = 0; i < cut->mNumRotationKeys; i++) {
				aiQuatKey& kpt = cut->mRotationKeys[i];
				ba.keyRot[(float)kpt.mTime] = Quaternion(kpt.mValue.w, kpt.mValue.x, kpt.mValue.y, kpt.mValue.z);
			}
		}
	}

	Animation* Animation3D::load(const std::string& name, const std::string& file, bool loop) {
		Animation* anim = get(name);
		if (anim) return anim;

		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(name, 0);
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
		Animation* ret = new Animation3D(anim0, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop);
		
		delete scn;
		push(name, ret);
		return ret;
	}

	Animation* Animation3D::load(const std::string& name, const unsigned char* dat, size_t len, bool loop) {
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
		Animation* ret = new Animation3D(anim0, anim0->mDuration, int(anim0->mTicksPerSecond), loop);

		delete scn;
		push(name, ret);
		return ret;
	}
}