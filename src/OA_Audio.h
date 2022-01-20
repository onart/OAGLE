#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include "externals/portaudio/portaudio.h"
#ifdef _WIN32
	#ifdef _WIN64
		#pragma comment(lib, "external/portaudio/portaudio_x64.lib")
	#else
		#pragma comment(lib, "external/portaudio/portaudio_x86.lib")
	#endif // _WIN64
	
#endif

namespace onart {
	class Audio
	{
	public:
		
	private:
	};
}

#endif // !__OA_AUDIO_H__
