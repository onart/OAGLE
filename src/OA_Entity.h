/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_ENTITY_H__
#define __OA_ENTITY_H__

#include <vector>
#include <string>
#include <map>

#include "OA_Transform.h"
#include "OA_Game.h"
#include "OA_Anim.h"

namespace onart {
	class Camera;
}

namespace onart {
	class Model;
	class Scene;
	/// <summary>
	/// 게임 속 세상에 존재하는 개체들입니다.
	/// </summary>
	class Entity
	{
	public:
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY 매크로를 정의한 경우 정수 키를 사용할 수 있습니다.
		using EntityKey = int;
#else
		using EntityKey = std::string;	// const char*로 변경 예정
#endif
	private:
		float lt = 0;
		EntityKey key;
		std::shared_ptr<Model> model;
		int as = -1;
		float animStartTimepoint;
		float animTps = 1;
		int animKp = 0;
		std::vector<pAnimation> anims;
		bool isFixed;
		bool responseContinuously;
		char eclass;
		static std::multimap<EntityKey, Entity*> entities;
	public:
		/// <summary>
		/// 개체를 생성합니다. 특별히 매 프레임 업데이트할 내용이 없는 개체는 상속 없이 그대로 사용할 수 있습니다.
		/// <para>스프라이트 혹은 모델: addAnim(), setModel()로 정합니다.</para>
		/// <para>상속할 때: isFixed=true인 경우 카메라의 영향을 받지 않습니다. (이는 [-1,1]^3에 들어갈 정도로 축척을 맞춰야 한다는 의미입니다)</para>
		/// </summary>
		/// <param name="k">개체의 이름(혹은 id)입니다. 기본적으로 std::string 타입이지만 OA_USE_INT_AS_KEY 매크로 사용 시 int로 사용할 수 있습니다.</param>
		/// <param name="transform">개체의 위치, 크기, 회전을 나타냅니다.</param>
		/// <param name="isFixed">카메라의 이동에 영향을 받는 여부를 설정합니다. false가 영향을 안 받는다는 뜻으로, 2D, 3D 모두 가능합니다.</param>
		/// <param name="responseContinuously">true인 경우 애니메이션 진행 상태에 대하여 연속적으로 반응합니다.</param>
		/// <param name="isTranslucent">모델 및 애니메이션 중 하나라도 투명성이 조금이라도 있는 경우 true로 설정합니다.</param>
		Entity(const EntityKey& k, const Transform& transform, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = false);
		/// <summary>
		/// 개체를 생성합니다. 특별히 매 프레임 업데이트할 내용이 없는 개체는 상속 없이 그대로 사용할 수 있습니다.
		/// <para>스프라이트 혹은 모델: addAnim(), setModel()로 정합니다.</para>
		/// <para>상속할 때: isFixed=true인 경우 카메라의 영향을 받지 않습니다. (이는 [-1,1]^3에 들어갈 정도로 축척을 맞춰야 한다는 의미입니다)</para>
		/// </summary>
		/// <param name="k">개체의 이름(혹은 id)입니다. 기본적으로 std::string 타입이지만 OA_USE_INT_AS_KEY 매크로 사용 시 int로 사용할 수 있습니다.</param>
		/// <param name="anim0">기본 외형을 정합니다. 형태는 있으나 업데이트 함수는 필요 없을 때 유용합니다.</param>
		/// <param name="transform">개체의 위치, 크기, 회전을 나타냅니다.</param>
		/// <param name="isFixed">카메라의 이동에 영향을 받는 여부를 설정합니다. false가 영향을 안 받는다는 뜻으로, 2D, 3D 모두 가능합니다.</param>
		/// <param name="responseContinuously">true인 경우 애니메이션 진행 상태에 대하여 연속적으로 반응합니다.</param>
		/// <param name="isTranslucent">애니메이션에 투명성이 조금이라도 있는 경우 true로 설정합니다.</param>
		Entity(const EntityKey& k, const Transform& transform, pAnimation& anim0, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = true);
		/// <summary>
		/// 개체를 생성합니다. 특별히 매 프레임 업데이트할 내용이 없는 개체는 상속 없이 그대로 사용할 수 있습니다.
		/// <para>스프라이트 혹은 모델: addAnim(), setModel()로 정합니다.</para>
		/// <para>상속할 때: isFixed=true인 경우 카메라의 영향을 받지 않습니다. (이는 [-1,1]^3에 들어갈 정도로 축척을 맞춰야 한다는 의미입니다)</para>
		/// </summary>
		/// <param name="k">개체의 이름(혹은 id)입니다. 기본적으로 std::string 타입이지만 OA_USE_INT_AS_KEY 매크로 사용 시 int로 사용할 수 있습니다.</param>
		/// <param name="transform">개체의 위치, 크기, 회전을 나타냅니다.</param>
		/// <param name="model">기본 외형을 정합니다. 형태는 있으나 업데이트 함수는 필요 없을 때 유용합니다.</param>
		/// <param name="isFixed">카메라의 이동에 영향을 받는 여부를 설정합니다. false가 영향을 안 받는다는 뜻으로, 2D, 3D 모두 가능합니다.</param>
		/// <param name="responseContinuously">true인 경우 애니메이션 진행 상태에 대하여 연속적으로 반응합니다.</param>
		/// <param name="isTranslucent">모델 텍스처에 투명성이 조금이라도 있는 경우 true로 설정합니다.</param>
		Entity(const EntityKey& k, const Transform& transform, std::shared_ptr<Model>& model, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = false);
		Entity(const Entity&) = delete;
		/// <summary>
		/// 위치를 얻습니다. 카메라 등과 같이 월드 좌표를 지속적으로 추적하는 것을 위해 만들어졌습니다.
		/// </summary>
		inline const vec3& getPos() { return transform.getGlobalPosition(); }
		/// <summary>
		/// 개체가 active하다는 것은 update()가 매 프레임 호출된다는 의미입니다.
		/// </summary>
		bool isActive = true;
		/// <summary>
		/// true인 경우에만 개체가 화면에 렌더링됩니다.
		/// </summary>
		bool isRendered = true;
		/// <summary>
		/// true인 경우 씬 변경 시 개체가 파괴되지 않습니다. 이 값은 언제든 변경할 수 있습니다.
		/// </summary>
		bool preserveOnSceneChange = false;
		/// <summary>
		/// 개체에 투명도가 조금이라도 있는 경우 true로 설정해 주세요. 2D 개체의 경우 완전한 직사각형 이미지가 아니면 모두 true여야 합니다.
		/// 앞뒤 순서 변경이 없는 2D 게임(ex: 90도 측면 시점, 90도 조감 시점)에서는 거의 모든 개체에 투명도가 존재하기 때문에 설정할 필요 없습니다. 이때는 OAGLE_2DGAME 매크로를 이 파일 위에 정의해 주세요. 게임 성능이 다소 좋아지며 이 상수에 신경 쓸 필요가 없어집니다.
		/// 2D 게임이라도 포켓몬과 같이 비스듬한 조감의 경우 사물의 전후 관계가 변하기 때문에 OAGLE_2DGAME 매크로를 사용하지 않아야 합니다. (사용할 경우 처음에 앞에 있던 개체가 뒤로 갈 경우 직사각형 영역만큼 가려짐)
		/// </summary>
		const bool isTranslucent;
		/// <summary>
		/// 개체의 키를 리턴합니다.
		/// </summary>
		/// <returns></returns>
		inline const EntityKey& name() const { return key; }
		/// <summary>
		/// 개체의 변환 상태를 나타냅니다.
		/// </summary>
		inline Transform* getTransform() { return &transform; }
		/// <summary>
		/// 카메라 기준 상대적 z좌표를 리턴합니다.
		/// </summary>
		float zIndex();
		/// <summary>
		/// 응용 단계에서 사용하지 않는 것이 좋습니다.
		/// </summary>
		void act(int kp, float progress = 0);
		/// <summary>
		/// 애니메이션의 특정 키포인트를 지났을 때 개체가 취할 행동입니다.
		/// 이벤트를 전달하는 용도로 사용할 수도 있으나 키포인트 번호와 중복되지 않도록 응용 단계에서 통일하는 편이 추천됩니다.
		/// </summary>
		/// <param name="kp">이벤트 플래그/키포인트</param>
		virtual void Act(int kp, float progress = 0);
		/// <summary>
		/// 기본적으로 응용 계층에서 접근할 일 없는 함수입니다. 꼭 필요한 경우가 아니라면 오버라이딩 및 호출하지 않는 것이 좋습니다.
		/// </summary>
		virtual void render();
		/// <summary>
		/// 기본적으로 응용 계층에서 접근할 일 없는 함수입니다. 꼭 필요한 경우가 아니라면 오버라이딩 및 호출하지 않는 것이 좋습니다.
		/// </summary>
		virtual void renderShadowMap();
		/// <summary>
		/// 기본적으로 응용 계층에서 접근할 일 없는 함수입니다. 꼭 필요한 경우가 아니라면 오버라이딩 및 호출하지 않는 것이 좋습니다.
		/// </summary>
		virtual void update();
		/// <summary>
		/// Update() 함수를 오버라이딩하여 개체가 프레임마다 취하는 행동을 정의합니다.
		/// </summary>
		virtual void Update();
		/// <summary>
		/// 다른 개체와의 충돌 혹은 겹칩 판정 시 프레임마다 호출됩니다.
		/// </summary>
		virtual void onTrigger(Entity* other);
		/// <summary>
		/// 현재 애니메이션 키를 읽습니다. 응용 단계에서 사용할 일 없는 함수입니다.
		/// </summary>
		inline int getAnimKey() { return responseContinuously ? -1 : animKp; }
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가진 것들을 찾습니다. 없는 경우 빈 벡터를 반환합니다.
		/// </summary>
		static std::vector<Entity*> gets(const EntityKey& k);
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가지고 원하는 개체형인 것을 벡터에 담아 리턴합니다.
		/// </summary>
		template <class T>
		static std::vector<T*> gets(const EntityKey& k);
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가진 것 중 가장 앞의 하나를 찾습니다. 없는 경우 nullptr를 반환합니다. 이 키에 대하여 하나의 개체만 있을 것이 확실한 경우 사용하기에 좋습니다.
		/// </summary>
		static Entity* get(const EntityKey& k);
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가진 것 중 가장 앞의 하나를 찾아 원하는 개체형으로 캐스트하여 반환합니다.
		/// 없거나 가장 앞의 것이 타입이 일치하지 않는 경우 nullptr를 반환합니다. 이 키에 대하여 하나의 개체만 있을 것이 확실한 경우 사용하기에 좋습니다.
		/// </summary>
		template <class T>
		inline static T* get(const EntityKey& k) { return dynamic_cast<T*>(get(k)); }
		/// <summary>
		/// 개체를 제거합니다.
		/// </summary>
		inline static void destroy(Entity* e) { delete e; }
		/// <summary>
		/// 개체의 분류를 리턴합니다.
		/// </summary>
		inline char getClass() { return eclass; }
		/// <summary>
		/// 현존 객체 중에서 해당 키를 가지고 원하는 개체형인 것 중 가장 앞의 하나를 찾아 반환합니다. 단 하나도 없는 경우 nullptr를 반환합니다.
		/// </summary>
		template<class T>
		static T* get2(const EntityKey& k);
		
