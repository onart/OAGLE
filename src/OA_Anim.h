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
		/// ������ ���� LESS �Լ��Դϴ�. ���Ǹ� ���� �����߽��ϴ�.
		/// </summary>
		inline bool operator<(const Keypoint& k2) const {
			return tp < k2.tp;
		}

		/// <summary>
		/// ������ ���� LESS �Լ��Դϴ�. ���Ǹ� ���� �����߽��ϴ�.
		/// </summary>
		inline bool operator<(float k2) const {
			return tp < k2;
		}
	};

	class Entity;

	/// <summary>
	/// ��ü�� �ð��� ���¸� ��Ÿ���� Animation Ŭ�����Դϴ�. �ð��� ���� �̿��� �κ�(ex: ���� ����)�� ��ü�� act()���� �����ϴ� ���� �ſ� ����˴ϴ�.
	/// �ؽ�ó �� ���簢�� ��ǥ�� �ѱ�� 2d animation, ������ ������ �ִ� ����� ��ȯ ����� �ѱ�� 3d animation�� �Ļ��˴ϴ�.
	/// </summary>
	class Animation
	{
	public:
		/// <summary>
		/// �ִϸ��̼��� �ش� ������ �������� ���̴��� ������ �ִϸ��̼� Ű����Ʈ�� �����ϴ�. 2D �ִϸ��̼��� bind�� draw�� ȣ��ǹǷ� �� ����� �̺��� ���� �Ѱܾ� �մϴ�.
		/// </summary>
		/// <param name="elapsed">���� ���� �� ��� �ð�</param>
		/// <param name="e">�̺�Ʈ ���� ��ü</param>
		virtual void go(float elapsed, Entity* e, float dynamicTps = 1) = 0;
		/// <summary>
		/// �̸����� �ִϸ��̼��� ã���ϴ�. ���� ��� nullptr�� �����մϴ�.
		/// </summary>
		inline static Animation* get(const std::string& s) { auto i = animations.find(s); if (i != animations.end())return i->second; return nullptr; }
		/// <summary>
		/// �̸����� �ִϸ��̼��� �����ϴ�.
		/// </summary>
		/// <param name="s"></param>
		inline static void drop(const std::string& s) { if (animations.find(s) != animations.end()) { delete animations[s]; animations.erase(s); } }
	protected:
		Animation(bool loop, float duration, int staticTps = 1);
		inline float getTp(float elapsed) { if (duration <= 0)return 0; elapsed *= staticTps; return loop ? fmodf(elapsed, duration) : elapsed; }
		/// <summary>
		/// map�� �ִϸ��̼��� �߰��մϴ�.
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
	/// 2D ��ü�� �� �ִϸ��̼��Դϴ�.
	/// 2D ��ü�� �ð��� Ű����Ʈ�� 2������, (�ؽ�ó,����), (�ؽ�ó �� ���簢���� �ǹ�, ����)�Դϴ�. (�ؽ�ó,����)�� ��� ��ü�� ���� Ű����Ʈ �ѹ��ʹ� �����մϴ�.
	/// �� act()�� �ؽ�ó �� ���簢�� Ű����Ʈ���� �������� ȣ��˴ϴ�.
	/// <para>���簢��: LDWH(����/�Ʒ�/����/����) ������ ���ǵǴ� vec4�Դϴ�. ���� �ؽ�ó �̹����� ���� �ϴ��� (0,0)���� �ϴ� �ȼ� �����Դϴ�.</para>
	/// �ǹ��� ������ ��� �ǹ��� �������� 2D �ؽ�ó�� �׷����� ���簢�� ũ�Ⱑ �ش� ������ rect�� �������� �ڵ� �����ǰ� �ǹ��� ȸ��/ũ�� ��ȯ�� �߽��� �˴ϴ�.
	/// �������� ���� ��� ���簢�� ũ��� ������ ������ ��ȯ�� �߽��� �״�� ������ ���簢���� �߽��� �˴ϴ�.
	/// �ǹ��� �ִϸ��̼��� �� �������� ũ��(rects)�� ���� �ʰ� �׷��� ��쿡 ����ϴ� ���� �����ϴ�. �ǹ��� �ؽ�ó �� ���簢���� ���� �ϴ��� (0,0)���� �ϴ� �ȼ� �����Դϴ�.
	/// 
	/// ��� �ִϸ��̼��� ���� ������ �ݵ�� 0�̾�� �մϴ�.
	/// 
	/// �����ڿ��� �ؽ�ó �Է��� �������� Material::genTextureFromMemory() �Լ� �̿��� ����˴ϴ�.
	/// 
	/// </summary>
	class Animation2D: public Animation {
	public:
		/// <summary>
		/// �ִϸ��̼��� �����ϰ� �����մϴ�.
		/// </summary>
		/// <param name="name">�ִϸ��̼� �̸��Դϴ�. �̸��� ��ġ�� ��� ���뿡 ������� �������� �ʰ� ������ �ִ� ���� �����մϴ�.</param>
		/// <param name="loop">���� ���θ� �����մϴ�. false�� ��� �ִϸ��̼��� ������ ������ ���¸� �����մϴ�.</param>
		/// <param name="tex">������ �ؽ�ó�� ������ �����Դϴ�.</param>
		/// <param name="rects">������ ���簢�� ����(LDWH. ��/��/��/����, ������ px)�� ������ �����Դϴ�. ��� ������ �� �˴ϴ�.</param>
		/// <param name="pivots">rects�� �ϴ��Ϸ� �����ϴ� �ǹ� ��ǥ�Դϴ�. ���� �ϴ��� 0����, �ȼ� ������ �Է��ϸ� �˴ϴ�. �Է����� �ʴ� ��� �ִϸ��̼��� �� �������� ���� ���簢���� ���� ���簢���� �߽��� �� �ǹ��� �˴ϴ�.</param>
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
	/// 3D ��ü�� ���� �ִϸ��̼��Դϴ�. ���� ������ ���ǻ� .dae �� ������ ��ũ Ȥ�� �޸𸮿��� �ε��ϴ� �͸� ����մϴ�.
	/// 3D ��ü�� �ð��� Ű����Ʈ�� �ִ� 64���� ���� ��ġ, ȸ��, ũ��� �����˴ϴ�.
	/// ���� ���� ��ŭ �� ��° ���������� �Ǵ��Ͽ� �Ź� ȣ���ϴ� ���� �Ұ����մϴ�. �ִϸ��̼��� ������ �� ��ü�� act()�� �˸����� ����(timepoint)�� ���� ����� �ּ���.
	/// 3D ���� �ִϸ��̼ǿ��� "��"�� ���̴��� �̸������� ���޵˴ϴ�. �ٸ� �𵨿� ������ �ִϸ��̼��� �����ϰ� ���� ���
	/// �𵨿��� ����-�� ������ �� �� �ݵ�� ������ �̸��� ���� ����Ͻñ� �ٶ��ϴ�.
	/// </summary>
	class Animation3D: public Animation {
	public:
		/// <summary>
		/// .dae ���Ͽ��� 3D ���� �ִϸ��̼��� �ε��մϴ�.
		/// </summary>
		/// <param name="name">�ִϸ��̼��� �̸��� �����ּ���. �ߺ��� ��� ����� �ʰ� ������ ���� �״�� �����մϴ�.</param>
		/// <param name="file">���� �̸��� �Է����ּ���.</param>
		/// <param name="loop">���� ���θ� �����ϼ���.</param>
		/// <param name="sig_kp">act()�� �˸����� ����(float)</param>
		static Animation* load(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp = {});
		/// <summary>
		/// .dae ���� ������ �迭 �������� 3D ���� �ִϸ��̼��� �ε��մϴ�.
		/// </summary>
		/// <param name="name">�ִϸ��̼��� �̸��� �����ּ���. �ߺ��� ��� ����� �ʰ� ������ ���� �״�� �����մϴ�.</param>
		/// <param name="dat">���� �ּҸ� ������ �ּ���.</param>
		/// <param name="len">�迭 ������ ���̸� �Է��� �ּ���.</param>
		/// <param name="loop">���� ���θ� �����ϼ���.</param>
		/// /// <param name="sig_kp">act()�� �˸����� ����(float)</param>
		static Animation* load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::vector<float>& sig_kp = {});

		/// <summary>
		/// �ִϸ��̼��� ���� ���¸� �������ϰ�, �ʿ��� ��� ��ü���� �Լ��� ȣ���մϴ�.
		/// </summary>
		/// <param name="elapsed">�ִϸ��̼� ���� �� ���� �ð�</param>
		/// <param name="e">��ü ������</param>
		/// <param name="dynamicTps">�ִϸ��̼� �ӵ�(�⺻��=1)</param>
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
