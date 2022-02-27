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

constexpr unsigned long RINGBUFFER_SIZE = 8820;	// ���� ���/���� �ݿ��� �����̿� ���õǾ� �ֽ��ϴ�. �ܵ� ������ �����մϴ�.
constexpr int STD_SAMPLE_RATE = 44100;	// ������ ���α׷� ���ɿ� ���õǾ� �ֽ��ϴ�. �ܵ� ������ �����մϴ�.

constexpr bool OA_AUDIO_NOTHREAD = false;	// � ������ ����� ����� �����带 �����ϱ� ������ �ʴ� ��� true�� ������ �ּ���. �׷��� ������ ��� ������ Ÿ�ӿ� ����� ������ �н��ϴ�.
constexpr bool OA_AUDIO_WAIT_ON_DRAG = false;	// NOTHREAD ����� true�̸� �̰͵� true�� ���, â�� ��� �ִ� ���� ������ �޽��� �Է��� ���� ���ӵ� ��� �Ҹ��� �����մϴ�.

namespace onart {
	/// <summary>
	/// ������ ����ϴ� ����Դϴ�.
	/// �� ����� �� ������(OAGLE) ���Ͽ� �������Դϴ�. (��, ���� �� ��Ÿ ������Ʈ�� ����� ��⿡ ���������� �ʽ��ϴ�.)
	/// �� ����� �ٸ� ���� Ȱ���Ϸ��� main.cpp�� init(), finalize()�� ���� for���� �κ��� �� ������ �ּ���.
	/// ����� Ȱ���� ���� ���� ��� irrKlang ���̺귯���� ��õ�մϴ�.
	/// �� Ŭ������ init()ȣ�� �� 1���� �����带 �����մϴ�. ����� ��� OA_AUDIO_NOTHREAD�� ����ϴ� ��� ������ �����带 �������� ������ ������ Ÿ�� ���� ����� ������,
	/// (â ���� �� ������ ������ �״�� �ݺ�����Ǵ� ����, ���� �Ҹ��� ����� ����) �� �ϳ��� �����ؾ� �մϴ�.
	/// </summary>
	class Audio
	{
	public:
		/// <summary>
		/// â�� �巡���� �� �Ҹ��� ���߱� ���� �����Դϴ�. ���� �������� ������� ���ñ� �ٶ��ϴ�.
		/// ��ü �Ҹ��� ���߱� ���� �̸� ����� ���, ������ ���۰� ���ÿ� ������ Ǯ���� �˴ϴ�.
		/// </summary>
		static bool wait;
		/// <summary>
		/// �б� ���� ������ �����Դϴ�.
		/// </summary>
		static const float& masterVolume;
		/// <summary>
		/// ���� �ܰ迡�� ȣ���� ���� �����ϴ�.
		/// </summary>
		static void update();
		/// <summary>
		/// ����� ����� �����մϴ�. ȣ����� ������ �Ҹ� ���� ����� ����� �� �����ϴ�. ���� �ܰ迡�� ȣ���� ���� �����ϴ�.
		/// </summary>
		static void init();
		/// <summary>
		/// ����� ����� �����մϴ�. ���� �ܰ迡�� ȣ���� ���� �����ϴ�.
		/// </summary>
		static void terminate();
		/// <summary>
		/// ������ ������ �����մϴ�. ������ 0~1�Դϴ�.
		/// </summary>
		static void setMasterVolume(float v);
		/// <summary>
		/// ���� �ܰ迡�� ȣ���� ���� �����ϴ�.
		/// </summary>
		static void allow(bool);

		class Stream;

		/// <summary>
		/// ����� �����͸� �ҷ����� ����� �ҽ��Դϴ�. ����� ��� Stream ��ü�� �����˴ϴ�.
		/// �ε�, ��ε�, ����� �� �� �ֽ��ϴ�.
		/// 32��Ʈ �ε��Ҽ���, �ʴ� 44100����, ���׷����� �ڵ����� �������ϴ�. Ư�� ���� ������ ���� ����ϴ� ���, STD_SAMPLE_RATE�� �������ν� ���� ����� ������ �� �ֽ��ϴ�.
		/// </summary>
		class Source {
			friend class Stream;
			friend class Audio;
		public:
			/// <summary>
			/// �ҽ��� ������ �����մϴ�. �ҽ� ��ü�� ������ �����ϴ� ���� ���ɻ� ������, ����ڰ� ���ϴ� ��� ������ �� �ְ� �Ϸ��� �� �Լ��� ����ؾ� �մϴ�.
			/// �������/ȿ������ ���� �з��� �ϰ� �����ϴ� ���� ���� ���� ���Դϴ�.
			/// </summary>
			/// <param name="v">0�� 1 ������ ���� �����ϸ� �� ���� �� �Է� �� ������ �������ϴ�.</param>
			void setVolume(float v);

			/// <summary>
			/// �ҷ��� ������ �����͸� ȹ���մϴ�. �߸��� �̸��� �Է��� ��� nullptr�� �����մϴ�.
			/// </summary>
			static std::shared_ptr<Source> get(const std::string& name);

