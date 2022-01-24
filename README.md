# OAGLE
OA's GL Engine\
쓰라고 만든 건 아니지만 전체 혹은 일부를 얼마든지 사용/수정/배포할 수 있습니다. 출처 표기 필수 (MIT license)

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
