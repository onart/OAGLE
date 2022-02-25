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
	/// <summary>
	/// ���� �� ���� �����ϴ� ��ü���Դϴ�.
	/// </summary>
	class Entity
	{
	public:
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY ��ũ�θ� ������ ��� ���� Ű�� ����� �� �ֽ��ϴ�.
		using EntityKey = int;
#else
		using EntityKey = std::string;
#endif
		/// <summary>
		/// ��ü�� �����մϴ�. Ư���� �� ������ ������Ʈ�� ������ ���� ��ü�� ��� ���� �״�� ����� �� �ֽ��ϴ�.
		/// <para>��������Ʈ Ȥ�� ��: addAnim(), setModel()�� ���մϴ�.</para>
		/// <para>����� ��: isFixed=true�� ��� ī�޶��� ������ ���� �ʽ��ϴ�. (�̴� [-1,1]^3�� �� ������ ��ô�� ����� �Ѵٴ� �ǹ��Դϴ�)</para>
		/// </summary>
		/// <param name="k">��ü�� �̸�(Ȥ�� id)�Դϴ�. �⺻������ std::string Ÿ�������� OA_USE_INT_AS_KEY ��ũ�� ��� �� int�� ����� �� �ֽ��ϴ�.</param>
		/// <param name="transform">��ü�� ��ġ, ũ��, ȸ���� ��Ÿ���ϴ�.</param>
		/// <param name="isFixed">ī�޶��� �̵��� ������ �޴� ���θ� �����մϴ�. 2D, 3D ��� �����մϴ�.</param>
		Entity(const EntityKey& k, const Transform& transform, bool isFixed = false);
		~Entity();
		/// <summary>
		/// ��ġ�� ����ϴ�. ī�޶� ��� ���� ���� ��ǥ�� ���������� �����ϴ� ���� ���� ����������ϴ�.
		/// </summary>
		inline const vec3& getPos() { return transform.getPosition(); }
		/// <summary>
		/// ��ü�� active�ϴٴ� ���� update()�� �� ������ ȣ��ȴٴ� �ǹ��Դϴ�.
		/// </summary>
		bool isActive = true;
		/// <summary>
		/// true�� ��쿡�� ��ü�� ȭ�鿡 �������˴ϴ�.
		/// </summary>
		bool isRendered = true;
		/// <summary>
		/// ��ü�� Ű�� �����մϴ�.
		/// </summary>
		/// <returns></returns>
		inline const EntityKey& name() const { return key; }
		/// <summary>
		/// ���� �ܰ迡�� ������� �ʴ� ���� �����ϴ�.
		/// </summary>
		void act(int kp);
		/// <summary>
		/// �ִϸ��̼��� Ư�� Ű����Ʈ�� ������ �� ��ü�� ���� �ൿ�Դϴ�.
		/// �̺�Ʈ�� �����ϴ� �뵵�� ����� ���� ������ Ű����Ʈ ��ȣ�� �ߺ����� �ʵ��� ���� �ܰ迡�� �����ϴ� ���� ��õ�˴ϴ�.
		/// </summary>
		/// <param name="kp">�̺�Ʈ �÷���/Ű����Ʈ</param>
		virtual void Act(int kp);
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
		inline int getAnimKey() { return animKp; }
		/// <summary>
		/// ���� ��ü �߿��� �ش� Ű�� ���� �͵��� ã���ϴ�. ���� ��� �� ���͸� ��ȯ�մϴ�.
		/// </summary>
		static std::vector<Entity*> gets(const EntityKey& k);
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
		inline void setModel(Model* model) { this->model = model; }
		/// <summary>
		/// �ִϸ��̼� ���¸� �����մϴ�. ���� ���� ���� ��ȣ�� ���� �� �ִϸ��̼��� ó������ �ٽ� �����մϴ�.
		/// </summary>
		inline void setAnim(int idx, float dynamicTps = 1) { as = idx; animKp = 0; animStartTimepoint = tp; animTps = dynamicTps; }
		/// <summary>
		/// �ִϸ��̼��� ���� �迭�� ���� �߰��մϴ�. ù �ִϸ��̼��� �߰��� �� animState�� 0�� �˴ϴ�.
		/// </summary>
		void addAnim(const std::string& name);
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
	private:
		float lt = 0;
		EntityKey key;
		Model* model = nullptr;
		int as = -1;
		float animStartTimepoint;
		float animTps = 1;
		int animKp = 0;
		std::vector<pAnimation> anims;
		bool isFixed;

		static std::multimap<EntityKey, Entity*> entities;
	};
}


#endif // !__OA_ENTITY_H__



