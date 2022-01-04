#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

#include "oaglem.h"

#include <vector>
#include <map>
#include <string>

namespace onart {

	class Entity;

	/// <summary>
	/// 개체의 시각적 상태를 나타내는 Animation 클래스입니다. 시각적 상태 이외의 부분(ex: 판정 변경)은 개체의 act()에서 정의하는 것이 매우 권장됩니다.
	/// 텍스처 및 직사각형 좌표를 넘기는 2d animation, 정점에 영향을 주는 골격의 변환 행렬을 넘기는 3d animation이 파생됩니다.
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
	/// 2D 개체의 컷 애니메이션입니다.
	/// 2D 개체의 시각적 키포인트는 2종류로, (텍스처,시점), (텍스처 내 직사각형과 피벗, 시점)입니다. (텍스처,시점)의 경우 개체로 보낼 키포인트 넘버와는 무관합니다.
	/// 즉 act()는 텍스처 내 직사각형 키포인트만을 기준으로 호출됩니다.
	/// <para>직사각형: LDWH(왼쪽/아래/가로/세로) 순서로 정의되는 vec4입니다. 원본 텍스처 이미지의 좌측 하단이 (0,0), 우측 상단이 (1,1)입니다.</para>
	/// 피벗이 설정된 경우 피벗을 기준으로 2D 텍스처가 그려지는 직사각형 크기가 해당 시점의 rect를 기준으로 자동 조절되고 피벗이 회전/크기 변환의 중심이 됩니다. 이에 따라
	/// 별개의 이미지에서 하나의 애니메이션으로 끌어올 경우 이미지가 차지하는 비율을 같게 맞추거나 개체의 모델을 따로 조절할 필요가 있습니다.
	/// 설정되지 않은 경우 직사각형 크기는 변하지 않으며 변환의 중심은 그대로 프레임 직사각형의 중심이 됩니다.
	/// 피벗은 애니메이션의 각 프레임의 크기(rects)가 같지 않게 그려진 경우에 명시하는 것이 좋습니다. 피벗은 텍스처 내 직사각형의 좌측 하단을 (0,0), 우측 상단을 (1,1)으로 합니다.
	/// 
	/// 생성자에서 텍스처 입력은 가독성상 Material::genTextureFromMemory() 함수 이용이 권장됩니다.
	/// 
	/// </summary>
	class Animation2D: public Animation {
	public:
		/// <summary>
		/// 애니메이션을 생성합니다.
		/// </summary>
		/// <param name="loop">루프 여부를 선택합니다. false인 경우 애니메이션이 끝나면 마지막 상태를 유지합니다.</param>
		/// <param name="tex">시점과 텍스처의 순서쌍 집합입니다.</param>
		/// <param name="rects">시점과 직사각형 영역의 순서쌍 집합입니다. 비어 있으면 안 됩니다.</param>
		/// <param name="pivots">피벗과 직사각형 영역의 순서쌍 집합입니다.</param>
		Animation2D(bool loop, const std::map<float, unsigned>& tex, const std::map<float, vec4>& rects, const std::map<float, vec2>& pivots = {});
		void go(float elapsed, Entity* e);
	private:
		std::map<float, unsigned> tex;
		std::map<float, vec4> rects;
		std::map<float, vec2> pivots;
		bool hasPivot;
	};

	/// <summary>
	/// 3D 개체의 관절 애니메이션입니다.
	/// 3D 개체의 시각적 키포인트는 최대 64개의 뼈의 위치, 회전, 크기로 구성되며 act()는 위치, 회전, 크기 모든 키포인트에 대하여 한 번에 8비트씩 담아 호출됩니다.
	/// 0xff & kp는 회전, 0xff00 & kp는 위치, 0xff0000 & kp는 크기에 해당합니다.
	/// 3D 애니메이션은 연속적입니다. 
	/// </summary>
	class Animation3D: public Animation {
	public:
		Animation3D(std::string name);
		void go(float elapsed, Entity* e);
	};
}

#endif // !__OA_ANIM_H__
