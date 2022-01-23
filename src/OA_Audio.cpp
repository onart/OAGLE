#include "OA_Audio.h"

#include "externals/portaudio/portaudio.h"

extern "C" {
	#include "externals/ffmpeg/libavcodec/avcodec.h"
	#include "externals/ffmpeg/libavformat/avformat.h"
}


#ifdef _WIN32
	#ifdef _WIN64
		#pragma comment(lib, "externals/portaudio/portaudio_x64.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avformat.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avcodec.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avutil.lib")
	#else
		#pragma comment(lib, "externals/portaudio/portaudio_x86.lib")
	#endif // _WIN64
	
#endif

namespace onart {

	float Audio::master;

	/// <summary>
	/// "PCM"을 재생하도록 하는 콜백 함수
	/// </summary>
	static int paCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusFlags, void* userData) {
		
	}

	void Audio::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			printf("\n오디오 초기화에 실패했습니다.\n");
			printf("%s\n", Pa_GetErrorText(err));
		}
		master = 1;
	}

	void Audio::terminate() {
		PaError err = Pa_Terminate();
	}

	void Audio::setMasterVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		master = v;
	}

	Audio::Source* Audio::Source::load(const std::string& file, const std::string& name) {
		
	}

	Audio::Source::~Source() {
		delete[] ch1;
		delete[] ch2;
	}
}