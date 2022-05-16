/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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

USE_SHADER_UNIFORM;
USE_NVEC_XYZW_UPPER;
USE_NVEC_LDWH_UPPER;

namespace onart {
	std::map<std::string, pAnimation> Animation::animations;
	std::map<int, mat4> Animation3D::recentGlobalTransforms;
	bool Animation::actSwitch = true;	// 애니메이션 상태에 개체가 반응하는 함수를 호출할지 정합니다. (true=호출) 초기값은 true이며 프레임버퍼를 이용한 deferred 렌더링 등을 할 때 개체 이벤트를 한 번만 발생시키도록 활용할 수 있습니다. 예를 들어, 애니메이션 액션 함수에 의하여 위치가 직접적으로 옮겨지는 경우(권장되지 않는 바지만)라면 모델을 마지막으로 그릴 때만 act를 활성화시킬 수 있습니다.

	static ppMesh mRect;
	
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

	pAnimation Animation::get(const std::string& name) {
		auto i = animations.find(name);
		if (i != animations.end()) return i->second;
		return pAnimation();
	}

	void Animation::drop(const std::string& name) {
		animations.erase(name);
	}

	void Animation::collect(bool removeUsing) {
		if (removeUsing) { animations.clear(); return; }
		for (auto iter = animations.cbegin(); iter != animations.cend();) {
			if (iter->second.use_count() == 1) {
				animations.erase(iter++);
			}
			else {
				++iter;
			}
		}
	}

	pAnimation Animation2D::make(const std::string& name, bool loop, const std::vector<Keypoint<pTexture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots) {
		pAnimation anim = get(name);
		if (anim) return anim;
		if (!pivots.empty() && pivots.size() != rects.size()) {
			fprintf(stderr,"pivots가 비어 있지 않은 경우 rects와 pivots의 길이는 같아야 합니다.\n");
			return pAnimation();
		}
		// 시점순으로 정렬
		auto tx(tex);
		std::sort(tx.begin(), tx.end());
		if (!std::is_sorted(rects.begin(), rects.end())) {
			fprintf(stderr, "rects는 반드시 시점순으로 정렬된 상태로 입력되어야 합니다.\n");
			return pAnimation();
		}

		// rect를 상대값으로 변형하여 저장
		const bool pv = !pivots.empty();
		size_t sz = rects.size(); std::vector<Keypoint<vec4>> rctz(sz);
		std::vector<vec4> sctrz(pivots.size());
		for (size_t i = 0; i < sz; i++) { 
			float timepoint = rects[i].tp;
			const ivec2& wh = kpNow(tx, timepoint)->value->size;
			rctz[i] = { rects[i].tp, rects[i].value / vec4((float)wh[X], (float)wh[Y], (float)wh[X], (float)wh[Y])};
			if (pv) {
				// pivots를 가지고 원본 프레임 사이즈에 비례하게 크기 조절하는 벡터 추가
				vec2 pivv = vec2(0.5f) - pivots[i] / vec2(rects[i].value[WIDTH], rects[i].value[HEIGHT]);
				vec2 xy(rctz[i].value[WIDTH], rctz[i].value[HEIGHT]);
				xy *= vec2((float)wh[X], (float)wh[Y]) / 1024.0f;
				pivv *= xy;
				sctrz[i] = vec4(xy[X], xy[Y], pivv[X], pivv[Y]);
			}
		}
		struct anim2d :public Animation2D { 
			anim2d(bool _1, const std::vector<Keypoint<pTexture>>& _2, const std::vector<Keypoint<vec4>>& _3, const std::vector<vec4>& _4):Animation2D(_1,_2,_3,_4) {}
		};
		pAnimation a2d = std::make_shared<anim2d>(loop, tx, rctz, sctrz);
		push(name, a2d);
		return a2d;
	}

