#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

namespace onart {
	/// <summary>
	/// 개체의 시각적 상태를 나타내는 Animation 클래스입니다.
	/// 텍스처 및 직사각형 좌표를 넘기는 2d animation, 정점에 영향을 주는 골격의 변환 행렬을 넘기는 3d animation이 파생됩니다.
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
