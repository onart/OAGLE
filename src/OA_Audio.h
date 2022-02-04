#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>
#include <vector>
#include <cstdint>

struct AVFormatContext;
struct AVCodecContext;
struct SwrContext;
struct PaStreamCallbackTimeInfo;

constexpr unsigned long RINGBUFFER_SIZE = 8820;

namespace onart {
	/// <summary>
	/// 음성을 재생하는 모듈입니다.
	/// 이 모듈은 이 엔진에(OAGLE) 대하여 종속적입니다. (단, 엔진 및 기타 컴포넌트는 오디오 모듈에 종속적이지 않습니다.)
	/// 이 모듈을 다른 곳에 활용하는 방법은 Source의 프레임당 쓰기 부분을 다른 쓰레드로 돌리는 것이며,
	/// 상업적 활용을 하지 않을 경우 irrKlang 라이브러리를 추천합니다.
	/// </summary>
	class Audio
	{
	public:
		/// <summary>
		/// 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void update();
		/// <summary>
		/// 기반 오디오 라이브러리(PortAudio)를 초기화합니다.
		/// </summary>
		static void init();
		/// <summary>
		/// 기반 오디오 라이브러리(PortAudio)의 사용을 끝냅니다.
		/// </summary>
		static void terminate();
		/// <summary>
		/// 마스터 볼륨을 조정합니다. 범위는 0~1입니다.
		/// </summary>
		static void setMasterVolume(float v);

		class Stream;
		/// <summary>
		/// 오디오 데이터를 불러오는 오디오 소스입니다. 재생할 경우 Stream 객체가 생성됩니다.
		/// 로드/언로드/재생을 할 수 있습니다.
		/// 2채널, 초당 44100샘플 인코딩된 mp3 파일을 사용하는 것을 강력히 권장합니다.
		/// 그러지 않는 경우, 런타임에 별도의 리샘플링 과정을 매번 거치게 되어 더 낮은 성능을 보일 것입니다.
		/// 파일 변환은 제공하는 웹 서비스도 많으므로 어렵지 않게 할 수 있습니다.
		/// </summary>
		class Source {
			friend class Stream;
			friend class Audio;
		public:
			/// <summary>
			/// 불러온 음성을 이름으로 찾습니다.
			/// </summary>
			static Source* get(const std::string& name);
			/// <summary>
			/// 소리 파일에서 음성을 불러옵니다. 별명이 겹치는 경우 기존에 이미 로드한 것을 리턴합니다.
			/// </summary>
			/// <param name="file">불러올 음성 파일 이름(경로)입니다.</param>
			/// <param name="name">프로그램 내에서 사용할 별명입니다. 입력하지 않는 경우 파일 이름 그대로 들어갑니다.</param>
			/// <returns>불러온 소스의 포인터를 리턴합니다.</returns>
			static Source* load(const std::string& file, const std::string& name = "");

			/// <summary>
			/// 불러온 음성을 메모리에서 제거합니다.
			/// </summary>
			static void drop(const std::string& name);

			/// <summary>
			/// 불러온 음성을 재생합니다. 재생한 스트림의 포인터가 리턴되며, 이를 이용해 중단/재개/정지를 할 수 있습니다. 루프 여부를 선택할 수 있습니다.
			/// 루프 음성이 아닌 경우 자동으로 메모리가 해제됩니다.
			/// </summary>
			Stream* play(bool loop = false);
		private:
			Source(AVFormatContext*, AVCodecContext*, SwrContext*, int);
			int getFrame(int frameNumber, float** dst);
			AVFormatContext* ctx;
			AVCodecContext* cdc;
			SwrContext* resampler = nullptr;
			float volume = 1;
			int frameCount;
			std::vector<Stream*> playing;
			void update();
			~Source();
		};

		/// <summary>
		/// 현재 재생되고 있는 소리입니다.
		/// 중단/재개/정지를 할 수 있습니다.
		/// 최대 0.1초의 딜레이가 있을 수 있습니다. 딜레이가 훨씬 짧았으면 좋겠다면, RINGBUFFER_SIZE를 더 작게 조절해 주세요.
		/// </summary>
		class Stream {
			friend class Source;
		public:
			/// <summary>
			/// 현재 재생되고 있는 스트림 수입니다.
			/// </summary>
			static const int& activeCount;
			/// <summary>
			/// 재생되고 있는 소리를 일시적으로 멈춥니다. 멈춰 있는 경우 아무것도 하지 않습니다.
			/// </summary>
			void pause();
			/// <summary>
			/// 멈춰 있는 소리를 다시 재생합니다. 멈춰 있지 않은 경우 아무것도 하지 않습니다.
			/// </summary>
			void resume();
			/// <summary>
			/// 소리를 정지합니다.
			/// </summary>
			void end();
			/// <summary>
			/// 소리를 처음부터 다시 재생합니다. 멈춰 있는 경우 처음으로 돌아가지만 바로 재생되지는 않습니다.
			/// </summary>
			void restart();
		private:
			~Stream();
			Stream(Source* src, bool loop);
			bool update();
			int nextFrame = 0;
			Source* src;
			float* buffer = nullptr;
			unsigned long restSamples = 0;
			static int activeStreamCount;
			bool stopped = false;
			bool loop;
		};
	private:
		static int sampleRate;
		static std::map<std::string, Source*> source;
		static float master;
		static void* masterStream;
		static int playCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData);
	};
}

#endif // !__OA_AUDIO_H__
