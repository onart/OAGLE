#ifndef __OA_SHADER_H__
#define __OA_SHADER_H__

#include "oaglem.h"

namespace onart {

	/// <summary>
	/// 정점(vertex) 셰이더 - 조각(fragment) 셰이더가 연결된 프로그램
	/// <para>이 클래스는 스레드 안전성이 보장되지 않습니다.</para>
	/// </summary>
	class Shader
	{
		public:
			enum class UniformCode { SUCCESS, NOTFOUND, INVALID };
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
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, bool b);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, int i);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, float f);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const vec2& v2);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const vec3& v3);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const vec4& v4);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const ivec2& v2);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const ivec3& v3);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const ivec4& v4);
			/// <summary>
			/// 셰이더 프로그램에 uniform 변수를 보냅니다. 행렬을 전치로 보낼지 여부를 직접 선택합니다.
			/// </summary>
			/// <returns>디버그 모드에 한하여 실패 코드를 리턴합니다.</returns>
			UniformCode uniform(const char* name, const mat4& m4, bool tr = true);

		private:
			static Shader* usingShader;
			unsigned id = 0;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			
	};
}


#endif // !__OA_SHADER_H__
