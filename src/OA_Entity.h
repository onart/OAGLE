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
	/// ���� �� ���� �����ϴ� ��ü���Դϴ�.
	/// </summary>
	class Entity
	{
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY ��ũ�θ� ������ ��� ���� Ű�� ����� �� �ֽ��ϴ�.
		using EntityKey = int;
#else
		using EntityKey = std::string;
#endif
	public:
		Entity(const EntityKey& k, const Transform& transform);
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