	Animation2D::Animation2D(bool loop, const std::vector<Keypoint<pTexture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec4>& sctrs)
		: tex(tex), rects(rects), sctrs(sctrs), Animation(loop, rects.empty() ? 0 : rects.rbegin()->tp), hasRect(!rects.empty()), hasTex(!tex.empty()), hasPiv(!sctrs.empty()) {
	}

	void Animation2D::go(float elapsed, Entity* e, float dynamicTps) {
		if (!mRect)mRect = Mesh::get("rect");
		
		float tp = getTp(elapsed * dynamicTps);
		if (hasTex) {
			Game::program3[oneColor] = false;
			Game::program3.texture(kpNow(tex, tp)->value->id);
		}
		else {
			Game::program3[oneColor] = true;
		}

		if (hasRect) {
			Game::program3[useFull] = false;
			auto l = std::upper_bound(rects.begin(), rects.end(), tp);
			int kp = 0;
			if (l == rects.begin()) { Game::program3[ldwh] = rects[0].value; }
			else { 
				l--;
				Game::program3[ldwh] = l->value;
				kp = int(l - rects.begin());
				int ak = e->getAnimKey();
				if (ak != kp) {
					if (ak < 0) {
						if (l + 1 != rects.end()) {
							float nextp = (l + 1)->tp;
							e->act(kp, (tp - l->tp) / (nextp - l->tp));
						}
						else {
							e->act(kp);
						}
					}
					else {
						e->act(kp);
					}
				}
			}
			
			if (hasPiv) {
				vec4& sctr = sctrs[kp];
				mat4 pivMat(
					sctr[0], 0, 0, sctr[2],
					0, sctr[1], 0, sctr[3],
					0, 0, 1, 0,
					0, 0, 0, 1);
				Game::program3[piv] = pivMat;
			}
			else {
				Game::program3[piv] = mat4();
			}
		}
		else {
			Game::program3[useFull] = true;
			Game::program3[piv] = mat4();
		}
		Game::program3[hasBones] = false;
		Game::program3[is2d] = true;
		Game::program3.bind(**mRect);
		Game::program3.draw();
	}

	pAnimation Sprite::make(const std::string& name, const pTexture& tex, vec4 rect, vec2 pivot) {
		pAnimation anim = get(name);
		if (anim) return anim;
		vec4 ldwh;
		const ivec2& wh = tex->size;
		if (rect == vec4()) { ldwh = vec4(0, 0, 1, 1); rect = vec4(0, 0, (float)wh[X], (float)wh[Y]); }
		else { ldwh = rect / vec4((float)wh[X], (float)wh[Y], (float)wh[X], (float)wh[Y]); }
		if (isnan(pivot[0])) {
			pivot = vec2((float)wh[X], (float)wh[Y]) / 2;
		}
		vec2 pivv = vec2(0.5f) - pivot / vec2(rect[WIDTH], rect[HEIGHT]);
		vec2 xy(ldwh[WIDTH], ldwh[HEIGHT]);
		xy *= vec2((float)wh[X], (float)wh[Y]) / 1024;
		pivv *= xy;
		vec4 sctr(xy[X], xy[Y], pivv[X], pivv[Y]);
		struct spr :public Sprite {
			spr(const pTexture& _1, const vec4& _2, const vec4& _3) :Sprite(_1, _2, _3) {}
		};
		pAnimation a2d = std::make_shared<spr>(tex, ldwh, sctr);
		push(name, a2d);
		return a2d;
	}

	Sprite::Sprite(const pTexture& tex, const vec4& rect, const vec4& sctr) :tex(tex), ldwh(rect), sctr(sctr), Animation(false, 0) {

	}

