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
#include <cstring>
#include <filesystem>
#include <thread>
#include <mutex>
#include <condition_variable>

/// 이쪽 상수들은 portaudio 라이브러리에서 단일 스트림을 사용하기 위해 리샘플할 기준이 됩니다. 여기 있는 인자들을 바꾸는 경우 버퍼링 시 연산도 바꿔야 합니다. (생각보다 복잡)
constexpr int FF_RESAMPLE_FORMAT = AVSampleFormat::AV_SAMPLE_FMT_FLT;
constexpr int PA_SAMPLE_FORMAT = paFloat32;
constexpr int STD_CHANNEL_COUNT = 2;
using STD_SAMPLE_FORMAT = float;
/// =====================================================
constexpr size_t MEMBUFFER_SIZE = 8192;

namespace onart {

	std::map<std::string, Audio::Source*> Audio::source;
	void* Audio::masterStream;
	float Audio::master = 1;
	const float& Audio::masterVolume = Audio::master;
	int Audio::Stream::activeStreamCount = 0;
	const int& Audio::Stream::activeCount = Audio::Stream::activeStreamCount;
	bool Audio::noup = true;

#ifndef OA_AUDIO_NOTHREAD
	static std::condition_variable aCV;
	static std::mutex audioMutex;
	static std::unique_lock<std::mutex> aLock;
#elif defined(OA_AUDIO_WAIT_ON_DRAG)
	bool Audio::wait = false;
#endif

	static class RingBuffer {
	public:
		unsigned long writable();
		void add(STD_SAMPLE_FORMAT* in, unsigned long max);
		void addComplete();
		void read(void* out, unsigned long count);
	private:
		STD_SAMPLE_FORMAT body[RINGBUFFER_SIZE] = { 0, };	// 약 6프레임 분량
		unsigned long readIndex = 0;	// 콜백에서 읽는 기준
		unsigned long limitIndex = 0;	// 쓰기 제한 기준
		unsigned long writeIndex = 0;	// 쓰기 시작점
		bool isFirst = true;
	} ringBuffer;

	unsigned long RingBuffer::writable() {
		if (isFirst) { limitIndex = readIndex; }
		if (writeIndex > limitIndex) {
			return RINGBUFFER_SIZE - writeIndex + limitIndex;
		}
		else {
			return limitIndex - writeIndex;
		}
	}

	void RingBuffer::addComplete() {	// +-1로 자르고 인덱스 맞춤
		isFirst = true;
		if (limitIndex == writeIndex) return;
		if (limitIndex > writeIndex) {
			if (Audio::masterVolume != 1) { 
				mulAll(body + writeIndex, Audio::masterVolume, limitIndex - writeIndex); 
			}
			clampAll(body + writeIndex, -1.0f, 1.0f, limitIndex - writeIndex);
			writeIndex = limitIndex;
		}
		else {
			if (Audio::masterVolume != 1) {
				mulAll(body + writeIndex, Audio::masterVolume, RINGBUFFER_SIZE - writeIndex);
				mulAll(body, Audio::masterVolume, limitIndex);
			}
			clampAll(body + writeIndex, -1.0f, 1.0f, RINGBUFFER_SIZE - writeIndex);
			clampAll(body, -1.0f, 1.0f, limitIndex);
			writeIndex = limitIndex;
		}
	}

	void RingBuffer::add(STD_SAMPLE_FORMAT* in, unsigned long max) {
		if (isFirst) {
			if (limitIndex >= writeIndex) {
				unsigned long w = limitIndex - writeIndex;
				w = w > max ? max : w;
				memcpy(body + writeIndex, in, w * sizeof(STD_SAMPLE_FORMAT));
			}
			else {
				unsigned long w = RINGBUFFER_SIZE - writeIndex;
				w = w > max ? max : w;
				memcpy(body + writeIndex, in, w * sizeof(STD_SAMPLE_FORMAT));
				if (max > w) {
					in += w;
					max -= w;
					w = limitIndex > max ? max : limitIndex;
					memcpy(body, in, w * sizeof(STD_SAMPLE_FORMAT));
				}
			}
			isFirst = false;
			return;
		}
		else {
			if (limitIndex >= writeIndex) {
				unsigned long w = limitIndex - writeIndex;
				w = w > max ? max : w;
				addAll(body + writeIndex, in, w);
			}
			else {
				unsigned long w = RINGBUFFER_SIZE - writeIndex;
				w = w > max ? max : w;
				addAll(body + writeIndex, in, w);
				if (max > w) {
					in += w;
					max -= w;
					w = limitIndex > max ? max : limitIndex;
					addAll(body, in, w);
				}
			}
		}
		
	}

