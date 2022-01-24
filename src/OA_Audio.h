#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>
#include <vector>

struct AVFormatContext;


namespace onart {
	class Audio
	{
	public:
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
		/// PCM 데이터를 담는 오디오 소스입니다. 재생할 경우 Stream 객체가 생성됩니다.
		/// 로드/언로드/재생을 할 수 있습니다.
		/// </summary>
		class Source {
		public:
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
			/// </summary>
			Stream* play(bool loop = false);

		private:
			Source(AVFormatContext*, int sampleRate, int sampleCount, unsigned long sampleFormat, int channelCount);
			int sampleRate;
			int sampleCount;
			int channelCount;
			unsigned long sampleFormat;
			AVFormatContext* ctx;
			std::vector<Stream*> playing;
			~Source();
		};
		/// <summary>
		/// 현재 재생되고 있는 소리입니다.
		/// 중단/재개/정지를 할 수 있습니다.
		/// </summary>
		class Stream {
			friend class Source;
		public:
			/// <summary>
			/// 재생되고 있는 소리를 일시적으로 멈춥니다. 멈춰 있는 경우 아무것도 하지 않습니다.
			/// </summary>
			void pause();
			/// <summary>
			/// 멈춰 있는 소리를 다시 재생합니다. 멈춰 있지 않은 경우 아무것도 하지 않습니다.
			/// </summary>
			void resume();
			/// <summary>
			/// 소리를 정지합니다. 소멸자와 동일합니다.
			/// </summary>
			void end();
			/// <summary>
			/// 소리를 처음부터 다시 재생합니다.
			/// </summary>
			void restart();
			
			void* getChannel(int idx);
			inline bool isStopped() { return stopped; }
		private:
			Stream(AVFormatContext* src, int sampleRate, int sampleCount, unsigned long sampleFormat, int channelCount, bool loop);
			int nextFrame = 0;
			int sampleCount;
			int sampleRate;
			unsigned long sampleFormat;
			AVFormatContext* src;
			void* stream;
			bool stopped = false;
			bool loop;
			
			std::vector<void*> chBuf;	// ch1: 모노 기준 유일한 채널이자 스테레오 기준 좌측 채널(혹은 교차배치 기준 유일한 버퍼). ch2: 모노 기준 미사용, 스테레오 기준 우측 채널
		};
	private:
		
		static std::map<std::string, Source*> source;
		static float master;
	};
}

#endif // !__OA_AUDIO_H__
