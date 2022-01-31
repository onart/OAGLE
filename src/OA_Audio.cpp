#include "OA_Audio.h"
#include "oagle_simd.h"

#include "externals/portaudio/portaudio.h"

extern "C" {
	#include "externals/ffmpeg/libavcodec/avcodec.h"
	#include "externals/ffmpeg/libavformat/avformat.h"
	#include "externals/ffmpeg/libswresample/swresample.h"
	#include "externals/ffmpeg/libavutil/opt.h"
}


#ifdef _WIN32
	#ifdef _WIN64
		#pragma comment(lib, "externals/portaudio/portaudio_x64.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avformat.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avcodec.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/avutil.lib")
		#pragma comment(lib, "externals/ffmpeg/libx8664/swresample.lib")
	#else
		#pragma comment(lib, "externals/portaudio/portaudio_x86.lib")
		#pragma comment(lib, "externals/ffmpeg/libx86/avformat.lib")
		#pragma comment(lib, "externals/ffmpeg/libx86/avcodec.lib")
		#pragma comment(lib, "externals/ffmpeg/libx86/avutil.lib")
		#pragma comment(lib, "externals/ffmpeg/libx86/swresample.lib")
	#endif // _WIN64
	#pragma warning(disable: 26812 26819)
#endif

#include <cstdlib>

/// ���� ������� portaudio ���̺귯������ ���� ��Ʈ���� ����ϱ� ���� �������� ������ �˴ϴ�. Ÿ���� �ٲٴ� ��� ���۸� �� ���굵 �ٲ�� �մϴ�. (�������� ����)
constexpr int STD_SAMPLE_RATE = 44100;
constexpr int FF_RESAMPLE_FORMAT = AVSampleFormat::AV_SAMPLE_FMT_FLT;
constexpr int PA_SAMPLE_FORMAT = paFloat32;
constexpr int STD_CHANNEL_COUNT = 2;
using STD_SAMPLE_FORMAT = float;
/// =====================================================

namespace onart {

	std::map<std::string, Audio::Source*> Audio::source;
	void* Audio::masterStream;
	float Audio::master;
	static class RingBuffer {
	public:
		unsigned long writable();
		void add(float* in);
		void addComplete();
		void read(void* out, unsigned long count);
	private:
		float body[RINGBUFFER_SIZE] = { 0, };	// �� 6������ �з�
		unsigned long readIndex = 1000;
		unsigned long writeIndex = 0;
	} ringBuffer;

	unsigned long RingBuffer::writable() {
		if (readIndex < writeIndex) {
			return RINGBUFFER_SIZE - writeIndex + readIndex;
		}
		else {
			return readIndex - writeIndex;
		}
	}

	void RingBuffer::addComplete() {	// +-1�� �ڸ��� �ε��� ����
		if (readIndex == writeIndex) return;
		if (writeIndex < readIndex) {
			BEGINFROMZERO:
			for (; writeIndex < readIndex - 4; writeIndex += 4) {
				clamp4<float>(body + writeIndex, -1, 1);
			}
			for (; writeIndex < readIndex; writeIndex++) {
				body[writeIndex] = body[writeIndex] < -1 ? -1 : body[writeIndex];
				body[writeIndex] = body[writeIndex] > 1 ? 1 : body[writeIndex];
			}
			writeIndex = readIndex;
		}
		else {
			for (; writeIndex < RINGBUFFER_SIZE - 4; writeIndex += 4) {
				clamp4<float>(body + writeIndex, -1, 1);
			}
			for (; writeIndex < RINGBUFFER_SIZE; writeIndex++) {
				body[writeIndex] = body[writeIndex] < -1 ? -1 : body[writeIndex];
				body[writeIndex] = body[writeIndex] > 1 ? 1 : body[writeIndex];
			}
			writeIndex = 0;
			goto BEGINFROMZERO;
		}
	}