	void Sprite::go(float, Entity*, float) {
		if (!mRect)mRect = Mesh::get("rect");
		if (tex) { 
			Game::program3[oneColor] = false;
			Game::program3.texture(tex->id);
		}
		else {
			Game::program3[oneColor] = true;
		}
		Game::program3[useFull] = false;
		Game::program3[::ldwh] = ldwh;
		mat4 pivMat(
			sctr[0], 0, 0, sctr[2],
			0, sctr[1], 0, sctr[3],
			0, 0, 1, 0,
			0, 0, 0, 1);
		Game::program3[piv] = pivMat;
		Game::program3[hasBones] = false;
		Game::program3[is2d] = true;
		Game::program3.bind(**mRect);
		Game::program3.draw();
	}

	pAnimation FixedSprite::make(const std::string& name, const pTexture& tex, vec4 rect, vec2 pivot) {
		pAnimation anim = get(name);
		if (anim) return anim;
		vec4 ldwh;
		const ivec2& wh = tex->size;
		if (rect == vec4()) { ldwh = vec4(0, 0, 1, 1); rect = vec4(0, 0, (float)wh[0], (float)wh[1]); }
		else { ldwh = rect / vec4((float)wh[0], (float)wh[1], (float)wh[0], (float)wh[1]); }
		if (isnan(pivot[0])) {
			pivot = vec2((float)wh[0], (float)wh[1]) / 2;
		}
		vec2 pivv = vec2(0.5f) - pivot / vec2(rect[2], rect[3]);
		vec2 xy(ldwh[2], ldwh[3]);
		xy *= vec2((float)wh[0], (float)wh[1]) / 1024;
		pivv *= xy;
		vec4 sctr(xy[0], xy[1], pivv[0], pivv[1]);
		// print(sctrz[0]);	// 버튼에 사용하기 위한 값을 알아낼 수 있습니다.
		struct spr :public FixedSprite {
			spr(const pTexture& _1, const vec4& _2, const vec4& _3) :FixedSprite(_1, _2, _3) { }
		};
		pAnimation a2d = std::make_shared<spr>(tex, ldwh, sctr);
		push(name, a2d);
		return a2d;
	}

	FixedSprite::FixedSprite(const pTexture& tex, const vec4& rect, const vec4& sctr) :tex(tex), ldwh(rect), sctr(sctr), Animation(false, 0) {

	}

	void FixedSprite::go(float, Entity*, float) {
		if (!mRect)mRect = Mesh::get("rect");
		if (tex) {
			Game::program2[oneColor] = false;
			Game::program2.texture(tex->id);
		}
		else {
			Game::program2[oneColor] = true;
		}
		Game::program2[useFull] = false;
		Game::program2[::ldwh] = ldwh;
		Game::program2[isText] = false;
		mat4 pivMat(
			sctr[0], 0, 0, sctr[2],
			0, sctr[1], 0, sctr[3],
			0, 0, 1, 0,
			0, 0, 0, 1);
		Game::program2[piv] = pivMat;
		Game::program2.bind(**mRect);
		Game::program2.draw();
	}

	void Animation3D::go(float elapsed, Entity* e, float dynamicTps) {
		Game::program3.use();
		float tp = getTp(elapsed * dynamicTps);
		auto sub = std::upper_bound(sigKp.begin(), sigKp.end(), tp);
		for (auto& bone : keys) {
			bone.second.setTrans(tp);
		}
		setGlobalTrans(btree);
		if (sub != sigKp.begin()) {
			int kp = int(sub - sigKp.begin() - 1);
			//if (e->getAnimKey() != kp)e->act(kp);
			int ak = e->getAnimKey();
			if (ak != kp) {
				if (ak < 0) {
					if (sub + 1 != sigKp.end()) {
						float nextp = *(sub + 1);
						e->act(kp, (tp - *sub) / (nextp - *sub));
					}
					else {
						e->act(kp);
					}
				}
				else {
					e->act(kp);
				}
			}
			// if (e->getAnimKey() != kp)e->act(kp);
		}
		int i = 0;
		for (Bone& m : u) {
			Game::program3[bones][i++] = m.uni;
		}
		Game::program3[hasBones] = true;
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
		//n2i.clear();
	}

