#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

namespace onart {
	/// <summary>
	/// ��ü�� �ð��� ���¸� ��Ÿ���� Animation Ŭ�����Դϴ�.
	/// �ؽ�ó �� ���簢�� ��ǥ�� �ѱ�� 2d animation, ������ ������ �ִ� ����� ��ȯ ����� �ѱ�� 3d animation�� �Ļ��˴ϴ�.
	/// </summary>
	class Animation
	{
	public:

	private:
		float duration;
		int ticksPerSec;
		float dynamicTps = 1;
		bool loop;
	};
}

#endif // !__OA_ANIM_H__
