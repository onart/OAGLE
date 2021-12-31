#ifndef __OA_ANIM_H__
#define	__OA_ANIM_H__

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
	private:
		float duration;
		int staticTps = 1;
		float dynamicTps = 1;
		bool loop;
		static std::map<std::string, Animation*> animations;
	};

	/// <summary>
	/// 2D 개체의 컷 애니메이션입니다.
	/// 2D 개체의 시각적 키포인트는 2종류로, (텍스처,시각), (텍스처 내 직사각형과 피벗, 시각)입니다. (텍스처,시각)의 경우 개체로 보낼 키포인트 넘버와는 무관합니다.
	/// 피벗이 설정된 경우 피벗을 기준으로 2D 텍스처가 그려지는 직사각형 크기가 자동으로 조절됩니다. 설정되지 않은 경우 2D 텍스처가 그려지는 직사각형 크기는 변하지 않습니다.
	/// 즉 act()는 텍스처 내 직사각형 키포인트만을 기준으로 호출됩니다.
	/// <para>직사각형: LDWH(왼쪽/아래/가로/세로) 순서로 정의되는 vec4입니다. 좌측 하단이 (0,0), 우측 상단이 (1,1)입니다.</para>
	/// 생성자에서 텍스처 입력은 가독성상 Material::genTextureFromMemory() 함수 이용이 권장됩니다.
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
