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
#ifdef OA_USE_INT_AS_KEY	// OA_USE_INT_AS_KEY ��ũ�θ� ������ ��� ���� Ű�� ����� �� �ֽ��ϴ�.
	using EntityKey = int;
#else
	using EntityKey = std::string;
#endif
	/// <summary>
	/// ���ӿ� ǥ�õǴ� ���Դϴ�.
	/// </summary>
	class Scene
	{
	public:
		/// <summary>
		/// ���� ���� �ִ� ���Դϴ�. ���ӿ��� �� ���� �ϳ��� ���� ���� ���� �� �ֽ��ϴ�.
		/// </summary>
		static Scene* currentScene;
		/// <summary>
		/// pause()�� �ص� ������ �ʰ� �ϰ� ���� ��ü�� Ű�� ����մϴ�. �ߺ� ��ü�� �ټ� �����ϴ� Ű�� ����Ѵٸ�, �׿� ���� ��� ��ü�� pause()�δ� ������ �ʽ��ϴ�.
		/// ���� ����� ��ü�� ���� ��ȯ�Ǿ �����˴ϴ�. �� ��ü�� ����� �� �����ϴ�. �Ͻ����� �� �׷� �κ��� ������ ���� ���� ��������, ��ϵ� ��ü�� key ���� �������� ������� ������Ʈ�˴ϴ�.
		/// </summary>
		inline static void noStop(const EntityKey& k) { nonstop.insert(k); }
		/// <summary>
		/// ������ �ʵ��� ����ߴ� Ű�� ��� �����մϴ�.
		/// </summary>
		inline static void yesStop(const EntityKey& k) { nonstop.erase(k); }
		/// <summary>
		/// ���� �������� ����� �� ���� �Լ��Դϴ�.
		/// </summary>
		void update();
		/// <summary>
		/// ���� �������� ����� �� ���� �Լ��Դϴ�.
		/// </summary>
		void render();
		inline int getSceneId() { return id; }
		/// <summary>
		/// ������ �� �����Ӹ��� ȣ��˴ϴ�. �׻� �� �� ��ü�� Update()���� ���� ����˴ϴ�.
		/// </summary>
		virtual void Update() = 0;
		/// <summary>
		/// �� �����͸� ���� �̺�Ʈ�� �ߵ��� �� �ֽ��ϴ�.
		/// </summary>
		/// <param name="idx">�� ������ ������ �̺�Ʈ ��ȣ</param>
		virtual void actEvent(int idx);
		/// <summary>
		/// ���� �ʱ�ȭ�մϴ�. �ַ� �� ��ü�� �����մϴ�.
		/// </summary>
		virtual void init() = 0;
		/// <summary>
		/// ī�޶��� �̵��� �����մϴ�. ī�޶��� ���� ������ ��ġ�� ���� �����ӿ� ���� �ڸ��Ϸ��� ��ġ�� �Է¹޾� ī�޶� ������ �־�� �� ��ġ�� �����մϴ�.
		/// �������̵����� �ʴ� ���, ���������� ���ֵ˴ϴ�.
		/// </summary>
		/// <param name="currentCameraPos">���� �������� ī�޶� ��ġ</param>
		/// <param name="desiredCameraPos">���� �����ӿ� ī�޶� �ڸ��Ϸ��� ��ġ</param>
		/// <returns>���� �������� ������ ����� ���� ���� �����ӿ� ī�޶� �ڸ��ϴ� ��ġ</returns>
		virtual vec3 constrainCamera(const vec3& currentCameraPos, const vec3& desiredCameraPos);

		virtual ~Scene() {};
	protected:
		std::vector<Entity*> entities;
		int id;
		/// <summary>
		/// ���� ���� ���� �帥 �ð�(��)�Դϴ�.
		/// </summary>
		static const float& tp;
		/// <summary>
		/// ���� �����Ӱ� ���� ������ ���� �ð� ����(��)�Դϴ�.
		/// </summary>
		static const float& dt;
		/// <summary>
		/// �ٸ� ������ ��ȯ�մϴ�.
		/// </summary>
		void change(Scene* other);
		/// <summary>
		/// �� �� ��� ��ü�� ������Ʈ�� �ߴ��մϴ�. �� ��ü�� isActive�� �����Ű�� �ʰ� ���������� �����մϴ�. �������� �ߴ����� �ʽ��ϴ�.
		/// �ߴܵ��� �ʴ� ���� Ÿ���� noStop(), yesStop()�� ���� ��� �� ����� �� �ֽ��ϴ�.
		/// </summary>
		inline void pause() { isPaused = true; }
		/// <summary>
		/// pause�� ����մϴ�.
		/// </summary>
		inline void resume() { isPaused = false; }
		// +�ټ��� ����
	private:
		bool isPaused = false;
		static std::set<EntityKey> nonstop;
		// +bgm
	};
}

#endif // !__OA_SCENE_H__
