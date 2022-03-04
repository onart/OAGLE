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
#include <map>
#include <vector>

namespace onart {

	class Mesh;
	/// <summary>
	/// ����(vertex) ���̴� - ����(fragment) ���̴��� ����� ���α׷�
	/// <para>�� Ŭ������ ������ �������� ������� �ʽ��ϴ�.</para>
	/// <para>�� ������ ���� ���� Ȥ�� �̿��� �Ϸ��� ���, uniform ������ const char* ���ͷ� ��� Ȥ�� ���� ��� ������ ����� ĳ�̵� �� ������ ����ϱ� �ٶ��ϴ�.
	/// std::string ��� �����Ҵ�� �ּҸ� ������� ��� �۵��� ������ ������ ���ɴ�� �������� �������� �޸� ������ ���ɼ��� �����ϴ�.</para>
	/// �� ������Ʈ������ GLSL ���̴� ���� ������ �迭�� ���� ĳ���� ���� �������� �ʰڽ��ϴ�. ���� ��� shader["var"][2][1] ���� ��� �ݵ�� -1�� ��ġ�� �����ϴ�(=����).
	/// �̶��� �Ϲ� uniform ����ó�� �̷� ������ �� �ֽñ� �ٶ��ϴ�. 
	/// <para>shader.uniform("var[1][2]",1); or shader["var[1]"][2]=1; or shader["var[1][2]"]=1;</para>
	/// (�� ��� �޸𸮸� ���� ������ ������ �� �׷��� �������� �޸𸮸� ���� ����մϴ�.)
	/// </summary>
	class Shader
	{

		public:
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
			void uniform(const char* name, bool b);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, int i);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, float f);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const vec2& v2);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const vec3& v3);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const vec4& v4);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const ivec2& v2);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const ivec3& v3);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const ivec4& v4);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�.
			/// </summary>
			void uniform(const char* name, const mat4& m4);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, bool b);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, int i);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, float f);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const vec2& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const vec3& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const vec4& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec2& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec3& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec4& v);
			/// <summary>
			/// ���̴� ���α׷��� uniform ������ �����ϴ�. (�迭 ����)
			/// </summary>
			void uniform(const char* name, size_t index, const mat4& m);

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
			void draw(unsigned begin = 0, unsigned count = -1);

			class _uniformSender_ {
				friend class Shader;
			public:
				void operator=(int);
				void operator=(float);
				void operator=(bool);
				void operator=(const vec2&);
				void operator=(const vec3&);
				void operator=(const vec4&);
				void operator=(const ivec2&);
				void operator=(const ivec3&);
				void operator=(const ivec4&);
				void operator=(const mat4&);
				_uniformSender_& operator[](size_t i);
			private:
				int loc = -1;
				const char* point = nullptr;
			}__uniformSender;

			/// <summary>
			/// uniform ������ ������ ���� ������ �������̽��Դϴ�. �̰Ͱ� uniform �Լ��� ���ɻ� ���̴� ��ǻ� �����Ƿ� ���� ��� ����ϸ� �˴ϴ�.
			/// <para>���: shader["var"]=value;</para>
			/// �ŵ� ���������� std::string�� c_str()�Լ��� ����Ͽ� ������ ��� ���ɰ� �޸� �鿡�� ��� �������Ƿ� ���� �׷��� ������. (���ͷ��� �ּ������� ���� ����� �������ϴ�)
			/// </summary>
			/// <param name="name">uniform ���� �̸�(�迭 �̸��� �����ϰ� �迭 �ε������� �ִ� �͵� ����)</param>
			_uniformSender_& operator[](const char* name);

		private:
			static Shader* usingShader;
			unsigned id;
			unsigned recentLen = 0;
			std::map<const char*, int> uLocs;
			std::map<const char*, std::vector<int>> uaLocs;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			int getUloc(const char* name);
			int getUloc(const char* name, size_t index);
	};
}


#endif // !__OA_SHADER_H__
