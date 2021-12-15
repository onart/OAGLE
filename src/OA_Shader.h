#ifndef __OA_SHADER_H__
#define __OA_SHADER_H__

#include "oaglem.h"

namespace onart {

	/// <summary>
	/// ����(vertex) ���̴� - ����(fragment) ���̴��� ����� ���α׷�
	/// <para>�� Ŭ������ ������ �������� ������� �ʽ��ϴ�.</para>
	/// </summary>
	class Shader
	{
		public:
			enum class UniformCode { SUCCESS, NOTFOUND, INVALID };
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

		private:
			static Shader* usingShader;
			unsigned id = 0;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			
	};
}


#endif // !__OA_SHADER_H__
