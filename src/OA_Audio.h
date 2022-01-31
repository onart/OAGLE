#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>
#include <vector>

struct AVFormatContext;
struct AVCodecContext;
struct SwrContext;
struct PaStreamCallbackTimeInfo;

constexpr unsigned long RINGBUFFER_SIZE = 8820;

namespace onart {
	/// <summary>
	/// ������ ����ϴ� ����Դϴ�.
	/// �� ����� �� ������(OAGLE) ���Ͽ� �������Դϴ�. (��, ���� �� ��Ÿ ������Ʈ�� ����� ��⿡ ���������� �ʽ��ϴ�.)
	/// �� ����� �ٸ� ���� Ȱ���ϴ� ����� Source�� �����Ӵ� ���� �κ��� �ٸ� ������� ������ ���̸�,
	/// ����� Ȱ���� ���� ���� ��� irrKlang ���̺귯���� ��õ�մϴ�.
	/// </summary>
	class Audio
	{
	public:
		/// <summary>
		/// ���� �ܰ迡�� ȣ���� ���� �����ϴ�.
		/// </summary>
		static void update();
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
		/// ����� �����͸� �ҷ����� ����� �ҽ��Դϴ�. ����� ��� Stream ��ü�� �����˴ϴ�.
		/// �ε�/��ε�/����� �� �� �ֽ��ϴ�.
		/// 2ä��, �ʴ� 44100���� ���ڵ��� mp3 ������ ����ϴ� ���� ������ �����մϴ�.
		/// �׷��� �ʴ� ���, ��Ÿ�ӿ� ������ �����ø� ������ �Ź� ��ġ�� �Ǿ� �� ���� ������ ���� ���Դϴ�.
		/// ���� ��ȯ�� �����ϴ� �� ���񽺵� �����Ƿ� ����� �ʰ� �� �� �ֽ��ϴ�.
		/// </summary>
		class Source {
			friend class Stream;
			friend class Audio;
		public:
			/// <summary>
			/// �ҷ��� ������ �̸����� ã���ϴ�.
			/// </summary>
			static Source* get(const std::string& name);
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
			/// ���� ������ �ƴ� ��� �ڵ����� �޸𸮰� �����˴ϴ�.
			/// </summary>
			Stream* play(bool loop = false);

		private:
			Source(AVFormatContext*, SwrContext*, int);
			int getFrame(int frameNumber, float** dst);
			AVFormatContext* ctx;
			AVCodecContext* cdc;
			SwrContext* resampler = nullptr;
			int frameCount;
			std::vector<Stream*> playing;
			void update();
			~Source();
		};
		/// <summary>
		/// ���� ����ǰ� �ִ� �Ҹ��Դϴ�.
		/// �ߴ�/�簳/������ �� �� �ֽ��ϴ�.
		/// �ִ� 0.1���� �����̰� ���� �� �ֽ��ϴ�.
		/// </summary>
		class Stream {
			friend class Source;
		public:
			/// <summary>
			/// ����ǰ� �ִ� �Ҹ��� �Ͻ������� ����ϴ�. ���� �ִ� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			inline void pause() { stopped = true; }
			/// <summary>
			/// ���� �ִ� �Ҹ��� �ٽ� ����մϴ�. ���� ���� ���� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			inline void resume() { stopped = false; }
			/// <summary>
			/// �Ҹ��� �����մϴ�.
			/// </summary>
			void end();
			/// <summary>
			/// �Ҹ��� ó������ �ٽ� ����մϴ�. ���� �ִ� ��� ó������ ���ư����� �ٷ� ��������� �ʽ��ϴ�.
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
