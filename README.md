# OAGLE
OA's GL Engine\
쓰라고 만든 건 아니지만 전체 혹은 일부를 얼마든지 사용/수정/배포할 수 있습니다. 출처 표기 필수 (MIT license)

## Overview
이것은 C++, OpenGL 기반 게임 엔진 프로젝트입니다.
* 대상 플랫폼: x86 계열 프로세서, Windows 10\
대상 운영체제의 경우 DLL/SO 연결 부분만 조정하면 그대로 컴파일될 수 있도록 외부 라이브러리와 본 프로젝트의 코드 모두 크로스 플랫폼에 해당하는 코드만 사용되었습니다. 단, Makefile은 제공되지 않습니다.
프로세서는 SIMD를 사용하는 기능(oagle_simd.h에만 존재)과 DLL/SO 연결 부분을 조정해야 합니다.
GL ES(임베디드 시스템)에 대한 지원은 이루어지지 않습니다.

* 기능(확장 중)\
음원 재생\
2D 스프라이트/3D 모델 및 애니메이션 표시\
카메라 관련 모듈\
입력 지원\
수학 및 난수 관련 모듈\
씬 시스템(확장 중)\
텍스트 렌더링(예정)\
2D/3D 물리/충돌(예정)

* 셰이더\
Blinn-Phong\
단일광원\
노말매핑(예정)

* 문서\
PDF파일, Github Wiki를 준비중입니다.


## related source
* https://github.com/onart/OAGLE/wiki#%EB%9D%BC%EC%9D%B4%EC%84%BC%EC%8A%A4
