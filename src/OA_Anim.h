/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

#include "oaglem.h"
#include "OA_Material.h"

#include <vector>
#include <map>
#include <set>
#include <string>

struct aiScene;
struct aiNode;

namespace onart {

	template <class T>
	struct Keypoint {
		float tp;
		T value;

		inline operator float() const { return tp; }

		/// <summary>
		/// 정렬을 위한 LESS 함수입니다. 편의를 위해 동봉했습니다.
		/// </summary>
		inline bool operator<(const Keypoint& k2) const {
			return tp < k2.tp;
		}

		/// <summary>
		/// 정렬을 위한 LESS 함수입니다. 편의를 위해 동봉했습니다.
		/// </summary>
		inline bool operator<(float k2) const {
			return tp < k2;
		}
	};

	class Entity;

	/// <summary>
	/// 개체의 시각적 상태를 나타내는 Animation 클래스입니다. 시각적 상태 이외의 부분(ex: 판정 변경)은 개체의 act()에서 정의하는 것이 매우 권장됩니다.
	/// 텍스처 및 직사각형 좌표를 넘기는 2d animation, 정점에 영향을 주는 골격의 변환 행렬을 넘기는 3d animation이 파생됩니다.
	/// </summary>
	class Animation
	{
	public:
		/// <summary>
		/// 애니메이션의 해당 시점의 프레임을 셰이더에 보내며 애니메이션 키포인트를 보냅니다. 2D 애니메이션은 bind와 draw가 호출되므로 모델 행렬은 이보다 전에 넘겨야 합니다.
		/// </summary>
		/// <param name="elapsed">최초 시작 후 경과 시간</param>
		/// <param name="e">이벤트 받을 개체</param>
		virtual void go(float elapsed, Entity* e, float dynamicTps = 1) = 0;
		/// <summary>
		/// 이름으로 애니메이션을 찾습니다. 없는 경우 nullptr를 리턴합니다.
		/// </summary>
		inline static Animation* get(const std::string& s) { auto i = animations.find(s); if (i != animations.end())return i->second; return nullptr; }
		/// <summary>
		/// 이름으로 애니메이션을 내립니다.
		/// </summary>
		/// <param name="s"></param>
		inline static void drop(const std::string& s) { if (animations.find(s) != animations.end()) { delete animations[s]; animations.erase(s); } }
	protected:
		Animation(bool loop, float duration, int staticTps = 1);
		inline float getTp(float elapsed) { if (duration <= 0)return 0; elapsed *= staticTps; return loop ? fmodf(elapsed, duration) : elapsed; }
		/// <summary>
		/// map에 애니메이션을 추가합니다.
		/// </summary>
		inline static void push(const std::string& s, Animation* a) { animations[s] = a; }
		inline ~Animation() { }
	private:
		float duration;
		int staticTps = 1;
		bool loop;
		static std::map<std::string, Animation*> animations;
	};

	/// <summary>
	/// 2D 개체의 컷 애니메이션입니다.
	/// 2D 개체의 시각적 키포인트는 2종류로, (텍스처,시점), (텍스처 내 직사각형과 피벗, 시점)입니다. (텍스처,시점)의 경우 개체로 보낼 키포인트 넘버와는 무관합니다.
	/// 즉 act()는 텍스처 내 직사각형 키포인트만을 기준으로 호출됩니다.
	/// <para>직사각형: LDWH(왼쪽/아래/가로/세로) 순서로 정의되는 vec4입니다. 원본 텍스처 이미지의 좌측 하단을 (0,0)으로 하는 픽셀 단위입니다.</para>
	/// 피벗이 설정된 경우 피벗을 기준으로 2D 텍스처가 그려지는 직사각형 크기가 해당 시점의 rect를 기준으로 자동 조절되고 피벗이 회전/크기 변환의 중심이 됩니다.
	/// 설정되지 않은 경우 직사각형 크기는 변하지 않으며 변환의 중심은 그대로 프레임 직사각형의 중심이 됩니다.
	/// 피벗은 애니메이션의 각 프레임의 크기(rects)가 같지 않게 그려진 경우에 명시하는 것이 좋습니다. 피벗은 텍스처 내 직사각형의 좌측 하단을 (0,0)으로 하는 픽셀 단위입니다.
	/// 
	/// 모든 애니메이션의 시작 시점은 반드시 0이어야 합니다.
	/// 
	/// 생성자에서 텍스처 입력은 가독성상 Material::genTextureFromMemory() 함수 이용이 권장됩니다.
	/// 
	/// </summary>
	class Animation2D: public Animation {
	public:
		/// <summary>
		/// 애니메이션을 생성하고 리턴합니다.
		/// </summary>
		/// <param name="name">애니메이션 이름입니다. 이름이 겹치는 경우 내용에 관계없이 생성하지 않고 기존에 있던 것을 리턴합니다.</param>
		/// <param name="loop">루프 여부를 선택합니다. false인 경우 애니메이션이 끝나면 마지막 상태를 유지합니다.</param>
		/// <param name="tex">시점과 텍스처의 순서쌍 집합입니다.</param>
		/// <param name="rects">시점과 직사각형 영역(LDWH. 좌/하/폭/높이, 단위는 px)의 순서쌍 집합입니다. 비어 있으면 안 됩니다.</param>
		/// <param name="pivots">rects에 일대일로 대응하는 피벗 좌표입니다. 좌측 하단을 0으로, 픽셀 단위로 입력하면 됩니다. 입력하지 않는 경우 애니메이션의 각 프레임은 단위 정사각형에 들어가며 정사각형의 중심이 곧 피벗이 됩니다.</param>
		static Animation* make(const std::string& name, bool loop, const std::vector<Keypoint<Texture>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots = {});
		void go(float elapsed, Entity* e, float dynamicTps = 1);
	private:
		Animation2D(bool loop, const std::vector<Keypoint<unsigned>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec4>& sctrs = {});
		std::vector<Keypoint<unsigned>> tex;
		std::vector<Keypoint<vec4>> rects;
		std::vector<vec4> sctrs;
		const bool hasTex, hasRect, hasPiv;
	};