	pAnimation Animation3D::load(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp) {
		pAnimation anim = get(name);
		if (anim) return anim;

		Assimp::Importer importer;
		const aiScene* scn = importer.ReadFile(file, 0);
		if (!scn) {
			printf("\nAssimp 오류: %s\n", importer.GetErrorString());
			return pAnimation();
		}
		if (!scn->HasAnimations()) {
			printf("\n파일에 애니메이션이 없습니다.\n");
			return pAnimation();
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		struct anim3d :public Animation3D {
			anim3d(const aiScene* _1, float _2, int _3, bool _4, const std::vector<float>& _5) :Animation3D(_1, _2, _3, _4, _5) {}
		};
		pAnimation ret = std::make_shared<anim3d>(scn, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);

		push(name, ret);
		return get(name);
	}

	pAnimation Animation3D::load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::vector<float>& sig_kp) {
		pAnimation anim = get(name);
		if (anim) return anim;
		Assimp::Importer importer;

		const aiScene* scn = importer.ReadFileFromMemory(dat, len, 0, ".dae");
		if (!scn) {
			printf("\nAssimp 오류: %s\n", importer.GetErrorString());
			return pAnimation();
		}
		if (!scn->HasAnimations()) {
			printf("\n파일에 애니메이션이 없습니다\n");
			return pAnimation();
		}
		aiAnimation* anim0 = scn->mAnimations[0];
		struct anim3d :public Animation3D {
			anim3d(const aiScene* _1, float _2, int _3, bool _4, const std::vector<float>& _5) :Animation3D(_1, _2, _3, _4, _5) {}
		};
		pAnimation ret = std::make_shared<anim3d>(scn, float(anim0->mDuration), int(anim0->mTicksPerSecond), loop, sig_kp);
		
		push(name, ret);
		return get(name);
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
		mat4 global = parent * nodeTransform;	// 외부 클래스에서 이 값에 접근이 가능해야 함
		bool isBone = t.id >= 0;	// 정점에 직접적 연관이 있는가?
		if (isBone) {
			recentGlobalTransforms[t.id] = global;
			Bone& b = u[t.id];
			b.uni = globalInverse * global * b.offset;
		}
		for (auto& ch : t.children) {
			setGlobalTrans(ch, global);
		}
	}

	void Animation3D::exportBin(const std::string& fileName) {
		FILE* fp;
		fopen_s(&fp, (fileName + ".oanim").c_str(), "wb");
		if(fp){
			{
				float f = getDuration();
				int i = getTps();
				fwrite(&f, sizeof(float), 1, fp);
				fwrite(&i, sizeof(int), 1, fp);
				fwrite(&globalInverse, sizeof(mat4), 1, fp);
			}
			// vector<Bone> u;
			int sz = (int)u.size();
			fwrite(&sz, sizeof(int), 1, fp);
			for (Bone& b : u) { fwrite(&b.offset, sizeof(mat4), 1, fp); }
			// map<int, BoneAnim> keys;
			sz = (int)keys.size();
			fwrite(&sz, sizeof(int), 1, fp);
			for (auto& k : keys) {
				fwrite(&k.first, sizeof(int), 1, fp);
				sz = (int)k.second.keyPos.size();
				fwrite(&sz, sizeof(int), 1, fp);
				fwrite(k.second.keyPos.data(), sizeof(Keypoint<vec3>), sz, fp);
				sz = (int)k.second.keyRot.size();
				fwrite(&sz, sizeof(int), 1, fp);
				fwrite(k.second.keyRot.data(), sizeof(Keypoint<Quaternion>), sz, fp);
				sz = (int)k.second.keyScale.size();
				fwrite(&sz, sizeof(int), 1, fp);
				fwrite(k.second.keyScale.data(), sizeof(Keypoint<vec3>), sz, fp);
			}
			// BoneTree btree;
			static struct {
				void operator()(BoneTree& bt, FILE* fp) {
					fwrite(&bt.id, sizeof(int), 1, fp);
					int sz = (int)bt.children.size();
					fwrite(&sz, sizeof(int), 1, fp);
					fwrite(&bt.transformation, sizeof(mat4), 1, fp);
					for (BoneTree& child : bt.children) {
						operator()(child, fp);
					}
				}
			}traverse;
			traverse(btree, fp);
			fclose(fp);
		}
		else {
			perror("fopen");
		}
	}

