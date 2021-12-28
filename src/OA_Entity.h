#ifndef __OA_ENTITY_H__
#define __OA_ENTITY_H__

#include <string>
#include "OA_Transform.h"

namespace onart {
	/// <summary>
	/// 게임 속 세상에 존재하는 개체들입니다.
	/// </summary>
	class Entity
	{
	public:
		/// <summary>
		/// 개체가 active하다는 것은 update()가 매 프레임 호출된다는 의미입니다.
		/// </summary>
		bool isActive = true;
	protected:
		/// <summary>
		/// 개체의 위치, 크기, 회전을 나타냅니다.
		/// </summary>
		Transform transform;
		/// <summary>
		/// 개체의 시각(timepoint)입니다. 개체가 Active한 경우에만 증가합니다.
		/// </summary>
		const float& localTp;
		/// <summary>
		/// 게임이 실행된 후 현재 프레임에 들어오기까지 흐른 시간(초)입니다.
		/// </summary>
		static const float& tp;
	private:
		float lt = 0;
	};
}


#endif // !__OA_ENTITY_H__



