#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

#include "oaglem.h"

#include <vector>
#include <map>
#include <set>
#include <string>

struct aiAnimation;

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
	/// <para>���簢��: LDWH(����/�Ʒ�/����/����) ������ ���ǵǴ� vec4�Դϴ�. ���� �ؽ�ó �̹����� ���� �ϴ��� (0,0), ���� ����� (1,1)�Դϴ�.</para>
	/// �ǹ��� ������ ��� �ǹ��� �������� 2D �ؽ�ó�� �׷����� ���簢�� ũ�Ⱑ �ش� ������ rect�� �������� �ڵ� �����ǰ� �ǹ��� ȸ��/ũ�� ��ȯ�� �߽��� �˴ϴ�. �̿� ����
	/// ������ �̹������� �ϳ��� �ִϸ��̼����� ����� ��� �̹����� �����ϴ� ������ ���� ���߰ų� ��ü�� ���� ���� ������ �ʿ䰡 �ֽ��ϴ�.
	/// �������� ���� ��� ���簢�� ũ��� ������ ������ ��ȯ�� �߽��� �״�� ������ ���簢���� �߽��� �˴ϴ�.
	/// �ǹ��� �ִϸ��̼��� �� �������� ũ��(rects)�� ���� �ʰ� �׷��� ��쿡 ����ϴ� ���� �����ϴ�. �ǹ��� �ؽ�ó �� ���簢���� ���� �ϴ��� (0,0), ���� ����� (1,1)���� �մϴ�.
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
		/// <param name="rects">������ ���簢�� ������ ������ �����Դϴ�. ��� ������ �� �˴ϴ�.</param>
		/// <param name="pivots">�ǹ��� ���簢�� ������ ������ �����Դϴ�.</param>
		static Animation* make(const std::string& name, bool loop, const std::vector<Keypoint<unsigned>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots = {});
		void go(float elapsed, Entity* e, float dynamicTps = 1);
	private:
		Animation2D(bool loop, const std::vector<Keypoint<unsigned>>& tex, const std::vector<Keypoint<vec4>>& rects, const std::vector<vec2>& pivots = {});
		std::vector<Keypoint<unsigned>> tex;
		std::vector<Keypoint<vec4>> rects;
		std::vector<vec2> pivots;
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
		static Animation* load(const std::string& name, const std::string& file, bool loop, const std::vector<float>& sig_kp);
		/// <summary>
		/// .dae ���� ������ �迭 �������� 3D ���� �ִϸ��̼��� �ε��մϴ�.
		/// </summary>
		/// <param name="name">�ִϸ��̼��� �̸��� �����ּ���. �ߺ��� ��� ����� �ʰ� ������ ���� �״�� �����մϴ�.</param>
		/// <param name="dat">���� �ּҸ� ������ �ּ���.</param>
		/// <param name="len">�迭 ������ ���̸� �Է��� �ּ���.</param>
		/// <param name="loop">���� ���θ� �����ϼ���.</param>
		/// /// <param name="sig_kp">act()�� �˸����� ����(float)</param>
		static Animation* load(const std::string& name, const unsigned char* dat, size_t len, bool loop, const std::vector<float>& sig_kp);
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
			std::string name;
			mat4 transformation;
			mat4 global;
			std::vector<BoneTree> children;
		};

		Animation3D(aiAnimation*, float duration, int tps, bool loop, const std::vector<float>& sig_kp);
		std::vector<float> sigKp;
		std::map<std::string, BoneAnim> keys;
		std::vector<mat4> u;
		BoneTree btree;

		void setGlobalTrans(BoneTree& t, const mat4& parent = mat4());
	};
}

#endif // !__OA_ANIM_H__
