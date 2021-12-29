#ifndef __OA_ENTITY_H__
#define __OA_ENTITY_H__

#include <vector>
#include <string>
#include <map>
#include "OA_Transform.h"

namespace onart {
	class Model;
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
		/// ������ ����� �� ���� �����ӿ� ��������� �帥 �ð�(��)�Դϴ�.
		/// </summary>
		static const float& tp;
	private:
		float lt = 0;
		EntityKey key;
		Model* model;
		static std::multimap<EntityKey, Entity*> entities;
	};
}


#endif // !__OA_ENTITY_H__



