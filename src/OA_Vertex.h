/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

#define USE_BUMP
#define USE_ANIM

namespace onart {

	/// <summary>
	/// ������ ������������ �����Դϴ�.
	/// <para>���� ������ ���ؼ��� USE_BUMP ��ũ�ΰ� ���ǵǾ�� �մϴ�.</para>
	/// <para>���� �ִϸ��̼��� ���ؼ��� USE_ANIM ��ũ�ΰ� ���ǵǾ�� �մϴ�.</para>
	/// </summary>
	struct Vertex
	{
		/// <summary>
		/// �� �� ������ ��ġ�Դϴ�.
		/// </summary>
		vec3 pos;
		/// <summary>
		/// ���������� �����Դϴ�.
		/// </summary>
		vec3 norm;
		/// <summary>
		/// 2D �ؽ�ó ��ǥ�Դϴ�.
		/// </summary>
		vec2 tc;
#ifdef USE_BUMP
		/// <summary>
		/// ���� ������ ���� ���� �����Դϴ�.
		/// </summary>
		vec3 tan;
		/// <summary>
		/// ���� ������ ���� ���� ���Ϳ� ������ �Ǵ� �����Դϴ�. 
		/// </summary>
		vec3 bitan;
#endif // USE_BUMP
#ifdef USE_ANIM
		/// <summary>
		/// ���� �ִϸ��̼��� ���� ���� �ε����Դϴ�.
		/// </summary>
		ivec4 boneIDs = { -1,-1,-1,-1 };
		/// <summary>
		/// ���� �ִϸ��̼��� ���� ���� �����(����ġ)�Դϴ�.
		/// </summary>
		vec4 boneWeights = { 0,0,0,0 };
#endif // USE_ANIM

	};

	/// <summary>
	/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� Ŭ�����Դϴ�.
	/// </summary>
	class Mesh {
		friend struct std::default_delete<Mesh>;
		public:
			/// <summary>
			/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� �̸����� ã�ƿɴϴ�. ���� ��� nullptr�� ���� �����͸� �����մϴ�.
			/// </summary>
			/// <param name="name">�ҷ��� �� ���� ���� �̸�</param>
			/// <returns>�޽��� ��ġ�� ����Ű�� �������� �ּ�(Mesh**)</returns>
			static std::shared_ptr<std::unique_ptr<Mesh>> get(const std::string& name);
			/// <summary>
			/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� ���� �߰��մϴ�.
			/// ��, ����� �̸��� ����� �� ������ �� �ܿ� �̹� �ִ� �̸��� ������� �õ��ϸ� ���� ���� ����ϴ�.
			/// ����̸� ������ �� ���� ����ϰ� �ִ� ��� ���� ���� �߰��� ������ �ٲ�ϴ�. ���� ���θ� �����մϴ�(�޸� ���� Ȥ�� ��Ʈ�� �̸��� ����� ���� �ƴϸ� ����).
			/// <para>����� �̸�: rect, circ, sphr, clnd, cubo, icubo, (�� ���ڿ�)</para>
			/// </summary>
			static bool add(const std::string& name, const std::vector<Vertex>& v, const std::vector<unsigned>& i);
			/// <summary>
			/// ���� ���� �޸𸮿��� �����մϴ�. ��Ʈ�� �𵨰� ��� ���� ���� ������ �� �����ϴ�.
			/// </summary>
			/// <param name="name">�����Ϸ��� �� �̸�</param>
			/// <returns>���� ����</returns>
			static bool drop(const std::string& name);
			/// <summary>
			/// ����ϰ� ���� ���� �޽ø� ��� �޸𸮿��� �����մϴ�.
			/// </summary>
			static void collect();
			/// <summary>
			/// ���� �迭 ������Ʈ�� id�� ����ϴ�.
			/// </summary>
			inline unsigned getID() const { return vao; }
			inline unsigned getVB() const { return vb; }
			inline unsigned getIB() const { return ib; }
			/// <summary>
			/// ���� �迭�� ���̸� ����ϴ�.
			/// </summary>			
			inline unsigned getLength() const { return length; }
			/// <summary>
			/// ���簢�� ���� �迭 ������Ʈ�� �����մϴ�. �� ȣ�� �� ���� ó������ �ٽ� �����մϴ�.
			/// �ؽ�ó �̹����� �״�� ���簢���� ���ϴ�.
			/// </summary>
			static void rectModel();
			/// <summary>
			/// �� ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. �� ȣ�� �� ���� ó������ �ٽ� �����մϴ�.
			/// �ؽ�ó �̹����� ���簢�� �̹����� �����ϴ� ���Դϴ�.
			/// </summary>
			static void circleModel();
			/// <summary>
			/// �� ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. �� ȣ�� �� ���� ó������ �ٽ� �����մϴ�.
			/// �ؽ�ó �̹����� �������� ���������� ����� �����մϴ�.
			/// </summary>
			static void sphereModel();
			/// <summary>
			/// ������ü ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. �� ȣ�� �� ���� ó������ �ٽ� �����մϴ�.
			/// �ؽ�ó �̹����� 4x3 �迭�� ���� �� ���簢���� ���� �� 1�� 2���� ��,
			/// 2���� ���� ��, ��, ��, ��,
			/// 3�� 2���� �� �����Դϴ�.
			/// </summary>
			static void cuboidModel();
			/// <summary>
			/// ������ü ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. �ؽ�ó�� ������ü ���ο����� ���̵��� �ϴ� ���̰� �ֽ��ϴ�. �� ȣ�� �� ���� ó������ �ٽ� �����մϴ�.
			/// �ؽ�ó �̹����� 4x3 �迭�� ���� �� ���簢���� ���� �� 1�� 2���� ��,
			/// 2���� ���� ��, ��, ��, ��,
			/// 3�� 2���� �� �����Դϴ�.
			/// </summary>
			static void iCuboidModel();
			/// <summary>
			/// ���� ���Ϳ� �ε��� ���͸� ������ ���� ���� ������Ʈ�� �����ϰ� �����մϴ�.
			/// </summary>
			/// <param name="v">���� ����</param>
			/// <param name="i">�ε��� ����</param>
			/// <param name="vb">���� ���۸� ���� �ּ�</param>
			/// <param name="ib">�ε��� ���۸� ���� �ּ�</param>
			/// <returns></returns>
			static unsigned createVAO(const std::vector<Vertex>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib);
			/// <summary>
			/// ���� ���ۿ� �ε��� ���۸� ������ ���� ���� ������Ʈ�� �����ϰ� �����մϴ�.
			/// </summary>
			static unsigned createVAO(unsigned vb, unsigned ib);
			inline Mesh() {}
		private:
			inline Mesh(unsigned vb, unsigned ib, unsigned vao, unsigned length) :vb(vb), ib(ib), vao(vao), length(length) {};
			~Mesh();	// ���α׷� ���� ������ ���Ƿ� �޽� ���� �Ұ���
			unsigned vb = 0, ib = 0, vao = 0;
			const unsigned length = 0;

			static std::map<std::string, std::shared_ptr<std::unique_ptr<Mesh>>> list;
	};

	using ppMesh = std::shared_ptr<std::unique_ptr<Mesh>>;	// Mesh Ŭ������ ���� ���� �������Դϴ�.
}

#endif // !__OA_VERTEX_H__