	/// <summary>
	/// ffmpeg이 wav 파일을 만나면 자동으로 안 해 주길래 따로 추가
	/// </summary>
	union WavFile {
		char header[44];
		struct {
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
	};

	void Audio::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			printf("\n오디오 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		err = Pa_OpenDefaultStream(&masterStream, 0, 2, PA_SAMPLE_FORMAT, STD_SAMPLE_RATE, paFramesPerBufferUnspecified, Audio::playCallback, nullptr);
		if (err != PaErrorCode::paNoError) {
			printf("\n오디오 스트림 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		
		err = Pa_StartStream(masterStream);
		if (err != PaErrorCode::paNoError) {
			printf("\n오디오 스트림 시작에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		if constexpr (!OA_AUDIO_NOTHREAD) {
			std::thread aud(audioThread);
			aud.detach();	// 데몬스레드
		}
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

	void Audio::allow() {
		//if constexpr (!OA_AUDIO_NOTHREAD) aCV.notify_one();
		noup = false;
	}

	void Audio::acquire() {
		//if constexpr (!OA_AUDIO_NOTHREAD) audioMutex.lock();
	}

	void Audio::Source::setVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		volume = v;
	}

	int Audio::Source::MemorySource::request(int req, unsigned char* buf) {
		if (size <= cursor) return AVERROR_EOF;
		uint64_t nextCursor = cursor + (uint64_t)req;
		if (nextCursor < cursor) {	// 오버플로
			return -1;
		}
		else if (nextCursor > size) {
			nextCursor = size;
		}
		uint64_t cpsize = nextCursor - cursor;
		memcpy(buf, dat + cursor, cpsize);
		cursor = nextCursor;
		return (int)cpsize;
	}

	int64_t Audio::Source::MemorySource::seek(int64_t pos, int origin) {
		switch (origin)
		{
		case SEEK_SET:
			cursor = (uint64_t)pos;
			break;
		case SEEK_CUR:
			cursor += (uint64_t)pos;	// int와 uint는 표현 및 더하는 방식이 같음
			break;
		case SEEK_END:
			cursor = size + (uint64_t)pos;
			break;
		case AVSEEK_SIZE:
			return (int64_t)size;
		default:
			cursor = (uint64_t)pos;
		}
		if (cursor > size) { return -1; }
		return (int64_t)cursor;
	}

	int Audio::Source::readMem(void* ptr, uint8_t* buf, int len) {
		MemorySource* src = (MemorySource*)ptr;
		return src->request(len, buf);
	}

	int64_t Audio::Source::seekMem(void* ptr, int64_t pos, int origin) {
		MemorySource* src = (MemorySource*)ptr;
		return src->seek(pos, origin);
	}

	Audio::Source::MemorySource::MemorySource(const void* dat, uint64_t size) :dat((const unsigned char*)dat), size(size), buf((unsigned char*)av_malloc(MEMBUFFER_SIZE)) {	}
	Audio::Source::MemorySource::~MemorySource() { av_free(buf); }

	Audio::Source* Audio::Source::load(const void* mem, size_t size, const std::string& name) {
		if (Audio::source.find(name) != Audio::source.end()) { return Audio::source[name]; }
		AVFormatContext* fmt = avformat_alloc_context();
		MemorySource* msrc = new MemorySource(mem, size);
		AVIOContext* ioContext = avio_alloc_context(msrc->buf, MEMBUFFER_SIZE, 0, msrc, readMem, nullptr, seekMem);		
		fmt->pb = ioContext;
		fmt->flags |= AVFMT_FLAG_CUSTOM_IO;
		if (avformat_open_input(&fmt, "", nullptr, nullptr) < 0) {
			printf("디멀티플렉싱에 실패했습니다. 파일을 다시 점검해 주세요.\n");
			av_free(ioContext);
			delete msrc;
			
			return nullptr;
		}
		if (!initDemux(fmt)) { 
			avformat_close_input(&fmt);
			return nullptr;
		}
		int audioStreamIndex = -1;
		audioStreamIndex = av_find_best_stream(fmt, AVMEDIA_TYPE_AUDIO, -1, -1, &fmt->audio_codec, 0);
		AVCodecContext* cctx = avcodec_alloc_context3(fmt->audio_codec);
		avcodec_parameters_to_context(cctx, fmt->streams[audioStreamIndex]->codecpar);
		int er = avcodec_open2(cctx, nullptr, nullptr);
		SwrContext* resampler = nullptr;

		AVSampleFormat inputFormat = cctx->sample_fmt;
		if (inputFormat < AVSampleFormat::AV_SAMPLE_FMT_U8 || inputFormat > AVSampleFormat::AV_SAMPLE_FMT_S64P) {
			printf("지원하지 않는 샘플 형식입니다. 파일을 변환하는 것을 고려해 주세요.\n");
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
		if (cctx->channel_layout == 0) {
			const WavFile& wvhd = reinterpret_cast<const WavFile*>(mem)[0];
			if (memcmp(wvhd.RIFF.chunkId, "RIFF", 4) == 0 &&
				memcmp(wvhd.RIFF.format, "WAVE", 4) == 0 &&
				memcmp(wvhd.FMT.chunkId, "fmt ", 4) == 0
				) {
				switch (wvhd.FMT.numChannels)
				{
				case 1:
					cctx->channel_layout = AV_CH_LAYOUT_MONO;
					break;
				case 2:
					cctx->channel_layout = AV_CH_LAYOUT_STEREO;
					break;
				case 3:
					cctx->channel_layout = AV_CH_LAYOUT_SURROUND;
					break;
				case 4:
					cctx->channel_layout = AV_CH_LAYOUT_QUAD;
					break;
				case 5:
					cctx->channel_layout = AV_CH_LAYOUT_4POINT0;
					break;
				case 6:
					cctx->channel_layout = AV_CH_LAYOUT_6POINT0;
					break;
				default:
					break;
				}
			}
		}
		if (
			!(inputFormat == AVSampleFormat::AV_SAMPLE_FMT_FLT &&
				sampleRate == STD_SAMPLE_RATE &&
				cctx->channel_layout == AV_CH_LAYOUT_STEREO)
			) {
			resampler = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_STEREO, (AVSampleFormat)FF_RESAMPLE_FORMAT, STD_SAMPLE_RATE, cctx->channel_layout, inputFormat, sampleRate, 0, nullptr);
			if (swr_init(resampler) < 0) {
				printf("리샘플러 초기화에 실패했습니다. 소스가 로드되지 않습니다.\n");
				avformat_close_input(&fmt);
				return nullptr;
			}
		}
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		av_frame_free(&tempF);

		return Audio::source[name] = new Source(fmt, cctx, resampler, frameCount, ioContext, msrc);
	}

	bool Audio::Source::initDemux(AVFormatContext* fmt) {
		if (avformat_find_stream_info(fmt, nullptr) < 0) {
			printf("스트림 정보를 얻지 못했습니다.\n");
			return false;
		}
		if (fmt->nb_streams <= 0) {
			printf("음성 파일이 유효하지 않은 것 같습니다. 다시 한 번 확인해 주세요.\n");
			return false;
		}
		AVCodecID cid = fmt->streams[0]->codecpar->codec_id;
		if (cid < AVCodecID::AV_CODEC_ID_FIRST_AUDIO || cid>AVCodecID::AV_CODEC_ID_MSNSIREN) {
			printf("음성 파일이 유효하지 않은 것 같습니다. 다시 한 번 확인해 주세요.\n");
			return false;
		}
		const AVCodec* codec = avcodec_find_decoder(cid);
		if (!codec) {
			printf("지원하지 않는 형식인 것 같습니다. 다른 형식으로 변환해 주세요.\n");
			return false;
		}
		fmt->audio_codec = codec;
		return true;
	}

	Audio::Source* Audio::Source::load(const std::string& file, const std::string& name) {
		std::string memName(name);
		if (memName.size() == 0) { memName = file; }
		if (Audio::source.find(memName) != Audio::source.end()) { return Audio::source[memName]; }

		AVFormatContext* fmt = avformat_alloc_context();
		if (avformat_open_input(&fmt, file.c_str(), nullptr, nullptr) < 0) {
			printf("음성 파일이 없거나 로드에 실패했습니다.\n");
			if (fmt)avformat_close_input(&fmt);
			return nullptr;
		}
		if (!initDemux(fmt)) {
			avformat_close_input(&fmt);
			return nullptr;
		}

		int audioStreamIndex = -1;
		audioStreamIndex = av_find_best_stream(fmt, AVMEDIA_TYPE_AUDIO, -1, -1, &fmt->audio_codec, 0);
		
		AVCodecContext* cctx = avcodec_alloc_context3(fmt->audio_codec);
		avcodec_parameters_to_context(cctx, fmt->streams[audioStreamIndex]->codecpar);
		int er=avcodec_open2(cctx, nullptr, nullptr);
		SwrContext* resampler = nullptr;

		AVSampleFormat inputFormat = cctx->sample_fmt;

		if (inputFormat < AVSampleFormat::AV_SAMPLE_FMT_U8 || inputFormat > AVSampleFormat::AV_SAMPLE_FMT_S64P) {
			printf("지원하지 않는 샘플 형식입니다. 파일을 변환하는 것을 고려해 주세요.\n");
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
		std::filesystem::path p(file);
		if (p.extension() == ".wav") {
			FILE* fp;
			fopen_s(&fp, file.c_str(), "rb");
			if (fp) {
				WavFile wf;
				fread(&wf, sizeof(wf), 1, fp);
				switch (wf.FMT.numChannels)
				{
				case 1:
					cctx->channel_layout = AV_CH_LAYOUT_MONO;
					break;
				case 2:
					cctx->channel_layout = AV_CH_LAYOUT_STEREO;
					break;
				case 3:
					cctx->channel_layout = AV_CH_LAYOUT_SURROUND;
					break;
				case 4:
					cctx->channel_layout = AV_CH_LAYOUT_QUAD;
					break;
				case 5:
					cctx->channel_layout = AV_CH_LAYOUT_4POINT0;
					break;
				case 6:
					cctx->channel_layout = AV_CH_LAYOUT_6POINT0;
					break;
				default:
					break;
				}
				fclose(fp);
			}
			
		}

		if (
			!(inputFormat == AVSampleFormat::AV_SAMPLE_FMT_FLT &&
			sampleRate == STD_SAMPLE_RATE &&
			cctx->channel_layout==AV_CH_LAYOUT_STEREO)
			) {
			resampler = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_STEREO, (AVSampleFormat)FF_RESAMPLE_FORMAT, STD_SAMPLE_RATE, cctx->channel_layout, inputFormat, sampleRate, 0, nullptr);
			if (swr_init(resampler) < 0) {
				printf("리샘플러 초기화에 실패했습니다. 소스가 로드되지 않습니다.\n");
				avformat_close_input(&fmt);
				return nullptr;
			}
		}
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		av_frame_free(&tempF);

		return Audio::source[memName] = new Source(fmt, cctx, resampler, frameCount);
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
			return -1;	// 반복이면 첫 프레임(0번)을 요청, 아니면 종료. 음의 값을 받아 객체의 제거를 유도하기도 함
		}
		// [언젠가 단일 seek로 수정. timestamp 필요]
		AVPacket pkt;
		static int recentFrame = 0;
		if (frameNumber != recentFrame + 1) {
			av_seek_frame(ctx, -1, 0, AVSEEK_FLAG_FRAME);
			for (int i = 0; i < frameNumber; i++) {
				av_read_frame(ctx, &pkt);
				av_packet_unref(&pkt);
			}
		}
		recentFrame = frameNumber;
		// [언젠가 단일 seek로 수정]
		AVFrame* frm = av_frame_alloc();
		if (av_read_frame(ctx, &pkt) == 0) {
			int i = avcodec_send_packet(cdc, &pkt);
			if (i == 0) {
				int err = avcodec_receive_frame(cdc, frm);
				if (resampler) {
					AVFrame* frm2 = av_frame_alloc();
					frm2->channel_layout = AV_CH_LAYOUT_STEREO;
					frm2->format = AV_SAMPLE_FMT_FLT;
					frm2->sample_rate = STD_SAMPLE_RATE;
					auto err=swr_convert_frame(resampler, frm2, frm);
					*dst = (float*)malloc(frm2->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					memcpy(*dst, frm2->data[0], frm2->nb_samples * sizeof(STD_SAMPLE_FORMAT) * STD_CHANNEL_COUNT);
					int ret = frm2->nb_samples * STD_CHANNEL_COUNT;
					av_packet_unref(&pkt);
					av_frame_free(&frm);
					av_frame_free(&frm2);
					return ret;
				}
				else {
					int ret = frm->nb_samples * STD_CHANNEL_COUNT;
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

	Audio::Source::Source(AVFormatContext* ctx, AVCodecContext* cdc, SwrContext* resampler, int frameCount, AVIOContext* ioctx, MemorySource* ms)
		: ctx(ctx), cdc(cdc), resampler(resampler), frameCount(frameCount), ioctx(ioctx), memsrc(ms) {

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
		if (ioctx) { 
			av_free(ioctx);
			delete memsrc;
		}
	}

	/// "PCM"을 재생하도록 하는 콜백 함수
	int Audio::playCallback(const void* input, void* output, unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData) {
#ifdef OA_AUDIO_WAIT_ON_DRAG
		if (wait) { memset(output, 0, frameCount * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT)); return PaStreamCallbackResult::paContinue; }	// 문제점: 가끔 소리가 약간 체감될 정도로 딜레이됨
#endif
		ringBuffer.read(output, frameCount * STD_CHANNEL_COUNT);
		return PaStreamCallbackResult::paContinue;	// 정지 신호 외에 정지하지 않음
	}

	Audio::Stream::Stream(Source* src, bool loop)
		:src(src), loop(loop) {
		activeStreamCount++;
	}

	Audio::Stream::~Stream() {
		free(buffer);
		if (!stopped) activeStreamCount--;
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
		if (reap) { playing.erase(std::remove(playing.begin(), playing.end(), nullptr), playing.end()); }
	}

	void RingBuffer::read(void* out, unsigned long count) {
		unsigned long r1 = readIndex + count > RINGBUFFER_SIZE ? RINGBUFFER_SIZE - readIndex : count;
		memcpy(out, body + readIndex, r1 * sizeof(STD_SAMPLE_FORMAT));
		if (Audio::Stream::activeCount == 0) memset(body + readIndex, 0, r1 * sizeof(STD_SAMPLE_FORMAT));
		readIndex += count;
		if (readIndex >= RINGBUFFER_SIZE) {
			readIndex = count - r1;
			memcpy((STD_SAMPLE_FORMAT*)out + r1, body, readIndex * sizeof(STD_SAMPLE_FORMAT));
			if (Audio::Stream::activeCount == 0) memset(body, 0, readIndex * sizeof(STD_SAMPLE_FORMAT));
		}
	}

	bool Audio::Stream::update() {	// 리턴값: true 리턴 시 메모리 수거됨(소멸)
		if (stopped) return false;
		unsigned long need = ringBuffer.writable();
		
		while (restSamples < need) {
			STD_SAMPLE_FORMAT* temp;
			int count = src->getFrame(nextFrame++, &temp);
			if (count == -1) {	// 음원 끝까지 재생함
				if (loop) nextFrame = 0;
				else {
					stopped = true;
					activeStreamCount--;
					break;
				}
			}
			else if (count > 0) {
				if (src->volume != 1) { mulAll(temp, src->volume, count); }
				restSamples += count;
				STD_SAMPLE_FORMAT* tmp2 = (STD_SAMPLE_FORMAT*)realloc(buffer, restSamples * sizeof(STD_SAMPLE_FORMAT));
				if (!tmp2) { 
					perror("stream update - realloc()");
					return true;
				}
				buffer = tmp2;
				memcpy(buffer + (restSamples - count), temp, count * sizeof(STD_SAMPLE_FORMAT));
				free(temp);
			}
		}
		if (stopped) {
			return true;
		}
		ringBuffer.add(buffer, restSamples);
		
		memmove(buffer, buffer + need, (restSamples - need) * sizeof(STD_SAMPLE_FORMAT));
		restSamples -= need;
		return false;
	}

	void Audio::Stream::pause() {
		if (!stopped) {
			stopped = true;
			activeStreamCount--;
		}
	}

	void Audio::Stream::resume() {
		if (stopped) {
			stopped = false;
			activeStreamCount++;
		}
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

	void Audio::audioThread() {
		while (true) {
			while (noup) { printf(""); }
			noup = true;
			update();
		}
	}
}