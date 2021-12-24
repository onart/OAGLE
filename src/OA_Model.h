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
		void render(const Shader& shader);
	private:
		Mesh** mesh;
		vec4 color;
		std::string meshName;
		
		std::vector<Material*> materials;
		std::vector<size_t> startIdx;
		std::vector<size_t> idxLen;
	};
}

#endif // !__OA_MODEL_H__


