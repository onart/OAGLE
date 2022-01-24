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
	#pragma warning(disable: 26812 26819)
#endif

#include <cstdlib>

namespace onart {

	std::map<std::string, Audio::Source*> Audio::source;

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

	float Audio::master;

	void Audio::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			printf("\n����� �ʱ�ȭ�� �����߽��ϴ�.\n");
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
		
		PaSampleFormat sf = 0;
		switch (cctx->sample_fmt)
		{
		case AVSampleFormat::AV_SAMPLE_FMT_U8P:
			sf = paNonInterleaved;
			// fall through
		case AVSampleFormat::AV_SAMPLE_FMT_U8:
			sf |= paUInt8;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_S16P:
			sf = paNonInterleaved;
			// fall through
		case AVSampleFormat::AV_SAMPLE_FMT_S16:
			sf |= paInt16;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_S32P:
			sf = paNonInterleaved;
			// fall through
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_S32:
			sf |= paInt32;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_FLTP:
			sf = paNonInterleaved;
			// fall through
		case AVSampleFormat::AV_SAMPLE_FMT_FLT:
			sf |= paFloat32;
			break;
		default:
			printf("�������� �ʴ� ���� �����Դϴ�. �� ���� ������ ������ ��ȯ�ϴ� ���� ����� �ּ���.\n");
			break;
		}
		AVPacket tempP;
		AVFrame* tempF = av_frame_alloc();
		
		int sampleCount = 0;
		int sampleRate = 0;
		int channelCount = 0;
		while (av_read_frame(fmt, &tempP) == 0) {
			int i = avcodec_send_packet(cctx, &tempP);
			if (i == 0) {
				avcodec_receive_frame(cctx, tempF);
				sampleCount += tempF->nb_samples;
			}
		}
		sampleRate = tempF->sample_rate;
		channelCount = tempF->channels;
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		avcodec_close(cctx);
		av_frame_free(&tempF);
		return Audio::source[memName] = new Source(fmt, sampleRate, sampleCount, sf, channelCount);
	}

	void Audio::Source::drop(const std::string& name) {
		auto iter = source.find(name);
		if (iter != source.end()) {
			delete iter->second;
			source.erase(iter);
		}
	}

	Audio::Source::Source(AVFormatContext* ctx, int sampleRate, int sampleCount, unsigned long sampleFormat, int channelCount)
		:ctx(ctx), sampleRate(sampleRate), sampleCount(sampleCount), sampleFormat(sampleFormat), channelCount(channelCount) {

	}

	Audio::Source::~Source() {
		for (auto p : playing) {
			delete p;
		}
		avformat_close_input(&ctx);
	}

	/// <summary>
	/// "PCM"�� ����ϵ��� �ϴ� �ݹ� �Լ�
	/// </summary>
	static int playCallback(const void* input, void* output,
		unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo,
		PaStreamCallbackFlags statusFlags, void* userData) {
		return PaStreamCallbackResult::paContinue;
	}
	/// <summary>
	/// �ݹ� ���� 
	/// </summary>
	static void finishCallback(void* data) {
		
	}

	Audio::Stream::Stream(AVFormatContext* src, int sampleRate, int sampleCount, unsigned long sampleFormat, int channelCount, bool loop)
		:src(src), sampleRate(sampleRate), sampleCount(sampleCount), sampleFormat(sampleFormat), loop(loop) {
		src->audio_codec;
		PaError err = Pa_OpenDefaultStream(
			&stream, 0, channelCount, sampleFormat, sampleRate, paFramesPerBufferUnspecified, playCallback, this
		);
		if (err == PaErrorCode::paNoError) {
			Pa_SetStreamFinishedCallback(stream, finishCallback);
			Pa_StartStream(stream);
		}
	}

	void Audio::Stream::pause() {
		Pa_StopStream(stream);
	}
}