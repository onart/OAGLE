/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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
		void render(const vec4& color = 1) const;
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


