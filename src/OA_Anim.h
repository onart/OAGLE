#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

#include "oaglem.h"

#include <vector>
#include <map>
#include <string>

namespace onart {

	class Entity;

	/// <summary>
	/// ��ü�� �ð��� ���¸� ��Ÿ���� Animation Ŭ�����Դϴ�. �ð��� ���� �̿��� �κ�(ex: ���� ����)�� ��ü�� act()���� �����ϴ� ���� �ſ� ����˴ϴ�.
	/// �ؽ�ó �� ���簢�� ��ǥ�� �ѱ�� 2d animation, ������ ������ �ִ� ����� ��ȯ ����� �ѱ�� 3d animation�� �Ļ��˴ϴ�.
	/// </summary>
	class Animation
	{
	public:
		virtual void go(float elapsed, Entity* e) = 0;
	protected:
		Animation(bool loop, float duration, int staticTps = 1, float dynamicTps = 1);
		inline float getTp(float elapsed) { if (duration == 0)return 0; return loop ? fmodf(elapsed, duration) : elapsed; }
	private:
		float duration;
		int staticTps = 1;
		float dynamicTps = 1;
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
	/// �����ڿ��� �ؽ�ó �Է��� �������� Material::genTextureFromMemory() �Լ� �̿��� ����˴ϴ�.
	/// 
	/// </summary>
	class Animation2D: public Animation {
	public:
		/// <summary>
		/// �ִϸ��̼��� �����մϴ�.
		/// </summary>
		/// <param name="loop">���� ���θ� �����մϴ�. false�� ��� �ִϸ��̼��� ������ ������ ���¸� �����մϴ�.</param>
		/// <param name="tex">������ �ؽ�ó�� ������ �����Դϴ�.</param>
		/// <param name="rects">������ ���簢�� ������ ������ �����Դϴ�. ��� ������ �� �˴ϴ�.</param>
		/// <param name="pivots">�ǹ��� ���簢�� ������ ������ �����Դϴ�.</param>
		Animation2D(bool loop, const std::map<float, unsigned>& tex, const std::map<float, vec4>& rects, const std::map<float, vec2>& pivots = {});
		void go(float elapsed, Entity* e);
	private:
		std::map<float, unsigned> tex;
		std::map<float, vec4> rects;
		std::map<float, vec2> pivots;
		bool hasPivot;
	};

	/// <summary>
	/// 3D ��ü�� ���� �ִϸ��̼��Դϴ�.
	/// 3D ��ü�� �ð��� Ű����Ʈ�� �ִ� 64���� ���� ��ġ, ȸ��, ũ��� �����Ǹ� act()�� ��ġ, ȸ��, ũ�� ��� Ű����Ʈ�� ���Ͽ� �� ���� 8��Ʈ�� ��� ȣ��˴ϴ�.
	/// 0xff & kp�� ȸ��, 0xff00 & kp�� ��ġ, 0xff0000 & kp�� ũ�⿡ �ش��մϴ�.
	/// 3D �ִϸ��̼��� �������Դϴ�. 
	/// </summary>
	class Animation3D: public Animation {
	public:
		Animation3D(std::string name);
		void go(float elapsed, Entity* e);
	};
}

#endif // !__OA_ANIM_H__
