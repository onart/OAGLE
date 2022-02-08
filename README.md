# OAGLE
OA's GL Engine\
쓰라고 만든 건 아니지만 전체 혹은 일부를 얼마든지 사용/수정/배포할 수 있습니다. 출처 표기 필수 (MIT license)

## Overview
이것은 OpenGL 기반 게임 엔진 프로젝트입니다.
* 대상 플랫폼: x86 계열 프로세서, Windows 10
대상 운영체제의 경우 DLL/SO 연결 부분만 조정하면 그대로 컴파일될 수 있도록 외부 라이브러리와 본 프로젝트의 코드 모두 크로스 플랫폼에 해당하는 코드만 사용되었습니다. 단, Makefile은 제공되지 않습니다.
프로세서는 SIMD를 사용하는 기능(oagle_simd.h에만 존재)과 DLL/SO 연결 부분을 조정해야 합니다.
GL ES(임베디드 시스템)에 대한 지원은 이루어지지 않습니다.

* 기능(확장 중)
음원 재생\
2D 스프라이트/3D 모델 및 애니메이션 표시\
카메라 관련 모듈\
입력 지원\
수학 및 난수 관련 모듈\
씬 시스템(확장 중)\
텍스트 렌더링(예정)\
2D/3D 물리/충돌(예정)

* 셰이더
Blinn-Phong
단일광원
노말매핑(예정)


## related source
* ASSIMP - https://assimp.org/ \
(수정 BSD 라이센스: 제품 안에 ASSIMP - you may use the library for free, in commercial or non-commercial applications, but you must include our license with your product and you may not advertise with us. 명시)
* stb_image.h - https://github.com/nothings/stb/blob/master/stb_image.h \
(퍼블릭 도메인, MIT 라이센스 중 택 1)
* GLFW - https://www.glfw.org/ \
(zlib/libpng 라이센스: 원본 배포 시 라이센스와 함께 / 변경 시 그 사실을 명시 / 원본을 자신이 작성했다는 명시 금지 / acknowledgement 선택사항)
* GLAD - https://glad.dav1d.de/ \
(GLAD 소스: MIT 라이센스, Khronos 스펙: Apache(파일 내 출처 표기), EGL 스펙 등: 원저작자 책임 없음)
* PortAudio - http://www.portaudio.com/ \
(라이센스: 자유 사용, 원저자 책임 없음, 코드에 저작권자 표시 유지)
* FFmpeg - https://www.ffmpeg.org/ \
(라이센스: LGPL 2.1+: ffmpeg의 코드를 수정하지 않은 경우 직접 만든 코드 공개 의무 없음, 바이너리(dll과 lib) 재배포 시 다음 내용을 포함) \
ffmpeg에서 배포하는 소스를 가지고 있는 상태에서 x64(또는 x86) native tools command prompt for VS 2022에서 MSYS2 실행(use-full-path), cl을 이용해 다음과 같이 구성. \
configure(실제 configure 경로) --toolchain=msvc --arch=x86 --disable-programs --disalbe-static --enable-shared \
ffmpeg 원본 소스 공개의 의무도 있는데 링크로 대체 (실제 사용 버전: 2022.1.21 13:12) https://github.com/FFmpeg/FFmpeg/tree/7247a6fed8de9c2162ed408682e095f0b7f19350
