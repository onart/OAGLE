#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

#include <map>
#include <string>

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
	/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� Ŭ�����Դϴ�. �⺻������ ���� ������ �Ұ����մϴ�.
	/// </summary>
	class Mesh {
		public:
			/// <summary>
			/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� �̸����� ã�ƿɴϴ�. ���� ��� nullptr�� �����մϴ�.
			/// </summary>
			/// <param name="name">�ҷ��� �� ���� ���� �̸�</param>
			/// <returns>�޽��� ��ġ�� ����Ű�� �������� �ּ�(Mesh**)</returns>
			static Mesh** get(const std::string& name);
			/// <summary>
			/// ���� ��(���͸���, �ִϸ��̼� ���� ���� ��)�� ���� �߰��մϴ�. ��, ����� �̸��� ����� �� ������ �� �ܿ� �̹� �ִ� �̸��� ������� �õ��ϸ� ���� ���� ����ϴ�. ���� ���θ� �����մϴ�.
			/// <para>����� �̸�: rect, circ, sphr, clnd, cubo, (�� ���ڿ�)</para>
			/// </summary>
			static bool add(const std::string& name, Mesh* m);
			/// <summary>
			/// ���� ���� �޸𸮿��� �����մϴ�. ��, ��Ʈ�� ���� ������ �� �����ϴ�. ���� ���θ� �����մϴ�. �̹� ���� �̸��� �����Ϸ��� �õ��� ��� �������� ��޵˴ϴ�.
			/// </summary>
			static bool unload(const std::string& name);
			/// <summary>
			/// ���� �迭 ������Ʈ�� id�� ����ϴ�.
			/// </summary>
			inline unsigned getID() const { return vao; }
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
			/// ���� ���ۿ� �ε��� ���۸� ������ ���� ���� ������Ʈ�� �����ϰ� �����մϴ�.
			/// </summary>
			static unsigned createVAO(unsigned vb, unsigned ib);
			inline Mesh() {}
		private:
			inline Mesh(unsigned vb, unsigned ib, unsigned vao, unsigned length) :vb(vb), ib(ib), vao(vao), length(length) {};
			~Mesh();	// ���α׷� ���� ������ ���Ƿ� �޽� ���� �Ұ���
			unsigned vb = 0, ib = 0, vao = 0;
			const unsigned length = 0;

			static std::map<std::string, Mesh*> list;
	};
}

#endif // !__OA_VERTEX_H__



