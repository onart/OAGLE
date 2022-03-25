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
#include "OA_Input.h"
#include "OA_Anim.h"

namespace onart {
	class Camera;
}

extern onart::Camera mainCamera;

namespace onart {
	class Model;
	class Scene;
	/// <summary>
	/// ���� �� ���� �����ϴ� ��ü���Դϴ�.
	/// </summary>
	class Entity
	{
	public:
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY ��ũ�θ� ������ ��� ���� Ű�� ����� �� �ֽ��ϴ�.
		using EntityKey = int;
#else
		using EntityKey = std::string;	// const char*�� ���� ����
#endif
		/// <summary>
		/// ��ü�� �����մϴ�. Ư���� �� ������ ������Ʈ�� ������ ���� ��ü�� ��� ���� �״�� ����� �� �ֽ��ϴ�.
		/// <para>��������Ʈ Ȥ�� ��: addAnim(), setModel()�� ���մϴ�.</para>
		/// <para>����� ��: isFixed=true�� ��� ī�޶��� ������ ���� �ʽ��ϴ�. (�̴� [-1,1]^3�� �� ������ ��ô�� ����� �Ѵٴ� �ǹ��Դϴ�)</para>
		/// </summary>
		/// <param name="k">��ü�� �̸�(Ȥ�� id)�Դϴ�. �⺻������ std::string Ÿ�������� OA_USE_INT_AS_KEY ��ũ�� ��� �� int�� ����� �� �ֽ��ϴ�.</param>
		/// <param name="transform">��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.</param>
		/// <param name="isFixed">ī�޶��� �̵��� ������ �޴� ���θ� �����մϴ�. false�� ������ �� �޴´ٴ� ������, 2D, 3D ��� �����մϴ�.</param>
		/// <param name="responseContinuously">true�� ��� �ִϸ��̼� ���� ���¿� ���Ͽ� ���������� �����մϴ�.</param>
		/// <param name="isTranslucent">�� �� �ִϸ��̼� �� �ϳ��� ������ �����̶� �ִ� ��� true�� �����մϴ�.</param>
		Entity(const EntityKey& k, const Transform& transform, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = false);
		/// <summary>
		/// ��ü�� �����մϴ�. Ư���� �� ������ ������Ʈ�� ������ ���� ��ü�� ��� ���� �״�� ����� �� �ֽ��ϴ�.
		/// <para>��������Ʈ Ȥ�� ��: addAnim(), setModel()�� ���մϴ�.</para>
		/// <para>����� ��: isFixed=true�� ��� ī�޶��� ������ ���� �ʽ��ϴ�. (�̴� [-1,1]^3�� �� ������ ��ô�� ����� �Ѵٴ� �ǹ��Դϴ�)</para>
		/// </summary>
		/// <param name="k">��ü�� �̸�(Ȥ�� id)�Դϴ�. �⺻������ std::string Ÿ�������� OA_USE_INT_AS_KEY ��ũ�� ��� �� int�� ����� �� �ֽ��ϴ�.</param>
		/// <param name="anim0">�⺻ ������ ���մϴ�. ���´� ������ ������Ʈ �Լ��� �ʿ� ���� �� �����մϴ�.</param>
		/// <param name="transform">��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.</param>
		/// <param name="isFixed">ī�޶��� �̵��� ������ �޴� ���θ� �����մϴ�. false�� ������ �� �޴´ٴ� ������, 2D, 3D ��� �����մϴ�.</param>
		/// <param name="responseContinuously">true�� ��� �ִϸ��̼� ���� ���¿� ���Ͽ� ���������� �����մϴ�.</param>
		/// <param name="isTranslucent">�ִϸ��̼ǿ� ������ �����̶� �ִ� ��� true�� �����մϴ�.</param>
		Entity(const EntityKey& k, const Transform& transform, pAnimation& anim0, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = true);
		/// <summary>
		/// ��ü�� �����մϴ�. Ư���� �� ������ ������Ʈ�� ������ ���� ��ü�� ��� ���� �״�� ����� �� �ֽ��ϴ�.
		/// <para>��������Ʈ Ȥ�� ��: addAnim(), setModel()�� ���մϴ�.</para>
		/// <para>����� ��: isFixed=true�� ��� ī�޶��� ������ ���� �ʽ��ϴ�. (�̴� [-1,1]^3�� �� ������ ��ô�� ����� �Ѵٴ� �ǹ��Դϴ�)</para>
		/// </summary>
		/// <param name="k">��ü�� �̸�(Ȥ�� id)�Դϴ�. �⺻������ std::string Ÿ�������� OA_USE_INT_AS_KEY ��ũ�� ��� �� int�� ����� �� �ֽ��ϴ�.</param>
		/// <param name="transform">��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.</param>
		/// <param name="model">�⺻ ������ ���մϴ�. ���´� ������ ������Ʈ �Լ��� �ʿ� ���� �� �����մϴ�.</param>
		/// <param name="isFixed">ī�޶��� �̵��� ������ �޴� ���θ� �����մϴ�. false�� ������ �� �޴´ٴ� ������, 2D, 3D ��� �����մϴ�.</param>
		/// <param name="responseContinuously">true�� ��� �ִϸ��̼� ���� ���¿� ���Ͽ� ���������� �����մϴ�.</param>
		/// <param name="isTranslucent">�� �ؽ�ó�� ������ �����̶� �ִ� ��� true�� �����մϴ�.</param>
		Entity(const EntityKey& k, const Transform& transform, std::shared_ptr<Model>& model, bool isFixed = false, bool responseContinuously = false, bool isTranslucent = false);
		Entity(const Entity&) = delete;
		/// <summary>
		/// ��ġ�� ����ϴ�. ī�޶� ��� ���� ���� ��ǥ�� ���������� �����ϴ� ���� ���� ����������ϴ�.
		/// </summary>
		inline const vec3& getPos() { return transform.getGlobalPosition(); }
		/// <summary>
		/// ��ü�� active�ϴٴ� ���� update()�� �� ������ ȣ��ȴٴ� �ǹ��Դϴ�.
		/// </summary>
		bool isActive = true;
		/// <summary>
		/// true�� ��쿡�� ��ü�� ȭ�鿡 �������˴ϴ�.
		/// </summary>
		bool isRendered = true;
		/// <summary>
		/// true�� ��� �� ���� �� ��ü�� �ı����� �ʽ��ϴ�. �� ���� ������ ������ �� �ֽ��ϴ�.
		/// </summary>
		bool preserveOnSceneChange = false;
		/// <summary>
		/// ��ü�� ������ �����̶� �ִ� ��� true�� ������ �ּ���. 2D ��ü�� ��� ������ ���簢�� �̹����� �ƴϸ� ��� true���� �մϴ�.
		/// �յ� ���� ������ ���� 2D ����(ex: 90�� ���� ����, 90�� ���� ����)������ ���� ��� ��ü�� ������ �����ϱ� ������ ������ �ʿ� �����ϴ�. �̶��� OAGLE_2DGAME ��ũ�θ� �� ���� ���� ������ �ּ���. ���� ������ �ټ� �������� �� ����� �Ű� �� �ʿ䰡 �������ϴ�.
		/// 2D �����̶� ���ϸ�� ���� �񽺵��� ������ ��� �繰�� ���� ���谡 ���ϱ� ������ OAGLE_2DGAME ��ũ�θ� ������� �ʾƾ� �մϴ�. (����� ��� ó���� �տ� �ִ� ��ü�� �ڷ� �� ��� ���簢�� ������ŭ ������)
		/// </summary>
		const bool isTranslucent;
		/// <summary>
		/// ��ü�� Ű�� �����մϴ�.
		/// </summary>
		/// <returns></returns>
		inline const EntityKey& name() const { return key; }
		/// <summary>
		/// ī�޶� ���� z��ǥ�� �����մϴ�.
		/// </summary>
		float zIndex();
		/// <summary>
		/// ���� �ܰ迡�� ������� �ʴ� ���� �����ϴ�.
		/// </summary>
		void act(int kp, float progress = 0);
		/// <summary>
		/// �ִϸ��̼��� Ư�� Ű����Ʈ�� ������ �� ��ü�� ���� �ൿ�Դϴ�.
		/// �̺�Ʈ�� �����ϴ� �뵵�� ����� ���� ������ Ű����Ʈ ��ȣ�� �ߺ����� �ʵ��� ���� �ܰ迡�� �����ϴ� ���� ��õ�˴ϴ�.
		/// </summary>
		/// <param name="kp">�̺�Ʈ �÷���/Ű����Ʈ</param>
		virtual void Act(int kp, float progress = 0);
		/// <summary>
		/// �⺻������ ���� �������� ������ �� ���� �Լ��Դϴ�. �� �ʿ��� ��찡 �ƴ϶�� �������̵� �� ȣ������ �ʴ� ���� �����ϴ�.
		/// </summary>
		virtual void render();
		/// <summary>
		/// �⺻������ ���� �������� ������ �� ���� �Լ��Դϴ�. �� �ʿ��� ��찡 �ƴ϶�� �������̵� �� ȣ������ �ʴ� ���� �����ϴ�.
		/// </summary>
		virtual void update();
		/// <summary>
		/// Update() �Լ��� �������̵��Ͽ� ��ü�� �����Ӹ��� ���ϴ� �ൿ�� �����մϴ�.
		/// </summary>
		virtual void Update();
		/// <summary>
		/// ���� �ִϸ��̼� Ű�� �н��ϴ�. ���� �ܰ迡�� ����� �� ���� �Լ��Դϴ�.
		/// </summary>
		inline int getAnimKey() { return responseContinuously ? -1 : animKp; }
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ���� �͵��� ã���ϴ�. ���� ��� �� ���͸� ��ȯ�մϴ�.
		/// </summary>
		static std::vector<Entity*> gets(const EntityKey& k);
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ������ ���ϴ� ��ü���� ���� ���Ϳ� ��� �����մϴ�.
		/// </summary>
		template <class T>
		static std::vector<T*> gets(const EntityKey& k);
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ���� �� �� ���� ���� �ϳ��� ã���ϴ�. ���� ��� nullptr�� ��ȯ�մϴ�. �� Ű�� ���Ͽ� �ϳ��� ��ü�� ���� ���� Ȯ���� ��� ����ϱ⿡ �����ϴ�.
		/// </summary>
		static Entity* get(const EntityKey& k);
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ���� �� �� ���� ���� �ϳ��� ã�� ���ϴ� ��ü������ ĳ��Ʈ�Ͽ� ��ȯ�մϴ�.
		/// ���ų� ���� ���� ���� Ÿ���� ��ġ���� �ʴ� ��� nullptr�� ��ȯ�մϴ�. �� Ű�� ���Ͽ� �ϳ��� ��ü�� ���� ���� Ȯ���� ��� ����ϱ⿡ �����ϴ�.
		/// </summary>
		template <class T>
		inline static T* get(const EntityKey& k) { return dynamic_cast<T*>(get(k)); }
		/// <summary>
		/// ��ü�� �����մϴ�.
		/// </summary>
		inline static void destroy(Entity* e) { delete e; }
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ������ ���ϴ� ��ü���� �� �� ���� ���� �ϳ��� ã�� ��ȯ�մϴ�. �� �ϳ��� ���� ��� nullptr�� ��ȯ�մϴ�.
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
		/// ��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.
		/// </summary>
		Transform transform;
		/// <summary>
		/// ��ü�� �ð�(timepoint)�Դϴ�. ��ü�� Active�� ��쿡�� �����մϴ�.
		/// </summary>
		const float& localTp;
		/// <summary>
		/// ��ü�� ���� �ִϸ��̼� �����Դϴ�.
		/// </summary>
		const int& animState;
		inline void setModel(std::shared_ptr<Model>& model) { this->model = model; }
		/// <summary>
		/// �ִϸ��̼� ���¸� �����մϴ�. ���� ���� ���� ��ȣ�� ���� �� �ִϸ��̼��� ó������ �ٽ� �����մϴ�.
		/// </summary>
		inline void setAnim(int idx, float dynamicTps = 1) { as = idx; animKp = 0; animStartTimepoint = tp; animTps = dynamicTps; }
		/// <summary>
		/// �ִϸ��̼��� ���� �迭�� ���� �߰��մϴ�. ù �ִϸ��̼��� �߰��� �� animState�� 0�� �˴ϴ�.
		/// </summary>
		void addAnim(const std::string& name);
		/// <summary>
		/// �ִϸ��̼��� ���� �迭�� ���� �߰��մϴ�. ù �ִϸ��̼��� �߰��� �� animState�� 0�� �˴ϴ�. nullptr�� ������ ��� ������� �ʽ��ϴ�.
		/// </summary>
		void addAnim(pAnimation&);
		/// <summary>
		/// ������ ����� �� ���� �����ӿ� ��������� �帥 �ð�(��)�Դϴ�.
		/// </summary>
		static const float& tp;
		/// <summary>
		/// ���� �����Ӱ� ���� ������ ���� �ð� ����(��)�Դϴ�.
		/// </summary>
		static const float& dt;
		/// <summary>
		/// ��ü�� �����Դϴ�. ���̴� ���� �ƴ϶� ���к� ���ϱ�� ���Ǵ� �� �����ϼ���.
		/// </summary>
		vec4 color = 1;
		virtual ~Entity();
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

		static std::multimap<EntityKey, Entity*> entities;
	};
}


#endif // !__OA_ENTITY_H__



