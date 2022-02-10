#ifndef __OA_SCENE_H__
#define __OA_SCENE_H__

#include "OA_Input.h"

#include <string>
#include <vector>
#include <set>

namespace onart {
	class Camera;
}

extern onart::Camera mainCamera;

namespace onart {

	class Entity;
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY 매크로를 정의한 경우 정수 키를 사용할 수 있습니다.
	using EntityKey = int;
#else
	using EntityKey = std::string;
#endif
	/// <summary>
	/// 게임에 표시되는 씬입니다.
	/// </summary>
	class Scene
	{
	public:
		/// <summary>
		/// 현재 열려 있는 씬입니다. 게임에는 한 번에 하나의 씬만 열려 있을 수 있습니다.
		/// </summary>
		static Scene* currentScene;
		/// <summary>
		/// pause()를 해도 멈추지 않게 하고 싶은 개체의 키를 등록합니다. 중복 개체가 다수 존재하는 키를 등록한다면, 그에 대한 모든 개체가 pause()로는 멈추지 않습니다.
		/// 또한 등록한 개체는 씬이 전환되어도 유지됩니다. 씬 자체는 등록할 수 없습니다. 일시정지 시 그런 부분을 관리할 일은 거의 없겠으나, 등록된 개체는 key 값의 오름차순 순서대로 업데이트됩니다.
		/// </summary>
		inline static void noStop(const EntityKey& k) { nonstop.insert(k); }
		/// <summary>
		/// 멈추지 않도록 등록했던 키를 등록 해제합니다.
		/// </summary>
		inline static void yesStop(const EntityKey& k) { nonstop.erase(k); }
		/// <summary>
		/// 응용 계층에서 사용할 일 없는 함수입니다.
		/// </summary>
		void update();
		/// <summary>
		/// 응용 계층에서 사용할 일 없는 함수입니다.
		/// </summary>
		void render();
		inline int getSceneId() { return id; }
		/// <summary>
		/// 씬에서 매 프레임마다 호출됩니다. 항상 씬 내 개체의 Update()보다 먼저 실행됩니다.
		/// </summary>
		virtual void Update() = 0;
		/// <summary>
		/// 씬 포인터를 통해 이벤트를 발동할 수 있습니다.
		/// </summary>
		/// <param name="idx">각 씬마다 배정된 이벤트 번호</param>
		virtual void actEvent(int idx);
		/// <summary>
		/// 씬을 초기화합니다. 주로 새 개체를 생성합니다.
		/// </summary>
		virtual void init() = 0;
		/// <summary>
		/// 카메라의 이동을 제한합니다. 카메라의 이전 프레임 위치와 현재 프레임에 새로 자리하려는 위치를 입력받아 카메라가 실제로 있어야 할 위치를 리턴합니다.
		/// 오버라이딩하지 않는 경우, 무제한으로 간주됩니다.
		/// </summary>
		/// <param name="currentCameraPos">이전 프레임의 카메라 위치</param>
		/// <param name="desiredCameraPos">현재 프레임에 카메라가 자리하려는 위치</param>
		/// <returns>현재 씬에서의 제한이 적용된 이후 현재 프레임에 카메라가 자리하는 위치</returns>
		virtual vec3 constrainCamera(const vec3& currentCameraPos, const vec3& desiredCameraPos);

		virtual ~Scene() {};
	protected:
		std::vector<Entity*> entities;
		int id;
		/// <summary>
		/// 게임 실행 이후 흐른 시간(초)입니다.
		/// </summary>
		static const float& tp;
		/// <summary>
		/// 이전 프레임과 현재 프레임 간의 시간 간격(초)입니다.
		/// </summary>
		static const float& dt;
		/// <summary>
		/// 다른 씬으로 전환합니다.
		/// </summary>
		void change(Scene* other);
		/// <summary>
		/// 씬 및 모든 개체의 업데이트를 중단합니다. 각 개체의 isActive를 변경시키지 않고 독립적으로 동작합니다. 렌더링은 중단하지 않습니다.
		/// 중단되지 않는 예외 타겟은 noStop(), yesStop()를 통해 등록 및 취소할 수 있습니다.
		/// </summary>
		inline void pause() { isPaused = true; }
		/// <summary>
		/// pause를 취소합니다.
		/// </summary>
		inline void resume() { isPaused = false; }
		// +다수의 광원
	private:
		bool isPaused = false;
		static std::set<EntityKey> nonstop;
		// +bgm
	};
}

#endif // !__OA_SCENE_H__
