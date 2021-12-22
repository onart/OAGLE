#ifndef __OA_SHADER_H__
#define __OA_SHADER_H__

#include "oaglem.h"

namespace onart {

	class Mesh;
	/// <summary>
	/// 정점(vertex) 셰이더 - 조각(fragment) 셰이더가 연결된 프로그램
	/// <para>이 클래스는 스레드 안전성이 보장되지 않습니다.</para>
	/// </summary>
	class Shader
	{

		public:
			/// <summary>
			/// 셰이더 유니폼 변수 디버그용 코드입니다.
			/// <para>SUCCESS: 성공</para>
			/// <para>NOTFOUND: 셰이더 프로그램에 해당 이름이 없습니다.</para>
			/// <para>INVALID: 셰이더 프로그램에 해당 이름에 대하여 유효하지 않은 값이 전달되었습니다.</para>
			/// </summary>
			enum class UniformCode { SUCCESS, NOTFOUND, INVALID };

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
			inline unsigned getID() { return id; };

			/// <summary>
			/// 정점 배열 객체를 그립니다. 엔진 구조상 uniform 전달보다 나중에 호출합니다.
			/// </summary>
			void draw(Mesh& m);

			/// <summary>
			/// 정점 배열 객체를 그립니다. 엔진 구조상 uniform 전달보다 나중에 호출합니다.
			/// </summary>
			/// <param name="name">전역 map인 models에 들어 있는 이름</param>
			//void draw(const char* name);

		private:
			static Shader* usingShader;
			unsigned id;

			unsigned compileScript(const char* script, unsigned type);
			bool validateShaderProgram();
			
	};
}


#endif // !__OA_SHADER_H__