	void RingBuffer::add(float* in) {
		unsigned long outIndex = writeIndex;
		int inIndex = 0;
		if (writeIndex > readIndex) {
			for (inIndex += 4; inIndex < (int)RINGBUFFER_SIZE - (int)writeIndex; inIndex += 4) {
				add4<float>(body + inIndex - 4 + writeIndex, in + inIndex - 4);
			}
			for (inIndex -= 4; inIndex < (int)RINGBUFFER_SIZE - (int)writeIndex; inIndex++) {
				body[inIndex + writeIndex] += in[inIndex];
			}
			outIndex = 0;
		}
		for (; outIndex + 4 < readIndex; outIndex += 4, inIndex += 4) {
			add4<float>(body + outIndex, in + inIndex);
		}
		for (outIndex -= 4; outIndex < readIndex; outIndex++, inIndex++) {
			body[outIndex] += in[inIndex];
		}
	}

	/// <summary>
	/// ffmpeg�� wav ������ ������ �ڵ����� �� �� �ֱ淡 ���� �߰�
	/// </summary>
	union WavFile {
		char header[44];
		struct {
			unsigned char chunkId[4];
			unsigned int chunkSize;
			unsigned char format[4];
		} RIFF;
		struct {
			unsigned char chunkId[4];
			unsigned int chunkSize;
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned int sampleRate;
			unsigned int avgByteRate;
			unsigned short blockAlign;
			unsigned short bitsPerSample;
		} FMT;
		struct {
			char chunkID[4];
			unsigned int chunkSize;
		} DATA;
	};

