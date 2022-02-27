/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <memory>

struct AVFormatContext;
struct AVCodecContext;
struct AVIOContext;
struct SwrContext;
struct PaStreamCallbackTimeInfo;

constexpr unsigned long RINGBUFFER_SIZE = 8820;	// 사운드 재생/정지 반영의 딜레이와 관련되어 있습니다. 단독 수정이 가능합니다.
constexpr int STD_SAMPLE_RATE = 44100;	// 음질과 프로그램 성능에 관련되어 있습니다. 단독 수정이 가능합니다.

constexpr bool OA_AUDIO_NOTHREAD = false;	// 어떤 이유든 오디오 모듈이 스레드를 생성하기 원하지 않는 경우 true로 설정해 주세요. 그러면 스레드 대신 프레임 타임에 오디오 내용을 읽습니다.
constexpr bool OA_AUDIO_WAIT_ON_DRAG = false;	// NOTHREAD 상수가 true이며 이것도 true인 경우, 창을 잡고 있는 등의 윈도우 메시지 입력이 오래 지속될 경우 소리가 정지합니다.

namespace onart {
	/// <summary>
	/// 음성을 재생하는 모듈입니다.
	/// 이 모듈은 이 엔진에(OAGLE) 대하여 종속적입니다. (단, 엔진 및 기타 컴포넌트는 오디오 모듈에 종속적이지 않습니다.)
	/// 이 모듈을 다른 곳에 활용하려면 main.cpp의 init(), finalize()와 메인 for루프 부분을 잘 참고해 주세요.
	/// 상업적 활용을 하지 않을 경우 irrKlang 라이브러리를 추천합니다.
	/// 이 클래스는 init()호출 시 1개의 스레드를 생성합니다. 헤더의 상수 OA_AUDIO_NOTHREAD를 사용하는 경우 별도의 스레드를 생성하지 않지만 프레임 타임 증가 우려가 있으며,
	/// (창 끌기 시 버퍼의 내용이 그대로 반복재생되는 문제, 가끔 소리가 끊기는 문제) 중 하나를 선택해야 합니다.
	/// </summary>
	class Audio
	{
	public:
		/// <summary>
		/// 창을 드래그할 때 소리를 멈추기 위한 변수입니다. 응용 계층에서 사용하지 마시기 바랍니다.
		/// 전체 소리를 멈추기 위해 이를 사용할 경우, 프레임 시작과 동시에 정지가 풀리게 됩니다.
		/// </summary>
		static bool wait;
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
		static void allow(bool);

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
			static std::shared_ptr<Source> get(const std::string& name);

			/// <summary>
			/// 소리 파일에서 음성을 불러옵니다. 별명이 겹치는 경우 기존에 이미 로드한 것을 리턴합니다.
			/// </summary>
			/// <param name="file">불러올 음성 파일 이름(경로)입니다.</param>
			/// <param name="name">프로그램 내에서 사용할 별명입니다. 입력하지 않는 경우 파일 이름 그대로 들어갑니다.</param>
			/// <returns>불러온 소스의 포인터를 리턴합니다.</returns>
			static std::shared_ptr<Source> load(const std::string& file, const std::string& name = "");

			/// <summary>
			/// 메모리에서 음성을 불러옵니다. 별명이 겹치는 경우 기존에 이미 로드한 것을 리턴합니다.
			/// </summary>
			/// <param name="mem">파일에 해당하는 메모리의 시작 주소입니다.</param>
			/// <param name="size">파일의 크기(바이트)입니다.</param>
			/// <param name="name">프로그램 내에서 사용할 별명이며, 필수적으로 지정해야 합니다.</param>
			static std::shared_ptr<Source> load(const void* mem, size_t size, const std::string& name);

			/// <summary>
			/// 불러온 음성을 메모리에서 제거합니다. 해당 이름의 음원이 없거나 다른 곳에서 사용 중인 경우 사용 종료 즉시 메모리가 회수됩니다.
			/// "사용"은 "재생"과는 무관하게 음원 포인터를 보유하고 있는 객체가 있으면 사용하는 것으로 칩니다. 객체/포인터를 소멸시키지 않고 사용을 종료하려면
			/// 해당 음원 포인터로 reset() 또는 swap(pAudioSource()) 등을 호출하면 됩니다.
			/// 음원의 포인터를 가지고 재생했지만 그 포인터를 따로 멤버 등으로 유지하지 않고 소멸했을 경우, drop으로 음원이 소멸된다면 즉시 스트림도 모두 함께 소멸됩니다.
			/// </summary>
			/// <param name="name">제거할 음원 이름</param>
			static void drop(const std::string& name);

			/// <summary>
			/// 불러온 모든 음원 중 현재 사용되고 있지 않은 것을 모두 메모리에서 제거합니다.
			/// 사용 중인 음원은 사용이 끝나는 즉시 메모리에서 회수할지, 그대로 남겨둘지 선택할 수 있습니다.
			/// "사용"은 "재생"과는 무관하게 음원 포인터를 보유하고 있는 객체가 있으면 사용하는 것으로 칩니다. 객체/포인터를 소멸시키지 않고 사용을 종료하려면
			/// 해당 음원 포인터로 reset() 또는 swap(pAudioSource()) 등을 호출하면 됩니다.
			/// </summary>
			/// <param name="removeUsing">true인 경우 사용 중인 음원의 모든 사용이 끝나는 즉시 메모리에서 회수됩니다.</param>
			static void collect(bool removeUsing = false);

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
		static std::map<std::string, std::shared_ptr<Source>> source;
		static float master;
		static void* masterStream;
		static int playCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData);
	};

	using pAudioSource = std::shared_ptr<Audio::Source>;	// 음원의 포인터입니다.
}

#endif // !__OA_AUDIO_H__
