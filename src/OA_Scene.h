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
		/// ���� �������� ����� �� ���� �Լ��Դϴ�.
		/// </summary>
		void update();
		/// <summary>
		/// ���� �������� ����� �� ���� �Լ��Դϴ�.
		/// </summary>
		void render();
		/// <summary>
		/// ���� ��ü�� �߰��մϴ�.
		/// </summary>
		void addEntity(Entity*);

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
		static struct __dropentity{
			friend class Entity;
		private:
			/// <summary>
			/// �̰��� ������ ��ü�� �����մϴ�. ��ü�� �޸𸮰� ���������� ������, ��ü�� �Ҹ��� ���� �� �Լ��� �ڵ����� ȣ��˴ϴ�.
			/// </summary>
			static void dropEntity(Entity*);
		}__drop;
		inline virtual ~Scene() {};
	protected:
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
		// +�ټ��� ����
	private:	
		std::vector<Entity*> entities;
		std::multimap<float, size_t> renderOrder;
		std::vector<Entity*> bubble;
		void reap();
		bool shouldReap = false;
		// +bgm
	};
}

#endif // !__OA_SCENE_H__
