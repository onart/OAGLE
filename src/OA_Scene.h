/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_SCENE_H__
#define __OA_SCENE_H__

#include "OA_Input.h"

#include <string>
#include <vector>
#include <map>

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
		/// 응용 계층에서 사용할 일 없는 함수입니다.
		/// </summary>
		void update();
		/// <summary>
		/// 응용 계층에서 사용할 일 없는 함수입니다.
		/// </summary>
		void render();
		/// <summary>
		/// 씬에 개체를 추가합니다.
		/// </summary>
		void addEntity(Entity*);

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
		static struct __dropentity{
			friend class Entity;
		private:
			/// <summary>
			/// 이것은 씬에서 개체를 제외합니다. 개체의 메모리가 해제되지는 않으며, 개체가 소멸할 때는 이 함수도 자동으로 호출됩니다.
			/// </summary>
			static void dropEntity(Entity*);
		}__drop;
		inline virtual ~Scene() {};
	protected:
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
		// +다수의 광원
	private:	
		std::vector<Entity*> entities;
		std::multimap<float, size_t> renderOrder;
		std::vector<size_t> bubble;
		void reap();
		bool shouldReap = false;
		bool shouldOrder = false;
		enum class SceneDoing { UPDATE, RENDER, NOTHING } doing = SceneDoing::NOTHING;
		// +bgm
	};
}

#endif // !__OA_SCENE_H__