	pAnimation Animation3D::loadBin(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp) {
		FILE* fp;
		fopen_s(&fp, file.c_str(), "rb");
		if (fp) {
			struct anim3d :public Animation3D {
				anim3d(FILE* _1, float _2, int _3, bool _4, const std::vector<float>& _5) :Animation3D(_1, _2, _3, _4, _5) {}
			};
			float dur;
			fread(&dur, sizeof(float), 1, fp);
			int tps;
			fread(&tps, sizeof(int), 1, fp);
			pAnimation ret = std::make_shared<anim3d>(fp, dur, tps, loop, sig_kp);
			fclose(fp);
			return ret;
		}
		else {
			perror("fopen");
			return pAnimation();
		}
	}

	Animation3D::Animation3D(FILE* fp, float duration, int tps, bool loop, const std::vector<float>& sig_kp)
		:Animation(loop, duration, tps) {
		fread(&globalInverse, sizeof(mat4), 1, fp);
		int sz;
		// vector<Bone> u;
		fread(&sz, sizeof(int), 1, fp);
		u.reserve(sz);
		mat4* off = (mat4*)malloc(sizeof(mat4) * sz);	// 생성자 호출 x
		if (!off) exit(1);
		fread(off, sizeof(mat4), sz, fp);
		mat4* off2 = off;
		for (int i = 0; i < sz; i++, off2++) { u.push_back(Bone(*off2)); }
		free(off);
		// map<int, BoneAnim> keys;
		fread(&sz, sizeof(int), 1, fp);
		for (int i = 0; i < sz; i++) {
			int id; fread(&id, sizeof(int), 1, fp);
			BoneAnim& ba = keys[id];
			fread(&sz, sizeof(int), 1, fp);
			ba.keyPos.resize(sz);
			fread(&ba.keyPos[0], sizeof(Keypoint<vec3>), sz, fp);
			fread(&sz, sizeof(int), 1, fp);
			ba.keyRot.resize(sz);
			fread(&ba.keyRot[0], sizeof(Keypoint<Quaternion>), sz, fp);
			fread(&sz, sizeof(int), 1, fp);
			ba.keyScale.resize(sz);
			fread(&ba.keyScale[0], sizeof(Keypoint<vec3>), sz, fp);
		}
		// BoneTree btree;
		static struct {
			void operator()(BoneTree& bt, FILE* fp) {
				fread(&bt.id, sizeof(int), 1, fp);
				int sz; fread(&sz, sizeof(int), 1, fp);
				fread(&bt.transformation, sizeof(mat4), 1, fp);
				bt.children.resize(sz);
				for (BoneTree& child : bt.children) {
					operator()(child, fp);
				}
			}
		}traverse;
		traverse(btree, fp);
	}

	void Animation3D::exportCode(const std::string& fileName) {

	}

