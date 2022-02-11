#ifndef __OA_ENTITY_H__
#define __OA_ENTITY_H__

#include <vector>
#include <string>
#include <map>

#include "OA_Transform.h"
#include "OA_Input.h"

namespace onart {
	class Model;
	class Animation;
	/// <summary>
	/// 게임 속 세상에 존재하는 개체들입니다.
	/// </summary>
	class Entity
	{
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY 매크로를 정의한 경우 정수 키를 사용할 수 있습니다.
		using EntityKey = int;
#else
		using EntityKey = std::string;
#endif
	public:
		Entity(const EntityKey& k, const Transform& transform);
		~Entity();
		/// <summary>
		/// 위치를 얻습니다. 카메라 등과 같이 월드 좌표를 지속적으로 추적하는 것을 위해 만들어졌습니다.
		/// </summary>
		inline const vec3& getPos() { return transform.getPosition(); }
		/// <summary>
		/// 개체가 active하다는 것은 update()가 매 프레임 호출된다는 의미입니다.
		/// </summary>
		bool isActive = true;
		/// <summary>
		/// true인 경우에만 개체가 화면에 렌더링됩니다.
		/// </summary>
		bool isRendered = true;
		/// <summary>
		/// 응용 단계에서 사용하지 않는 것이 좋습니다.
		/// </summary>
		void act(int kp);
		/// <summary>
		/// 애니메이션의 특정 키포인트를 지났을 때 개체가 취할 행동입니다.
		/// 이벤트를 전달하는 용도로 사용할 수도 있으나 키포인트 번호와 중복되지 않도록 응용 단계에서 통일하는 편이 추천됩니다.
		/// </summary>
		/// <param name="kp">이벤트 플래그/키포인트</param>
		virtual void Act(int kp);
		/// <summary>
		/// 기본적으로 응용 계층에서 접근할 일 없는 함수입니다. 꼭 필요한 경우가 아니라면 오버라이딩 및 호출하지 않는 것이 좋습니다.
		/// </summary>
		virtual void render();
		/// <summary>
		/// 기본적으로 응용 계층에서 접근할 일 없는 함수입니다. 꼭 필요한 경우가 아니라면 오버라이딩 및 호출하지 않는 것이 좋습니다.
		/// </summary>
		virtual void update();
		/// <summary>
		/// Update() 함수를 오버라이딩하여 개체가 프레임마다 취하는 행동을 정의합니다.
		/// </summary>
		virtual void Update();
		/// <summary>
		/// 현재 애니메이션 키를 읽습니다. 응용 단계에서 사용할 일 없는 함수입니다.
		/// </summary>
		inline int getAnimKey() { return animKp; }
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가진 것들을 찾습니다. 없는 경우 빈 벡터를 반환합니다.
		/// </summary>
		static std::vector<Entity*> gets(const EntityKey& k);
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가진 것 중 가장 앞의 하나를 찾습니다. 없는 경우 nullptr를 반환합니다. 이 키에 대하여 하나의 개체만 있을 것이 확실한 경우 사용하기에 좋습니다.
		/// </summary>
		static Entity* get(const EntityKey& k);

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
		/// 개체의 현재 애니메이션 상태입니다.
		/// </summary>
		const int& animState;
		inline void setModel(Model* model) { this->model = model; }
		/// <summary>
		/// 애니메이션 상태를 변경합니다. 현재 적용 중인 번호를 고르면 그 애니메이션은 처음부터 다시 시작합니다.
		/// </summary>
		inline void setAnim(int idx, float dynamicTps = 1) { as = idx; animKp = 0; animStartTimepoint = tp; animTps = dynamicTps; }
		/// <summary>
		/// 애니메이션을 저장 배열의 끝에 추가합니다. 첫 애니메이션을 추가할 때 animState는 0이 됩니다.
		/// </summary>
		void addAnim(const std::string& name);
		/// <summary>
		/// 게임이 실행된 후 현재 프레임에 들어오기까지 흐른 시간(초)입니다.
		/// </summary>
		static const float& tp;
		/// <summary>
		/// 이전 프레임과 현재 프레임 간의 시간 간격(초)입니다.
		/// </summary>
		static const float& dt;
	private:
		float lt = 0;
		EntityKey key;
		Model* model = nullptr;
		int as = -1;
		float animStartTimepoint;
		float animTps = 1;
		int animKp = 0;
		std::vector<Animation*> anims;
		static std::multimap<EntityKey, Entity*> entities;
	};
}


#endif // !__OA_ENTITY_H__



