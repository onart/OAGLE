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
	/// 정점(vertex) 셰이더 - 조각(fragment) 셰이더가 연결된 프로그램
	/// <para>이 클래스는 스레드 안전성이 보장되지 않습니다.</para>
	/// <para>이 계층에 대한 수정 혹은 이용을 하려는 경우, uniform 변수는 const char* 리터럴 상수 혹은 전역 상수 정도만 제대로 캐싱될 수 있음을 명심하길 바랍니다.
	/// std::string 등에서 동적할당된 주소를 끌어오는 경우 작동은 하지만 성능은 성능대로 나빠지고 쓸데없는 메모리 차지의 가능성이 높습니다.</para>
	/// 이 프로젝트에서는 GLSL 셰이더 내의 다차원 배열에 대한 캐싱을 직접 지원하지 않겠습니다. 예를 들어 shader["var"][2][1] 같은 경우 반드시 -1의 위치로 보냅니다(=실패).
	/// 이때는 일반 uniform 변수처럼 이런 식으로 해 주시기 바랍니다. 
	/// <para>shader.uniform("var[1][2]",1); or shader["var[1]"][2]=1; or shader["var[1][2]"]=1;</para>
	/// (이 경우 메모리를 많이 먹지만 어차피 안 그래도 다차원은 메모리를 많이 사용합니다.)
	/// </summary>
	class Shader
	{

		public:
			/// <summary>
			/// 텍스처 전달용 코드입니다.
			/// <para>SURFACE0: 기본 표면 이미지입니다.</para>
			/// <para>ALPHA: 투명도 마스크 이미지입니다.</para>
			/// <para>BUMP: 법선 매핑을 위한 이미지입니다.</para>
			/// <para>SURFACE1: 단일 개체에 대하여 추가 이미지가 필요한 경우 사용할 수 있습니다. 단, 가급적 별개의 정점 그룹을 사용하는 것을 추천합니다.</para>
			/// </summary>
			enum class TexCode { SURFACE0 = 0, ALPHA, BUMP, SURFACE1 };

			/// <summary>
			/// 스크립트 파일에서 읽어서 프로그램을 생성합니다.
			/// </summary>
			/// <param name="vert">정점 셰이더 파일의 경로입니다.</param>
			/// <param name="frag">조각 셰이더 파일의 경로입니다.</param>
			void initWithFile(const char* vert, const char* frag);

			/// <summary>
			/// 문자열 변수를 읽어서 프로그램을 생성합니다.
			/// </summary>
			/// <param name="vert">정점 셰이더 스크립트 문자열입니다.</param>
			/// <param name="frag">조각 셰이더 스크립트 문자열입니다.</param>
			void initWithMemory(const char* vert, const char* frag);

			/// <summary>
			/// 셰이더 사용을 시작합니다. 스레드 안전성이 보장되지 않음에 유의하시기 바랍니다.
			/// </summary>
			void use();

			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, bool b);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, int i);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, float f);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const vec2& v2);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const vec3& v3);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const vec4& v4);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const ivec2& v2);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const ivec3& v3);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const ivec4& v4);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			void uniform(const char* name, const mat4& m4);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, bool b);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, int i);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, float f);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const vec2& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const vec3& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const vec4& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec2& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec3& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const ivec4& v);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. (배열 버전)
			/// </summary>
			void uniform(const char* name, size_t index, const mat4& m);

			/// <summary>
			/// 셰이더 프로그램에 텍스처 정보를 보냅니다.
			/// </summary>
			/// <param name="tex">텍스처 객체입니다.</param>
			/// <param name="idx">그려질 객체에 대하여 몇 번째 텍스처일지 결정합니다.</param>
			void texture(unsigned tex, TexCode idx = TexCode::SURFACE0);

			/// <summary>
			/// 셰이더 프로그램의 id를 얻습니다.
			/// 엔진에서 캡슐화되지 않은 부분을 사용할 수 있습니다.
			/// </summary>
			inline unsigned getID() const { return id; };
			
			/// <summary>
			/// 정점 배열 객체를 그립니다. 엔진 구조상 uniform 전달보다 나중에 호출합니다.
			/// </summary>
			void draw(Mesh& m) const;
			
			/// <summary>
			/// 정점 배열 객체를 바인드합니다. 동일한 메시를 여러 번 그릴 때 약간 성능이 더 높을 것입니다.
			/// </summary>
			void bind(Mesh& m);

			/// <summary>
			/// 바인드한 정점 배열 객체를 그립니다. end가 begin보다 작은 경우 아무것도 그려지지 않습니다.
			/// </summary>
			/// <param name="begin">배열 인덱스의 시작점입니다. 0부터 시작합니다.</param>
			/// <param name="count">배열 인덱스의 개수입니다. 입력하지 않는 경우 마지막점까지로 간주됩니다.</param>
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
			/// uniform 변수를 보내기 위한 직관적 인터페이스입니다. 이것과 uniform 함수의 성능상 차이는 사실상 없으므로 편한 대로 사용하면 됩니다.
			/// <para>용법: shader["var"]=value;</para>
			/// 거듭 강조하지만 std::string의 c_str()함수를 사용하여 대입할 경우 성능과 메모리 면에서 모두 나빠지므로 절대 그러지 마세요. (리터럴이 최선이지만 전역 상수도 괜찮습니다)
			/// </summary>
			/// <param name="name">uniform 변수 이름(배열 이름도 가능하고 배열 인덱스까지 주는 것도 가능)</param>
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
