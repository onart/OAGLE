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
		/// ��� ����� ���̺귯��(PortAudio)�� �ʱ�ȭ�մϴ�.
		/// </summary>
		static void init();
		/// <summary>
		/// ��� ����� ���̺귯��(PortAudio)�� ����� �����ϴ�.
		/// </summary>
		static void terminate();
		/// <summary>
		/// ������ ������ �����մϴ�. ������ 0~1�Դϴ�.
		/// </summary>
		static void setMasterVolume(float v);

		class Stream;
		/// <summary>
		/// PCM �����͸� ��� ����� �ҽ��Դϴ�. ����� ��� Stream ��ü�� �����˴ϴ�.
		/// �ε�/��ε�/����� �� �� �ֽ��ϴ�.
		/// </summary>
		class Source {
		public:
			/// <summary>
			/// �Ҹ� ���Ͽ��� ������ �ҷ��ɴϴ�. ������ ��ġ�� ��� ������ �̹� �ε��� ���� �����մϴ�.
			/// </summary>
			/// <param name="file">�ҷ��� ���� ���� �̸�(���)�Դϴ�.</param>
			/// <param name="name">���α׷� ������ ����� �����Դϴ�. �Է����� �ʴ� ��� ���� �̸� �״�� ���ϴ�.</param>
			/// <returns>�ҷ��� �ҽ��� �����͸� �����մϴ�.</returns>
			static Source* load(const std::string& file, const std::string& name = "");

			/// <summary>
			/// �ҷ��� ������ �޸𸮿��� �����մϴ�.
			/// </summary>
			static void drop(const std::string& name);

			/// <summary>
			/// �ҷ��� ������ ����մϴ�. ����� ��Ʈ���� �����Ͱ� ���ϵǸ�, �̸� �̿��� �ߴ�/�簳/������ �� �� �ֽ��ϴ�. ���� ���θ� ������ �� �ֽ��ϴ�.
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
		/// ���� ����ǰ� �ִ� �Ҹ��Դϴ�.
		/// �ߴ�/�簳/������ �� �� �ֽ��ϴ�.
		/// </summary>
		class Stream {
			friend class Source;
		public:
			/// <summary>
			/// ����ǰ� �ִ� �Ҹ��� �Ͻ������� ����ϴ�. ���� �ִ� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			void pause();
			/// <summary>
			/// ���� �ִ� �Ҹ��� �ٽ� ����մϴ�. ���� ���� ���� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			void resume();
			/// <summary>
			/// �Ҹ��� �����մϴ�. �Ҹ��ڿ� �����մϴ�.
			/// </summary>
			void end();
			/// <summary>
			/// �Ҹ��� ó������ �ٽ� ����մϴ�.
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
			
			std::vector<void*> chBuf;	// ch1: ��� ���� ������ ä������ ���׷��� ���� ���� ä��(Ȥ�� ������ġ ���� ������ ����). ch2: ��� ���� �̻��, ���׷��� ���� ���� ä��
		};
	private:
		
		static std::map<std::string, Source*> source;
		static float master;
	};
}

#endif // !__OA_AUDIO_H__
