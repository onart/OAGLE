#ifndef __OA_SHADER_H__
#define __OA_SHADER_H__

#include "oaglem.h"

namespace onart {

	class Mesh;
	/// <summary>
	/// ����(vertex) ���̴� - ����(fragment) ���̴��� ����� ���α׷�
	/// <para>�� Ŭ������ ������ �������� ������� �ʽ��ϴ�.</para>
	/// </summary>
	class Shader
	{

		public:
			/// <summary>
			/// ���̴� ������ ���� ����׿� �ڵ��Դϴ�.
			/// <para>SUCCESS: ����</para>
			/// <para>NOTFOUND: ���̴� ���α׷��� �ش� �̸��� �����ϴ�.</para>
			/// <para>INVALID: ���̴� ���α׷��� �ش� �̸��� ���Ͽ� ��ȿ���� ���� ���� ���޵Ǿ����ϴ�.</para>
			/// </summary>
			enum class UniformCode { SUCCESS, NOTFOUND, INVALID };

			/// <summary>
			/// �ؽ�ó ���޿� �ڵ��Դϴ�.
			/// <para>SURFACE0: �⺻ ǥ�� �̹����Դϴ�.</para>
			/// <para>ALPHA: ���� ����ũ �̹����Դϴ�.</para>
			/// <para>BUMP: ���� ������ ���� �̹����Դϴ�.</para>
			/// <para>SURFACE1: ���� ��ü�� ���Ͽ� �߰� �̹����� �ʿ��� ��� ����� �� �ֽ��ϴ�. ��, ������ ������ ���� �׷��� ����ϴ� ���� ��õ�մϴ�.</para>
			/// </summary>
			enum class TexCode { SURFACE0 = 0, ALPHA, BUMP, SURFACE1 };

			/// <summary>
			/// ��ũ��Ʈ ���Ͽ��� �о ���α׷��� �����մϴ�.
			/// </summary>
			/// <param name="vert">���� ���̴� ������ ����Դϴ�.</param>
			/// <param name="frag">���� ���̴� ������ ����Դϴ�.</param>
			void initWithFile(const char* vert, const char* frag);

			/// <summary>
			/// ���ڿ� ������ �о ���α׷��� �����մϴ�.
			/// </summary>
			/// <param name="vert">���� ���̴� ��ũ��Ʈ ���ڿ��Դϴ�.</param>
			/// <param name="frag">���� ���̴� ��ũ��Ʈ ���ڿ��Դϴ�.</param>
			void initWithMemory(const char* vert, const char* frag);

			/// <summary>
			/// ���̴� ����� �����մϴ�. ������ �������� ������� ������ �����Ͻñ� �ٶ��ϴ�.
			/// </summary>
			void use();

			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, bool b);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, int i);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, float f);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec2& v2);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec3& v3);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec4& v4);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec2& v2);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec3& v3);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec4& v4);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. ����� ��ġ�� ������ ���θ� ���� �����մϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const mat4& m4, bool tr = true);

			/// <summary>
			/// ���̴� ���α׷��� �ؽ�ó ������ �����ϴ�.
			/// </summary>
			/// <param name="tex">�ؽ�ó ��ü�Դϴ�.</param>
			/// <param name="idx">�׷��� ��ü�� ���Ͽ� �� ��° �ؽ�ó���� �����մϴ�.</param>
			void texture(unsigned tex, TexCode idx = TexCode::SURFACE0);

			/// <summary>
			/// ���̴� ���α׷��� id�� ����ϴ�.
			/// �������� ĸ��ȭ���� ���� �κ��� ����� �� �ֽ��ϴ�.
			/// </summary>
			inline unsigned getID() { return id; };

			/// <summary>
			/// ���� �迭 ��ü�� �׸��ϴ�. ���� ������ uniform ���޺��� ���߿� ȣ���մϴ�.
			/// </summary>
			void draw(Mesh& m);

			/// <summary>
			/// ���� �迭 ��ü�� �׸��ϴ�. ���� ������ uniform ���޺��� ���߿� ȣ���մϴ�.
			/// </summary>
			/// <param name="name">���� map�� models�� ��� �ִ� �̸�</param>
			//void draw(const char* name);

		private:
			static Shader* usingShader;
			unsigned id;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			
	};
}


#endif // !__OA_SHADER_H__