	void Audio::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			printf("\n����� �ʱ�ȭ�� �����߽��ϴ�.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		err = Pa_OpenDefaultStream(&masterStream, 0, 2, PA_SAMPLE_FORMAT, STD_SAMPLE_RATE, paFramesPerBufferUnspecified, Audio::playCallback, &ringBuffer);
		if (err != PaErrorCode::paNoError) {
			printf("\n����� ��Ʈ�� �ʱ�ȭ�� �����߽��ϴ�.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		
		err = Pa_StartStream(masterStream);
		if (err != PaErrorCode::paNoError) {
			printf("\n����� ��Ʈ�� ���ۿ� �����߽��ϴ�.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		master = 1;
		system("cls");
	}

	void Audio::terminate() {
		Pa_CloseStream(masterStream);
		PaError err = Pa_Terminate();
	}

	void Audio::setMasterVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		master = v;
		
	}

	Audio::Source* Audio::Source::load(const std::string& file, const std::string& name) {
		std::string memName(name);
		if (memName.size() == 0) { memName = file; }
		if (Audio::source.find(memName) != Audio::source.end()) { return Audio::source[memName]; }
		// Ȯ���ڰ� wav�� ��� ���� �ε�
		AVFormatContext* fmt = avformat_alloc_context();

		if (avformat_open_input(&fmt, file.c_str(), nullptr, nullptr) < 0) {
			printf("���� ������ ���ų� �ε忡 �����߽��ϴ�.\n");
			if (fmt)avformat_close_input(&fmt);
			return nullptr;
		}
		avformat_find_stream_info(fmt, nullptr);
		
		if (fmt->nb_streams <= 0) {
			printf("���� ������ ��ȿ���� ���� �� �����ϴ�. �ٽ� �� �� Ȯ���� �ּ���.\n");
			avformat_close_input(&fmt);
			return nullptr;
		}
		AVCodecID cid = fmt->streams[0]->codecpar->codec_id;
		if (cid < AVCodecID::AV_CODEC_ID_MP2 && cid>AVCodecID::AV_CODEC_ID_MSNSIREN) {
			printf("���� ������ ��ȿ���� ���� �� �����ϴ�. �ٽ� �� �� Ȯ���� �ּ���.\n");
			avformat_close_input(&fmt);
			return nullptr;
		}
		const AVCodec* codec = avcodec_find_decoder(cid);
		fmt->audio_codec = codec;
		if (!codec) {
			printf("�������� �ʴ� ������ �� �����ϴ�. �ٸ� �������� �õ��� �ּ���.\n");
			avformat_close_input(&fmt);
			return nullptr;
		}
		AVCodecContext* cctx = avcodec_alloc_context3(codec);
		avcodec_open2(cctx, nullptr, nullptr);
		SwrContext* resampler = nullptr;

		AVSampleFormat inputFormat = cctx->sample_fmt;

		if (inputFormat < AVSampleFormat::AV_SAMPLE_FMT_U8 || inputFormat > AVSampleFormat::AV_SAMPLE_FMT_S64P) {
			printf("�������� �ʴ� ���� �����Դϴ�. ������ ��ȯ�ϴ� ���� ����� �ּ���.\n");
			avformat_close_input(&fmt);
			return nullptr;
		}

		AVPacket tempP;
		AVFrame* tempF = av_frame_alloc();

		int frameCount = 0;
		while (av_read_frame(fmt, &tempP) == 0) {
			int i = avcodec_send_packet(cctx, &tempP);
			if (i == 0) {
				avcodec_receive_frame(cctx, tempF);
				frameCount++;
			}
			av_packet_unref(&tempP);
		}
		int sampleRate = cctx->sample_rate;

		if (
			!(inputFormat == AVSampleFormat::AV_SAMPLE_FMT_FLT &&
			sampleRate == 44100 &&
			cctx->channel_layout==AV_CH_LAYOUT_STEREO)
			) {
			resampler = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_STEREO, (AVSampleFormat)FF_RESAMPLE_FORMAT, STD_SAMPLE_RATE, cctx->channel_layout, inputFormat, sampleRate, 0, nullptr);
			if (swr_init(resampler) < 0) {
				printf("�����÷� �ʱ�ȭ�� �����߽��ϴ�. �ҽ��� �ε���� �ʽ��ϴ�.\n");
				avformat_close_input(&fmt);
				return nullptr;
			}
		}
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		avcodec_close(cctx);
		av_frame_free(&tempF);

		return Audio::source[memName] = new Source(fmt, resampler, frameCount);
	}

	void Audio::Source::drop(const std::string& name) {
		auto iter = source.find(name);
		if (iter != source.end()) {
			delete iter->second;
			source.erase(iter);
		}
	}

	int Audio::Source::getFrame(int frameNumber, float** dst) {
		if (frameNumber >= frameCount || frameNumber < 0) {
			return -1;	// �ݺ��̸� ù ������(0��)�� ��û, �ƴϸ� ����. ���� ���� �޾� ��ü�� ���Ÿ� �����ϱ⵵ ��
		}
		// seek�� ������ �ѹ��� ���� ����� �̻��ؼ� �ӽù������� ���� ���� �ڵ� �߰�. �� �˾ƺ��� ������ ��
		AVPacket pkt;
		static int recentFrame = 0;
		if (frameNumber != recentFrame + 1) {
			av_seek_frame(ctx, -1, 0, AVSEEK_FLAG_FRAME);
			for (int i = 0; i < frameNumber; i++) {
				av_read_frame(ctx, &pkt);
				av_packet_unref(&pkt);
			}
		}
		// seek�� ������ �ѹ��� ���� ����� �̻��ؼ� �ӽù������� ���� ���� �ڵ� �߰�. �� �˾ƺ��� ������ ��
		AVFrame* frm = av_frame_alloc();
		if (av_read_frame(ctx, &pkt) == 0) {
			int i = avcodec_send_packet(cdc, &pkt);
			if (i == 0) {
				avcodec_receive_frame(cdc, frm);
				if (resampler) {
					AVFrame* frm2 = av_frame_alloc();
					frm2->channel_layout = AV_CH_LAYOUT_STEREO;
					frm2->format = AV_SAMPLE_FMT_FLT;
					frm2->sample_rate = 44100;
					auto err=swr_convert_frame(resampler, frm2, frm);
					*dst = (float*)malloc(frm2->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					memcpy(*dst, frm2->data[0], frm2->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					int ret = frm2->nb_samples;
					av_packet_unref(&pkt);
					av_frame_free(&frm);
					av_frame_free(&frm2);
					return ret;
				}
				else {
					int ret = frm->nb_samples;
					*dst = (float*)malloc(frm->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					memcpy(*dst, frm->data[0], frm->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					av_packet_unref(&pkt);
					av_frame_free(&frm);
					return ret;
				}
			}
			else {
				av_packet_unref(&pkt);
				av_frame_free(&frm);
				return 0;
			}
		}
		return 0;
	}

	Audio::Source::Source(AVFormatContext* ctx, SwrContext* resampler, int frameCount)
		:ctx(ctx), resampler(resampler), frameCount(frameCount) {
		cdc = avcodec_alloc_context3(ctx->audio_codec);
		avcodec_open2(cdc, nullptr, nullptr);
	}

	Audio::Source::~Source() {
		for (auto p : playing) {
			delete p;
		}
		avcodec_close(cdc);
		avcodec_free_context(&cdc);
		if (resampler) { 
			swr_close(resampler); 
			swr_free(&resampler);
		}
		avformat_close_input(&ctx);
	}

	/// "PCM"�� ����ϵ��� �ϴ� �ݹ� �Լ�
	int Audio::playCallback(const void* input, void* output, unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData) {

		RingBuffer* rb = (RingBuffer*)userData;
		rb->read(output, frameCount);
		return PaStreamCallbackResult::paContinue;	// ���� ��ȣ �ܿ� �������� ����
	}
	/// <summary>
	/// �ݹ� ���� 
	/// </summary>
	static void finishCallback(void* data) {
		
	}

	Audio::Stream::Stream(Source* src, bool loop)
		:src(src), loop(loop) {
		
	}

	Audio::Stream::~Stream() {
		free(buffer);
	}

	void Audio::update() {
		for (auto& s : source) {
			s.second->update();
		}
		ringBuffer.addComplete();
	}

	Audio::Source* Audio::Source::get(const std::string& name) {
		if (source.find(name) != source.end())return source[name];
		else return nullptr;
	}

	void Audio::Source::update() {
		bool reap = false;
		for (Stream*& s : playing) {
			if (s->update()) {
				delete s;
				s = nullptr;
				reap = true;
			}
		}
		if (reap) playing.erase(std::remove(playing.begin(), playing.end(), nullptr), playing.end());
	}

	void RingBuffer::read(void* out, unsigned long count) {
		unsigned long r1 = readIndex + count > RINGBUFFER_SIZE ? RINGBUFFER_SIZE - readIndex : count;
		memcpy(out, body + readIndex, r1 * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
		memset(body + readIndex, 0, r1 * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
		readIndex += count;
		if (readIndex >= RINGBUFFER_SIZE) {
			readIndex = count - r1;
			memcpy(out, body, readIndex * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT));
			memset(body, 0, readIndex * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT));
		}
	}

	bool Audio::Stream::update() {	// ���ϰ�: true ���� �� �޸� ���ŵ�(�Ҹ�)
		if (stopped) return false;
		unsigned long need = ringBuffer.writable();
		
		while (restSamples < need) {
			float* temp;
			int count = src->getFrame(nextFrame++, &temp);
			if (count == -1) {	// ���� ������ �����
				if (loop) nextFrame = 0;
				else {
					stopped = true;
					break;
				}
			}
			else if (count > 0) {
				restSamples += count;
				void* tmp2 = realloc(buffer, restSamples * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT));
				if (!tmp2) { 
					perror("stream update - realloc()");
					return false;
				}
				buffer = (float*)tmp2;
				memcpy(buffer + restSamples - count, temp, count * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT));
				free(temp);
			}
		}
		ringBuffer.add(buffer);
		memmove(buffer, buffer + need * STD_CHANNEL_COUNT, (restSamples - need) * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT));
		restSamples -= need;
		if (stopped) {
			return true;
		}
		return false;
	}

	void Audio::Stream::restart() {
		nextFrame = 0;
	}

	void Audio::Stream::end() {
		loop = false;
		nextFrame = -1;
	}

	Audio::Stream* Audio::Source::play(bool loop) {
		Stream* nw = new Stream(this, loop);
		playing.push_back(nw);
		return nw;
	}

}