	/// <summary>
	/// 3D 개체의 관절 애니메이션입니다. 현재 버전은 편의상 .dae 모델 파일을 디스크 혹은 메모리에서 로드하는 것만 허용합니다.
	/// 3D 개체의 시각적 키포인트는 최대 64개의 뼈의 위치, 회전, 크기로 구성됩니다.
	/// 뼈가 많은 만큼 몇 번째 프레임인지 판단하여 매번 호출하는 것은 불가능합니다. 애니메이션을 생성할 때 개체가 act()로 알림받을 시점(timepoint)을 직접 명시해 주세요.
	/// 3D 관절 애니메이션에서 "뼈"는 셰이더로 이름순으로 전달됩니다. 다른 모델에 동일한 애니메이션을 적용하고 싶은 경우
	/// 모델에서 정점-뼈 대응을 할 때 반드시 동일한 이름의 뼈를 사용하시기 바랍니다.
	/// </summary>
	class Animation3D: public Animation {
	public:
		/// <summary>
		/// .dae 파일에서 3D 관절 애니메이션을 로드합니다.
		/// </summary>
		/// <param name="name">애니메이션의 이름을 정해주세요. 중복인 경우 덮어쓰지 않고 기존의 것을 그대로 리턴합니다.</param>
		/// <param name="file">파일 이름을 입력해주세요.</param>
		/// <param name="loop">루프 여부를 선택하세요.</param>
		/// <param name="sig_kp">act()로 알림받을 시점(float)</param>
		static Animation* load(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp = {});
		/// <summary>
		/// .dae 파일 형식의 배열 변수에서 3D 관절 애니메이션을 로드합니다.
		/// </summary>
		/// <param name="name">애니메이션의 이름을 정해주세요. 중복인 경우 덮어쓰지 않고 기존의 것을 그대로 리턴합니다.</param>
		/// <param name="dat">변수 주소를 대입해 주세요.</param>
		/// <param name="len">배열 변수의 길이를 입력해 주세요.</param>
		/// <param name="loop">루프 여부를 선택하세요.</param>
		/// /// <param name="sig_kp">act()로 알림받을 시점(float)</param>
		static Animation* load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::vector<float>& sig_kp = {});

		/// <summary>
		/// 애니메이션의 현재 상태를 렌더링하고, 필요한 경우 개체에서 함수를 호출합니다.
		/// </summary>
		/// <param name="elapsed">애니메이션 시작 후 지난 시간</param>
		/// <param name="e">개체 포인터</param>
		/// <param name="dynamicTps">애니메이션 속도(기본값=1)</param>
		void go(float elapsed, Entity* e, float dynamicTps = 1);
	private:

		struct BoneAnim {
			std::vector<Keypoint<vec3>> keyPos;
			std::vector<Keypoint<Quaternion>> keyRot;
			std::vector<Keypoint<vec3>> keyScale;

			mat4 localTransform;
			void setTrans(float tp);
		};

		struct BoneTree {
			int id = -1;
			mat4 transformation;
			mat4 global;
			std::vector<BoneTree> children;
		};

		struct Bone
		{
			mat4 uni;
			const mat4 offset;
			inline Bone(const mat4& m) :offset(m) {}
		};

		void readHierarchy(aiNode* root, Animation3D::BoneTree& tree);

		Animation3D(const aiScene*, float duration, int tps, bool loop, const std::vector<float>& sig_kp);
		std::vector<float> sigKp;
		std::map<int, BoneAnim> keys;
		std::vector<Bone> u;
		std::map<std::string, int> n2i;
		BoneTree btree;
		mat4 globalInverse;
		void setGlobalTrans(BoneTree& t, const mat4& parent = mat4());
	};
}

#endif // !__OA_ANIM_H__
