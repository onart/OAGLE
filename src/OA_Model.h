#ifndef __OA_MODEL_H__
#define __OA_MODEL_H__

#include <string>
#include <vector>
#include "oaglem.h"

namespace onart {
	
	class Mesh;
	class Material;
	class Shader;
	/// <summary>
	/// ���������� ȭ�鿡 ������ �� Ŭ�����Դϴ�.
	/// ���� ���� �޽� �� �ؽ�ó�� ���ؼ��� �׸� �� �ֽ��ϴ�.
	/// </summary>
	class Model
	{
	public:
		/// <summary>
		/// �޽ð� ���ŵ� ��� �ٽ� �ε��մϴ�.
		/// </summary>
		void reloadMesh();
		/// <summary>
		/// �ش� ���̴��� �� ���� �������մϴ�.
		/// </summary>
		/// <param name="shader">���̴�</param>
		/// <param name="material">���͸��� ��ȣ(�� �𵨿��� ����ϴ� �͸� �����ϴ�.)</param>
		/// <param name="color">����(mix�� �ƴ϶� *�� ����˴ϴ�.)</param>
		void render(Shader& shader, const unsigned material = 0, const vec4& color = 1);
	private:
		Mesh** mesh;
		std::string meshName;

		std::vector<Material*> materials;
	};
}

#endif // !__OA_MODEL_H__


