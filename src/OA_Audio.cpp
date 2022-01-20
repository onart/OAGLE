#include "OA_Audio.h"

#include "externals/portaudio/portaudio.h"

#ifdef _WIN32
	#ifdef _WIN64
		#pragma comment(lib, "externals/portaudio/portaudio_x64.lib")
	#else
		#pragma comment(lib, "externals/portaudio/portaudio_x86.lib")
	#endif // _WIN64
	
#endif

namespace onart {
	void Audio::init() {
		auto err = Pa_Initialize();
	}
}