/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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
			UniformCode uniform(const char* name, bool b)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, int i)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, float f)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec2& v2)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec3& v3)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const vec4& v4) const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec2& v2)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec3& v3)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const ivec4& v4)const;
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. ����� ��ġ�� ������ ���θ� ���� �����մϴ�.
			/// </summary>
			/// <returns>����� ��忡 ���Ͽ� ���� �ڵ带 �����մϴ�.</returns>
			UniformCode uniform(const char* name, const mat4& m4, bool tr = true)const;

			/// <summary>
			/// ���̴� ���α׷��� �ؽ�ó ������ �����ϴ�.
			/// </summary>
			/// <param name="tex">�ؽ�ó ��ü�Դϴ�.</param>
			/// <param name="idx">�׷��� ��ü�� ���Ͽ� �� ��° �ؽ�ó���� �����մϴ�.</param>
			void texture(unsigned tex, TexCode idx = TexCode::SURFACE0) const;

			/// <summary>
			/// ���̴� ���α׷��� id�� ����ϴ�.
			/// �������� ĸ��ȭ���� ���� �κ��� ����� �� �ֽ��ϴ�.
			/// </summary>
			inline unsigned getID() const { return id; };
			
			/// <summary>
			/// ���� �迭 ��ü�� �׸��ϴ�. ���� ������ uniform ���޺��� ���߿� ȣ���մϴ�.
			/// </summary>
			void draw(Mesh& m) const;
			
			/// <summary>
			/// ���� �迭 ��ü�� ���ε��մϴ�. ������ �޽ø� ���� �� �׸� �� �ణ ������ �� ���� ���Դϴ�.
			/// </summary>
			void bind(Mesh& m);

			/// <summary>
			/// ���ε��� ���� �迭 ��ü�� �׸��ϴ�. end�� begin���� ���� ��� �ƹ��͵� �׷����� �ʽ��ϴ�.
			/// </summary>
			/// <param name="begin">�迭 �ε����� �������Դϴ�. 0���� �����մϴ�.</param>
			/// <param name="count">�迭 �ε����� �����Դϴ�. �Է����� �ʴ� ��� �������������� ���ֵ˴ϴ�.</param>
			void draw(unsigned begin = 0, unsigned count = -1) const;

			/// <summary>
			/// ���� �迭 ��ü�� �׸��ϴ�. ���� ������ uniform ���޺��� ���߿� ȣ���մϴ�.
			/// </summary>
			/// <param name="name">���� map�� models�� ��� �ִ� �̸�</param>
			//void draw(const char* name);

		private:
			static Shader* usingShader;
			unsigned id;
			unsigned recentLen;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			
	};
}


#endif // !__OA_SHADER_H__