	void UIAnimation::go(float elapsed, Entity* e, float dynamicTps) {		
		if (!mRect)mRect = Mesh::get("rect");

		float tp = getTp(elapsed * dynamicTps);
		if (hasTex) {
			Game::program2[oneColor] = false;
			Game::program2.texture(kpNow(tex, tp)->value->id);
		}
		else {
			Game::program2[oneColor] = true;
		}

		if (hasRect) {
			Game::program2[useFull] = false;
			auto l = std::upper_bound(rects.begin(), rects.end(), tp);
			int kp = 0;
			if (l == rects.begin()) { Game::program2[ldwh] = rects[0].value; }
			else {
				l--;
				Game::program2[ldwh] = l->value;
				kp = int(l - rects.begin());
				int ak = e->getAnimKey();
				if (ak != kp) {
					if (ak < 0) {
						if (l + 1 != rects.end()) {
							float nextp = (l + 1)->tp;
							e->act(kp, (tp - l->tp) / (nextp - l->tp));
						}
						else {
							e->act(kp);
						}
					}
					else {
						e->act(kp);
					}
				}
			}

			if (hasPiv) {
				vec4& sctr = sctrs[kp];
				mat4 pivMat(
					sctr[0], 0, 0, sctr[2],
					0, sctr[1], 0, sctr[3],
					0, 0, 1, 0,
					0, 0, 0, 1);
				Game::program2[piv] = pivMat;
			}
			else {
				Game::program2[piv] = mat4();
			}
		}
		else {
			Game::program2[useFull] = true;
			Game::program2[piv] = mat4();
		}
		Game::program2.bind(**mRect);
		Game::program2.draw();
	}

	UIAnimation::UIAnimation(bool loop, const std::vector<Keypoint<pTexture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec4>& sctrs)
		:tex(tex), rects(rects), sctrs(sctrs), Animation(loop, rects.empty() ? 0 : rects.rbegin()->tp), hasRect(!rects.empty()), hasTex(!tex.empty()), hasPiv(!sctrs.empty()) {

	}

	pAnimation UIAnimation::make(const std::string& name, bool loop, const std::vector<Keypoint<pTexture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots) {
		pAnimation anim = get(name);
		if (anim) return anim;
		if (!pivots.empty() && pivots.size() != rects.size()) {
			fprintf(stderr, "pivots가 비어 있지 않은 경우 rects와 pivots의 길이는 같아야 합니다.\n");
			return pAnimation();
		}
		// 시점순으로 정렬
		auto tx(tex);
		std::sort(tx.begin(), tx.end());
		if (!std::is_sorted(rects.begin(), rects.end())) {
			fprintf(stderr, "rects는 반드시 시점순으로 정렬된 상태로 입력되어야 합니다.\n");
			return pAnimation();
		}

		// rect를 상대값으로 변형하여 저장
		const bool pv = !pivots.empty();
		size_t sz = rects.size(); std::vector<Keypoint<vec4>> rctz(sz);
		std::vector<vec4> sctrz(pivots.size());
		for (size_t i = 0; i < sz; i++) {
			float timepoint = rects[i].tp;
			const ivec2& wh = kpNow(tx, timepoint)->value->size;
			rctz[i] = { rects[i].tp, rects[i].value / vec4((float)wh[X], (float)wh[Y], (float)wh[X], (float)wh[Y])};
			if (pv) {
				// pivots를 가지고 원본 프레임 사이즈에 비례하게 크기 조절하는 벡터 추가
				vec2 pivv = vec2(0.5f) - pivots[i] / vec2(rects[i].value[WIDTH], rects[i].value[HEIGHT]);
				vec2 xy(rctz[i].value[WIDTH], rctz[i].value[HEIGHT]);
				xy *= vec2((float)wh[X], (float)wh[Y]) / 1024.0f;
				pivv *= xy;
				sctrz[i] = vec4(xy[X], xy[Y], pivv[X], pivv[Y]);
			}
		}
		// print(sctrz[0]);	// 버튼에 사용하기 위한 값을 알아낼 수 있습니다.
		struct anim2d :public UIAnimation {
			anim2d(bool _1, const std::vector<Keypoint<pTexture>>& _2, const std::vector<Keypoint<vec4>>& _3, const std::vector<vec4>& _4) :UIAnimation(_1, _2, _3, _4) {}
		};
		pAnimation ua = std::make_shared<anim2d>(loop, tx, rctz, sctrz);
		push(name, ua);
		return ua;
	}
}