		struct __dropfromscene{
			friend class Scene;
		public:
			~__dropfromscene();
		private:
			Entity* __this = nullptr;
			Scene* relatedScene = nullptr;
			int index = -1;
		} __scene;
	protected:
		/// <summary>
		/// 개체의 위치, 크기, 회전을 나타냅니다.
		/// </summary>
		Transform transform;
		/// <summary>
		/// 개체의 시각(timepoint)입니다. 개체가 Active한 경우에만 증가합니다.
		/// </summary>
		inline float localTp() { return lt; }
		/// <summary>
		/// 개체의 현재 애니메이션 상태입니다.
		/// </summary>
		inline int animState() { return as; }
		inline void setModel(std::shared_ptr<Model>& model) { this->model = model; }
		/// <summary>
		/// 애니메이션 상태를 변경합니다. 현재 적용 중인 번호를 고르면 그 애니메이션은 처음부터 다시 시작합니다.
		/// </summary>
		inline void setAnim(int idx, float dynamicTps = 1) { as = idx; animKp = 0; animStartTimepoint = Game::tp(); animTps = dynamicTps; }
		/// <summary>
		/// 애니메이션을 저장 배열의 끝에 추가합니다. 첫 애니메이션을 추가할 때 animState는 0이 됩니다.
		/// </summary>
		void addAnim(const std::string& name);
		/// <summary>
		/// 애니메이션을 저장 배열의 끝에 추가합니다. 첫 애니메이션을 추가할 때 animState는 0이 됩니다. nullptr를 전달할 경우 저장되지 않습니다.
		/// </summary>
		void addAnim(pAnimation&);
		/// <summary>
		/// 개체의 색상입니다. 섞이는 것이 아니라 성분별 곱하기로 계산되는 점 주의하세요.
		/// </summary>
		vec4 color = 1;
		virtual ~Entity();
	};
}


#endif // !__OA_ENTITY_H__



