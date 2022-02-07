#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>
#include <vector>
#include <cstdint>

struct AVFormatContext;
struct AVCodecContext;
struct AVIOContext;
struct SwrContext;
struct PaStreamCallbackTimeInfo;

constexpr unsigned long RINGBUFFER_SIZE = 8820;	// 사운드 재생/정지 반영의 딜레이와 관련되어 있습니다. 단독 수정이 가능합니다.
constexpr int STD_SAMPLE_RATE = 44100;	// 음질과 프로그램 성능에 관련되어 있습니다. 단독 수정이 가능합니다.

constexpr bool OA_AUDIO_NOTHREAD = false;

namespace onart {
	/// <summary>
	/// 음성을 재생하는 모듈입니다.
	/// 이 모듈은 이 엔진에(OAGLE) 대하여 종속적입니다. (단, 엔진 및 기타 컴포넌트는 오디오 모듈에 종속적이지 않습니다.)
	/// 이 모듈을 다른 곳에 활용하는 방법은 Source의 프레임당 쓰기 부분을 다른 쓰레드로 돌리는 것이며,
	/// 상업적 활용을 하지 않을 경우 irrKlang 라이브러리를 추천합니다.
	/// 소스 파일에 OA_AUDIO_WAIT_ON_DRAG 매크로를 정의할 경우, 창을 드래그할 때 소리가 멈추지만 가끔 소리가 아주 잠깐씩 끊깁니다. 둘 모두를 원하지 않는다면 위에서 나온 것과 같이
	/// main.cpp의 onart::Audio::update() 부분을 다른 쓰레드로 돌리면 됩니다.
	/// </summary>
	class Audio
	{
	public:
#ifdef OA_AUDIO_WAIT_ON_DRAG
		/// <summary>
		/// 이 엔진의 오디오 모듈은 스레드를 사용하지 않으므로, 창을 드래그할 때 소리를 멈추기 위한 변수입니다. 응용 계층에서 사용하지 마시기 바랍니다.
		/// 전체 소리를 멈추기 위해 이를 사용할 경우, 프레임 시작과 동시에 정지가 풀리게 됩니다.
		/// </summary>
		static bool wait;
#endif
		/// <summary>
		/// 읽기 전용 마스터 볼륨입니다.
		/// </summary>
		static const float& masterVolume;
		/// <summary>
		/// 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void update();
		/// <summary>
		/// 오디오 사용을 시작합니다. 호출되지 않으면 소리 관련 기능을 사용할 수 없습니다. 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void init();
		/// <summary>
		/// 오디오 사용을 종료합니다. 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void terminate();
		/// <summary>
		/// 마스터 볼륨을 조정합니다. 범위는 0~1입니다.
		/// </summary>
		static void setMasterVolume(float v);
		/// <summary>
		/// 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void allow();
		/// <summary>
		/// 응용 단계에서 호출할 일이 없습니다.
		/// </summary>
		static void acquire();
		class Stream;

		/// <summary>
		/// 오디오 데이터를 불러오는 오디오 소스입니다. 재생할 경우 Stream 객체가 생성됩니다.
		/// 로드, 언로드, 재생을 할 수 있습니다.
		/// 32비트 부동소수점, 초당 44100샘플, 스테레오로 자동으로 맞춰집니다. 특히 여러 음원을 동시 재생하는 경우, STD_SAMPLE_RATE를 내림으로써 성능 향상을 도모할 수 있습니다.
		/// </summary>
		class Source {
			friend class Stream;
			friend class Audio;
		public:
			/// <summary>
			/// 소스의 볼륨을 조절합니다. 소스 자체의 볼륨을 조정하는 것이 성능상 좋지만, 사용자가 원하는 대로 조절할 수 있게 하려면 이 함수를 사용해야 합니다.
			/// 배경음악/효과음악 등의 분류로 일괄 조정하는 것은 응용 계층 몫입니다.
			/// </summary>
			/// <param name="v">0과 1 사이의 값만 가능하며 그 외의 값 입력 시 강제로 맞춰집니다.</param>
			void setVolume(float v);

			/// <summary>
			/// 불러온 음원의 포인터를 획득합니다. 잘못된 이름을 입력한 경우 nullptr를 리턴합니다.
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
			/// 메모리에서 음성을 불러옵니다. 별명이 겹치는 경우 기존에 이미 로드한 것을 리턴합니다.
			/// </summary>
			/// <param name="mem">파일에 해당하는 메모리의 시작 주소입니다.</param>
			/// <param name="size">파일의 크기(바이트)입니다.</param>
			/// <param name="name">프로그램 내에서 사용할 별명이며, 필수적으로 지정해야 합니다.</param>
			static Source* load(const void* mem, size_t size, const std::string& name);

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
			struct MemorySource {
				const unsigned char* dat;
				unsigned char* buf = nullptr;
				uint64_t cursor = 0;
				uint64_t size;
				MemorySource(const void* dat, uint64_t size);
				~MemorySource();
				int request(int req, unsigned char* buf);
				int64_t seek(int64_t pos, int origin);
			}*memsrc;

			static bool initDemux(AVFormatContext*);
			static int readMem(void* ptr, uint8_t* buf, int len);
			static int64_t seekMem(void* ptr, int64_t pos, int origin);

			Source(AVFormatContext*, AVCodecContext*, SwrContext*, int, AVIOContext* io = nullptr, MemorySource* ms = nullptr);
			int getFrame(int frameNumber, float** dst);
			AVFormatContext* ctx;
			AVCodecContext* cdc;
			AVIOContext* ioctx = nullptr;
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
		static bool noup;
		static void audioThread();
		static std::map<std::string, Source*> source;
		static float master;
		static void* masterStream;
		static int playCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData);
	};
}

#endif // !__OA_AUDIO_H__
