#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

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

	class Model {
		public:
			/// <summary>
			/// ���� �迭 ������Ʈ�� id�� ����ϴ�.
			/// </summary>
			inline unsigned getID() const { return id; }
			/// <summary>
			/// ���� �迭�� ���̸� ����ϴ�.
			/// </summary>			
			inline unsigned getLength() const { return length; }
			/// <summary>
			/// ���簢�� ���� �迭 ������Ʈ�� �����մϴ�. STL map�� models���� ã�� �� �ֽ��ϴ�. ("rect")
			/// �ؽ�ó �̹����� �״�� ���簢���� ���ϴ�.
			/// </summary>
			static void rectModel(bool reset = false);
			/// <summary>
			/// �� ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. STL map�� models���� ã�� �� �ֽ��ϴ�. ("circ")
			/// �ؽ�ó �̹����� ���簢�� �̹����� �����ϴ� ���Դϴ�.
			/// </summary>
			static void circleModel(bool reset = false);
			/// <summary>
			/// �� ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. STL map�� models���� ã�� �� �ֽ��ϴ�. ("sphr")
			/// �ؽ�ó �̹����� �������� ���������� ����� �����մϴ�.
			/// </summary>
			static void sphereModel(bool reset = false);
			/// <summary>
			/// ������ü ���� �迭 ������Ʈ�� �����ϰ� �����մϴ�. STL map�� models���� ã�� �� �ֽ��ϴ�. ("cubo")
			/// �ؽ�ó �̹����� 4x3 �迭�� ���� �� ���簢���� ���� �� 1�� 2���� ��,
			/// 2���� ���� ��, ��, ��, ��,
			/// 3�� 2���� �� �����Դϴ�.
			/// </summary>
			static void cuboidModel(bool reset = false);
			inline Model() :id(0), length(0) {}
		private:
			inline Model(unsigned id, unsigned length) :id(id), length(length) {}
			const unsigned id;
			const unsigned length;
	};
}

#endif // !__OA_VERTEX_H__



