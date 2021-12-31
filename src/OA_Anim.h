#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

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
	private:
		float duration;
		int staticTps = 1;
		float dynamicTps = 1;
		bool loop;
		static std::map<std::string, Animation*> animations;
	};

	/// <summary>
	/// 2D ��ü�� �� �ִϸ��̼��Դϴ�.
	/// 2D ��ü�� �ð��� Ű����Ʈ�� 2������, (�ؽ�ó,�ð�), (�ؽ�ó �� ���簢���� �ǹ�, �ð�)�Դϴ�. (�ؽ�ó,�ð�)�� ��� ��ü�� ���� Ű����Ʈ �ѹ��ʹ� �����մϴ�.
	/// �ǹ��� ������ ��� �ǹ��� �������� 2D �ؽ�ó�� �׷����� ���簢�� ũ�Ⱑ �ڵ����� �����˴ϴ�. �������� ���� ��� 2D �ؽ�ó�� �׷����� ���簢�� ũ��� ������ �ʽ��ϴ�.
	/// �� act()�� �ؽ�ó �� ���簢�� Ű����Ʈ���� �������� ȣ��˴ϴ�.
	/// <para>���簢��: LDWH(����/�Ʒ�/����/����) ������ ���ǵǴ� vec4�Դϴ�. ���� �ϴ��� (0,0), ���� ����� (1,1)�Դϴ�.</para>
	/// �����ڿ��� �ؽ�ó �Է��� �������� Material::genTextureFromMemory() �Լ� �̿��� ����˴ϴ�.
	/// 
	/// </summary>
	class Animation2D: public Animation {
	public:
		Animation2D(std::string name);
		void go(float elapsed, Entity* e);
	private:
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
