#ifndef __OA_MODEL_H__
#define __OA_MODEL_H__

#include <string>
#include <vector>
#include <map>
#include "oaglem.h"

struct aiScene;

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
		enum class TexType { AMBIENT, DIFFUSE, SPECULAR, NORMAL };
		/// <summary>
		/// �޸𸮿��� ���� �ҷ��ɴϴ�.
		/// </summary>
		/// <param name="data">���� �ּ�</param>
		/// <param name="len">������ ����</param>
		/// <param name="meshName">�޽� �̸�</param>
		/// <param name="hint">���� ��Ʈ</param>
		static Model* load(const unsigned char* data, size_t len, const std::string& meshName, const char* hint = "");
		/// <summary>
		/// ���Ͽ��� ���� �ҷ��ɴϴ�. �̶�, �޽� �̸��� ���� �̸��� ���� ����˴ϴ�.
		/// </summary>
		static Model* load(const std::string& file);
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
		void render(Shader& shader, const int material = 0, const vec4& color = 1);
		/// <summary>
		/// �̹� �ҷ��� �𵨿� diffuse �ؽ�ó �̹����� �߰��մϴ�.
		/// </summary>
		/// <param name="index">ä�� �ε����Դϴ�. ����� ����� ���� �޽������� � �𵨿� � �ε����� ä���� Ȯ���� �ּ���.</param>
		/// /// <param name="tex">�ؽ�ó�Դϴ�. Material::genTextureFrom... �Լ��鿡�� ���� �� �ֽ��ϴ�.</param>
		/// <param name="typ">�ؽ�ó �����Դϴ�. ���� ������ DIFFUSE, NORMAL�� ����մϴ�.</param>
		void addTex(unsigned index, unsigned tex, TexType typ);
	private:
		struct Geometry {
			unsigned start;
			unsigned count;
			unsigned material;
		};
		Model(const aiScene*, const std::string& meshName);
		Mesh** mesh;
		std::string meshName;

		std::vector<Material*> materials;
		std::vector<Geometry> geom;
		
		static std::map<std::string, Model*> list;
	};
}

#endif // !__OA_MODEL_H__