			/// <summary>
			/// �Ҹ� ���Ͽ��� ������ �ҷ��ɴϴ�. ������ ��ġ�� ��� ������ �̹� �ε��� ���� �����մϴ�.
			/// </summary>
			/// <param name="file">�ҷ��� ���� ���� �̸�(���)�Դϴ�.</param>
			/// <param name="name">���α׷� ������ ����� �����Դϴ�. �Է����� �ʴ� ��� ���� �̸� �״�� ���ϴ�.</param>
			/// <returns>�ҷ��� �ҽ��� �����͸� �����մϴ�.</returns>
			static std::shared_ptr<Source> load(const std::string& file, const std::string& name = "");

			/// <summary>
			/// �޸𸮿��� ������ �ҷ��ɴϴ�. ������ ��ġ�� ��� ������ �̹� �ε��� ���� �����մϴ�.
			/// </summary>
			/// <param name="mem">���Ͽ� �ش��ϴ� �޸��� ���� �ּ��Դϴ�.</param>
			/// <param name="size">������ ũ��(����Ʈ)�Դϴ�.</param>
			/// <param name="name">���α׷� ������ ����� �����̸�, �ʼ������� �����ؾ� �մϴ�.</param>
			static std::shared_ptr<Source> load(const void* mem, size_t size, const std::string& name);

			/// <summary>
			/// �ҷ��� ������ �޸𸮿��� �����մϴ�. �ش� �̸��� ������ ���ų� �ٸ� ������ ��� ���� ��� ��� ���� ��� �޸𸮰� ȸ���˴ϴ�.
			/// "���"�� "���"���� �����ϰ� ���� �����͸� �����ϰ� �ִ� ��ü�� ������ ����ϴ� ������ Ĩ�ϴ�. ��ü/�����͸� �Ҹ��Ű�� �ʰ� ����� �����Ϸ���
			/// �ش� ���� �����ͷ� reset() �Ǵ� swap(pAudioSource()) ���� ȣ���ϸ� �˴ϴ�.
			/// ������ �����͸� ������ ��������� �� �����͸� ���� ��� ������ �������� �ʰ� �Ҹ����� ���, drop���� ������ �Ҹ�ȴٸ� ��� ��Ʈ���� ��� �Բ� �Ҹ�˴ϴ�.
			/// </summary>
			/// <param name="name">������ ���� �̸�</param>
			static void drop(const std::string& name);

			/// <summary>
			/// �ҷ��� ��� ���� �� ���� ���ǰ� ���� ���� ���� ��� �޸𸮿��� �����մϴ�.
			/// ��� ���� ������ ����� ������ ��� �޸𸮿��� ȸ������, �״�� ���ܵ��� ������ �� �ֽ��ϴ�.
			/// "���"�� "���"���� �����ϰ� ���� �����͸� �����ϰ� �ִ� ��ü�� ������ ����ϴ� ������ Ĩ�ϴ�. ��ü/�����͸� �Ҹ��Ű�� �ʰ� ����� �����Ϸ���
			/// �ش� ���� �����ͷ� reset() �Ǵ� swap(pAudioSource()) ���� ȣ���ϸ� �˴ϴ�.
			/// </summary>
			/// <param name="removeUsing">true�� ��� ��� ���� ������ ��� ����� ������ ��� �޸𸮿��� ȸ���˴ϴ�.</param>
			static void collect(bool removeUsing = false);

			/// <summary>
			/// �ҷ��� ������ ����մϴ�. ����� ��Ʈ���� �����Ͱ� ���ϵǸ�, �̸� �̿��� �ߴ�/�簳/������ �� �� �ֽ��ϴ�. ���� ���θ� ������ �� �ֽ��ϴ�.
			/// ���� ������ �ƴ� ��� �ڵ����� �޸𸮰� �����˴ϴ�.
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
		/// ���� ����ǰ� �ִ� �Ҹ��Դϴ�.
		/// �ߴ�/�簳/������ �� �� �ֽ��ϴ�.
		/// �ִ� 0.1���� �����̰� ���� �� �ֽ��ϴ�. �����̰� �ξ� ª������ ���ڴٸ�, RINGBUFFER_SIZE�� �� �۰� ������ �ּ���.
		/// </summary>
		class Stream {
			friend class Source;
		public:
			/// <summary>
			/// ���� ����ǰ� �ִ� ��Ʈ�� ���Դϴ�.
			/// </summary>
			static const int& activeCount;
			/// <summary>
			/// ����ǰ� �ִ� �Ҹ��� �Ͻ������� ����ϴ�. ���� �ִ� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			void pause();
			/// <summary>
			/// ���� �ִ� �Ҹ��� �ٽ� ����մϴ�. ���� ���� ���� ��� �ƹ��͵� ���� �ʽ��ϴ�.
			/// </summary>
			void resume();
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

	using pAudioSource = std::shared_ptr<Audio::Source>;	// ������ �������Դϴ�.
}

#endif // !__OA_AUDIO_H__
