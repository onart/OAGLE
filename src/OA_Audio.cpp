/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Audio.h"
#ifdef OA_VORBIS_ONLY
#include "oagle_simd.h"

#include "externals/portaudio/portaudio.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <thread>
#include <mutex>
#include <condition_variable>

#ifdef _WIN64
	#pragma comment(lib, "externals/portaudio/portaudio64.lib")
	#pragma comment(lib, "externals/libvorbis/libvorbisfile64.lib")
	#pragma comment(lib, "externals/libvorbis/libvorbis64.lib")
	#pragma comment(lib, "externals/libvorbis/ogg64.lib")
#elif defined(_WIN32)
	#pragma comment(lib, "externals/portaudio/portaudio32.lib")
	#pragma comment(lib, "externals/libvorbis/libvorbisfile32.lib")
	#pragma comment(lib, "externals/libvorbis/libvorbis32.lib")
	#pragma comment(lib, "externals/libvorbis/ogg32.lib")
	#pragma warning(disable: 4244)
#else

#endif

namespace onart {
	std::vector<pAudioSource> Audio::src;
	std::map<std::string, size_t> Audio::n2i;
	void* Audio::masterStream;
	float Audio::master = 1;
	const float& Audio::masterVolume = Audio::master;
	int Audio::Stream::activeStreamCount = 0;
	const int& Audio::Stream::activeCount = Audio::Stream::activeStreamCount;
	bool Audio::noup = true;

	static std::condition_variable aCV, iCV;	// aCV: 오디오 업데이트 조건변수, iCV: 오디오 인터럽트 조건변수
	static std::mutex audioMutex, interMutex;	// audioMutex: 오디오 업데이트 뮤텍스, interMutex: 오디오 인터럽트 뮤텍스
	bool Audio::wait = false;

	static class RingBuffer {
	public:
		unsigned long writable();
		void add(int16_t* in, unsigned long max, unsigned long to = 0);	// 스택을 사용하기 위하여 만일을 대비한 더하기 오프셋을 추가
		void addComplete();
		void read(void* out, unsigned long count);
	private:
		unsigned long readIndex = 0;	// 콜백에서 읽는 기준
		unsigned long limitIndex = 0;	// 쓰기 제한 기준
		unsigned long writeIndex = 0;	// 쓰기 시작점
		bool isFirst = true;
		bool isFirst2 = true;
		alignas(16) int16_t body[RINGBUFFER_SIZE] = { 0, };	// 약 6프레임 분량
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

	void RingBuffer::addComplete() {	// 인덱스 맞춤
		isFirst = true;
		isFirst2 = true;
		writeIndex = limitIndex;
	}

	void RingBuffer::add(int16_t* in, unsigned long max, unsigned long to) {
		unsigned long writeIndex = this->writeIndex + to;
		if (isFirst || isFirst2) {
			if (limitIndex >= writeIndex) {
				memcpy(body + writeIndex, in, max * sizeof(int16_t));
			}
			else {
				if (RINGBUFFER_SIZE <= writeIndex) writeIndex %= RINGBUFFER_SIZE;
				if (limitIndex >= writeIndex) {
					memcpy(body + writeIndex, in, max * sizeof(int16_t));
				}
				else {
					unsigned long w = RINGBUFFER_SIZE - writeIndex;
					if (w > max) {
						memcpy(body + writeIndex, in, max * sizeof(int16_t));
					}
					else {
						memcpy(body + writeIndex, in, w * sizeof(int16_t));
						memcpy(body, in, (max - w) * sizeof(int16_t));
					}
				}
			}
			if (!isFirst && to == 0) isFirst2 = false;
			isFirst = false;
			return;
		}
		else {
			if (limitIndex >= writeIndex) {
				addsAll(body + writeIndex, in, max);
			}
			else {
				if (RINGBUFFER_SIZE <= writeIndex) writeIndex %= RINGBUFFER_SIZE;
				if (limitIndex >= writeIndex) {
					addsAll(body + writeIndex, in, max);
				}
				else {
					unsigned long w = RINGBUFFER_SIZE - writeIndex;
					if (w > max) {
						addsAll(body + writeIndex, in, max);
					}
					else {
						addsAll(body + writeIndex, in, w);
						addsAll(body, in, max - w);
					}
				}
			}
		}

	}

	void Audio::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		err = Pa_OpenDefaultStream(&masterStream, 0, 2, paInt16, STD_SAMPLE_RATE, paFramesPerBufferUnspecified, Audio::playCallback, nullptr);
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 스트림 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}

		err = Pa_StartStream(masterStream);
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 스트림 시작에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
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

	void Audio::allow(bool go) {
		if constexpr (!OA_AUDIO_NOTHREAD) {
			if (go) {
				aCV.notify_one();
				noup = false;
			}
			else {
				noup = true;
			}
		}
	}

	void Audio::Source::setVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		volume = v;
	}

	pAudioSource Audio::Source::load(const std::string& file, std::string name) {
		if (name.size() == 0) { name = file; }
		if (Audio::n2i.find(name) != Audio::n2i.end()) { return Audio::src[Audio::n2i[name]]; }
		OggVorbis_File vf;
		if (ov_fopen(file.c_str(), &vf) != 0) {
			fprintf(stderr, "유효하지 않은 파일입니다.");
			return pAudioSource();
		}
		vorbis_info* vi = ov_info(&vf, -1);
		if (vi->channels != 2 || vi->rate != STD_SAMPLE_RATE) {
			fprintf(stderr, "조건에 맞지 않는 Vorbis 파일입니다. 성능을 위해 샘플 레이트 %d, 채널 수 %d를 맞춰 주세요.\n", STD_SAMPLE_RATE, 2);
			return pAudioSource();
		}
		Audio::n2i[name] = src.size();
		struct pubSrc :public Source { pubSrc(OggVorbis_File* _1) :Source(_1) {}; };
		src.push_back(std::make_shared<pubSrc>(&vf));
		return *src.rbegin();
	}

	/// <summary>
	/// 가상 vorbis파일에 대한 읽기 콜백
	/// </summary>
	size_t Audio::Source::ovread(void* ptr, size_t size, size_t nmemb, void* datasource) {
		Oggsrc* ds = (Oggsrc*)datasource;
		size_t len = size * nmemb;
		if (ds->cursor + len > ds->size) {
			len = ds->size - ds->cursor;
		}
		memcpy(ptr, ds->data + ds->cursor, len);
		ds->cursor += len;
		return len;
	}

	/// <summary>
	/// 가상 vorbis파일에 대한 탐색 콜백
	/// </summary>
	int Audio::Source::ovseek(void* datasource, ogg_int64_t offset, int whence) {
		Oggsrc* ds = (Oggsrc*)datasource;
		switch (whence)
		{
		case SEEK_SET:
			ds->cursor = offset;
			break;
		case SEEK_CUR:
			ds->cursor += offset;
			break;
		case SEEK_END:
			ds->cursor = ds->size + offset;
			break;
		default:
			return -1;
		}
		return 0;
	}

	/// <summary>
	/// 가상 vorbis파일에 대한 위치 알림 콜백
	/// </summary>
	long Audio::Source::ovtell(void* datasource) {
		Oggsrc* ds = (Oggsrc*)datasource;
		return (long)(ds->cursor);
	}

	pAudioSource Audio::Source::load(void* mem, size_t size, const std::string& name) {
		if (Audio::n2i.find(name) != Audio::n2i.end()) { return Audio::src[Audio::n2i[name]]; }
		OggVorbis_File temp;
		Oggsrc temps{ (unsigned char*)mem,size };
		int code = ov_test_callbacks(&temps, &temp, nullptr, 0, { ovread, ovseek, nullptr, ovtell });
		if (code != 0) {
			fprintf(stderr, "유효하지 않은 파일입니다.");
			return pAudioSource();
		}
		vorbis_info* info = ov_info(&temp, -1);
		if (info->channels != 2 || info->rate != STD_SAMPLE_RATE) {
			fprintf(stderr, "조건에 맞지 않는 Vorbis 파일입니다. 성능을 위해 샘플 레이트 %d, 채널 수 %d를 맞춰 주세요.\n", STD_SAMPLE_RATE, 2);
		}
		struct pubSrc :public Source { pubSrc(unsigned char* _1, size_t _2) :Source(_1, _2) {}; };
		Audio::n2i[name] = src.size();
		src.push_back(std::make_shared<pubSrc>((unsigned char*)mem, size));
		ov_clear(&temp);
		return *src.rbegin();
	}

	Audio::Source::Source(OggVorbis_File* src) :vf(*src), memsrc() { }

	Audio::Source::Source(unsigned char* data, size_t size)
		:memsrc{ data,size } {
		ov_open_callbacks(&memsrc, &vf, nullptr, 0, { ovread, ovseek, nullptr, ovtell });
	}

	Audio::Source::~Source() { ov_clear(&vf); }

	void Audio::Source::drop(const std::string& name) {
		n2i.erase(name);
	}

	void Audio::Source::collect(bool removeUsing) {
		if (removeUsing) {
			n2i.clear();
			for (auto& p : Audio::src) { p.reset(); }
			return;
		}
		for (auto iter = n2i.cbegin(); iter != n2i.cend();) {
			auto& p = Audio::src[iter->second];
			if (p.use_count() == 1) {
				n2i.erase(iter++);
				p.reset();
			}
			else ++iter;
		}
	}

	int Audio::Source::getSamples(int64_t& offset, int count, int16_t* dst) {
		if (ov_pcm_tell(&vf) != offset) {
			ov_pcm_seek(&vf, offset);
		}
		int currentSection;
		int n = ov_read(&vf, (char*)dst, count * sizeof(int16_t), 0, sizeof(int16_t), 1, &currentSection);
		offset = ov_pcm_tell(&vf);
		if (n < 0) return -1;
		return n;
	}

	/// "PCM"을 재생하도록 하는 콜백 함수
	int Audio::playCallback(const void* input, void* output, unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData) {
		if constexpr (OA_AUDIO_WAIT_ON_DRAG)
			if (wait) { memset(output, 0, frameCount * 2 * sizeof(int16_t)); return PaStreamCallbackResult::paContinue; }
		ringBuffer.read(output, frameCount * 2);
		iCV.notify_one();
		return PaStreamCallbackResult::paContinue;	// 정지 신호 외에 정지하지 않음
	}

	Audio::Stream::Stream(Source* src, bool loop)
		:src(src), loop(loop) {
		activeStreamCount++;
	}

	Audio::Stream::~Stream() {
		if (!stopped) activeStreamCount--;
	}

	void Audio::update() {
		std::unique_lock ul(interMutex);
		iCV.wait(ul);
		bool reap = false;
		for (auto& s : src) {
			if (s)s->update();
			else reap = true;
		}
		ringBuffer.addComplete();
		if (reap) { src.erase(std::remove_if(src.begin(), src.end(), [](std::shared_ptr<Source>& x) { return !bool(x); }), src.end()); }
	}

	pAudioSource Audio::Source::get(const std::string& name) {
		if (n2i.find(name) != n2i.end())return src[n2i[name]];
		else return pAudioSource();
	}

	void Audio::Source::update() {
		bool reap = false;
		size_t sz = playing.size();	// 중간 삽입된 것은 다음 업데이트에 적용 (타이밍상 그럴 가능성은 희박)
		for (size_t i = 0; i < sz; i++) {
			auto& strm = playing[i];
			if (strm->update()) {
				strm.reset();
				reap = true;
			}
		}
		if (reap) { playing.erase(std::remove_if(playing.begin(), playing.end(), [](std::shared_ptr<Stream>& x) { return !bool(x); }), playing.end()); }
	}

	void RingBuffer::read(void* out, unsigned long count) {
		unsigned long r1 = readIndex + count > RINGBUFFER_SIZE ? RINGBUFFER_SIZE - readIndex : count;
		memcpy(out, body + readIndex, r1 * sizeof(int16_t));
		if (Audio::Stream::activeCount == 0) memset(body + readIndex, 0, r1 * sizeof(int16_t));
		readIndex += count;
		if (readIndex >= RINGBUFFER_SIZE) {
			readIndex = count - r1;
			memcpy((int16_t*)out + r1, body, readIndex * sizeof(int16_t));
			if (Audio::Stream::activeCount == 0) memset(body, 0, readIndex * sizeof(int16_t));
		}
	}

	bool Audio::Stream::update() {
		if (stopped) return false;
		if (offset == -1) return true;
		unsigned long need = ringBuffer.writable();
		bool isOver = false;
		constexpr unsigned long BUF_LEN = 3072;
		int16_t temp[BUF_LEN];
		int cursor = 0;
		unsigned long addTo = 0;
		while (need) {
			int count = src->getSamples(offset, std::min(need, BUF_LEN - cursor), temp + cursor);	// 바이트 수
			if (count == 0) {
				if (loop) { offset = 0; }
				else {
					isOver = true;
					stopped = true;
					activeStreamCount--;
					break;
				}
			}
			cursor += count / 2;
			need -= count / 2;
			if (need == 0 || BUF_LEN == cursor || isOver) {
				if (src->volume * Audio::masterVolume != 1.0f) {
					mulAll(temp, src->volume * Audio::masterVolume, cursor);
				}
				ringBuffer.add(temp, cursor, addTo);
				if (BUF_LEN == cursor) {	// 성능을 위해, 가급적 이 분기에 오지 않게 컴파일 타임에 적절히 조절하기 바랍니다. 스택 모자라면 temp를 데이터(전역)로 옮기세요.
					addTo += BUF_LEN;
					cursor = 0;
				}
			}
		}
		return isOver;
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
		offset = 0;
	}

	void Audio::Stream::end() {
		loop = false;
		offset = -1;
	}

	Audio::Stream* Audio::Source::play(bool loop) {
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		strm* nw = new strm(this, loop);
		playing.push_back(std::shared_ptr<strm>(nw));
		return nw;
	}

	Audio::SafeStreamPointer Audio::Source::playSafe(bool loop) {
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		auto sm = std::shared_ptr<strm>(new strm(this, loop));
		playing.push_back(sm);
		return SafeStreamPointer(reinterpret_cast<std::shared_ptr<Stream>&>(sm));
	}

	void Audio::audioThread() {
		while (true) {
			std::unique_lock ul(audioMutex);
			if (noup) {
				aCV.wait(ul);
			}
			update();
			ul.unlock();
		}
	}

	Audio::SafeStreamPointer::SafeStreamPointer(std::shared_ptr<Stream>& strm) :wp(strm) {  }
}

#else
#include "oagle_simd.h"

#include "externals/portaudio/portaudio.h"

extern "C" {
#include "externals/ffmpeg/libavcodec/avcodec.h"
#include "externals/ffmpeg/libavformat/avformat.h"
#include "externals/ffmpeg/libswresample/swresample.h"
#include "externals/ffmpeg/libavutil/opt.h"
#include "externals/ffmpeg/libavutil/log.h"
}

#ifdef _WIN64
	#pragma comment(lib, "externals/portaudio/portaudio64.lib")
	#pragma comment(lib, "externals/ffmpeg/libx8664/avformat.lib")
	#pragma comment(lib, "externals/ffmpeg/libx8664/avcodec.lib")
	#pragma comment(lib, "externals/ffmpeg/libx8664/avutil.lib")
	#pragma comment(lib, "externals/ffmpeg/libx8664/swresample.lib")
	#pragma warning(disable: 4244)
#elif defined(_WIN32)
	#pragma comment(lib, "externals/portaudio/portaudio32.lib")
	#pragma comment(lib, "externals/ffmpeg/libx86/avformat.lib")
	#pragma comment(lib, "externals/ffmpeg/libx86/avcodec.lib")
	#pragma comment(lib, "externals/ffmpeg/libx86/avutil.lib")
	#pragma comment(lib, "externals/ffmpeg/libx86/swresample.lib")
	#pragma warning(disable: 4244 26812 26819)
#else
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

	std::vector<pAudioSource> Audio::src;
	std::map<std::string, size_t> Audio::n2i;
	void* Audio::masterStream;
	float Audio::master = 1;
	const float& Audio::masterVolume = Audio::master;
	int Audio::Stream::activeStreamCount = 0;
	const int& Audio::Stream::activeCount = Audio::Stream::activeStreamCount;
	bool Audio::noup = true;

	static std::condition_variable aCV, iCV;	// aCV: 오디오 업데이트 조건변수, iCV: 오디오 인터럽트 조건변수
	static std::mutex audioMutex, interMutex;	// audioMutex: 오디오 업데이트 뮤텍스, interMutex: 오디오 인터럽트 뮤텍스
	bool Audio::wait = false;

	static class RingBuffer {
	public:
		unsigned long writable();
		void add(STD_SAMPLE_FORMAT* in, unsigned long max);
		void addComplete();
		void read(void* out, unsigned long count);
	private:
		unsigned long readIndex = 0;	// 콜백에서 읽는 기준
		unsigned long limitIndex = 0;	// 쓰기 제한 기준
		unsigned long writeIndex = 0;	// 쓰기 시작점
		bool isFirst = true;
		alignas(16) STD_SAMPLE_FORMAT body[RINGBUFFER_SIZE] = { 0, };	// 약 6프레임 분량
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
			fprintf(stderr, "\n오디오 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		err = Pa_OpenDefaultStream(&masterStream, 0, 2, PA_SAMPLE_FORMAT, STD_SAMPLE_RATE, paFramesPerBufferUnspecified, Audio::playCallback, nullptr);
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 스트림 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}

		err = Pa_StartStream(masterStream);
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 스트림 시작에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
#ifndef _DEBUG
		av_log_set_level(AV_LOG_QUIET);	// 어떤 이유든 ffmpeg 라이브러리의 로그를 보려면 디버그 모드로 컴파일하거나, 이 줄을 주석처리하면 됩니다.
#endif // !_DEBUG
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

	void Audio::allow(bool go) {
		if constexpr (!OA_AUDIO_NOTHREAD) {
			if (go) {
				aCV.notify_one();
				noup = false;
			}
			else {
				noup = true;
			}
		}
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

	pAudioSource Audio::Source::load(const void* mem, size_t size, const std::string& name) {
		if (Audio::n2i.find(name) != Audio::n2i.end()) { return Audio::src[Audio::n2i[name]]; }
		AVFormatContext* fmt = avformat_alloc_context();
		MemorySource* msrc = new MemorySource(mem, size);
		AVIOContext* ioContext = avio_alloc_context(msrc->buf, MEMBUFFER_SIZE, 0, msrc, readMem, nullptr, seekMem);
		fmt->pb = ioContext;
		fmt->flags |= AVFMT_FLAG_CUSTOM_IO;
		if (avformat_open_input(&fmt, "", nullptr, nullptr) < 0) {
			fprintf(stderr, "디멀티플렉싱에 실패했습니다. 파일을 다시 점검해 주세요.\n");
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
			fprintf(stderr, "지원하지 않는 샘플 형식입니다. 파일을 변환하는 것을 고려해 주세요.\n");
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
				fprintf(stderr, "리샘플러 초기화에 실패했습니다. 소스가 로드되지 않습니다.\n");
				avformat_close_input(&fmt);
				return nullptr;
			}
		}
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		av_frame_free(&tempF);
		struct pubSrc :public Source { pubSrc(AVFormatContext* _1, AVCodecContext* _2, SwrContext* _3, int _4, AVIOContext* _5, MemorySource* _6) :Source(_1, _2, _3, _4, _5, _6) {}; };
		Audio::n2i[name] = src.size();
		src.push_back(std::make_shared<pubSrc>(fmt, cctx, resampler, frameCount, ioContext, msrc));
		return *src.rbegin();
	}

	bool Audio::Source::initDemux(AVFormatContext* fmt) {
		if (avformat_find_stream_info(fmt, nullptr) < 0) {
			fprintf(stderr, "스트림 정보를 얻지 못했습니다.\n");
			return false;
		}
		if (fmt->nb_streams <= 0) {
			fprintf(stderr, "음성 파일이 유효하지 않은 것 같습니다. 다시 한 번 확인해 주세요.\n");
			return false;
		}
		AVCodecID cid = fmt->streams[0]->codecpar->codec_id;
		if (cid < AVCodecID::AV_CODEC_ID_FIRST_AUDIO || cid>AVCodecID::AV_CODEC_ID_MSNSIREN) {
			fprintf(stderr, "음성 파일이 유효하지 않은 것 같습니다. 다시 한 번 확인해 주세요.\n");
			return false;
		}
		const AVCodec* codec = avcodec_find_decoder(cid);
		if (!codec) {
			fprintf(stderr, "지원하지 않는 형식인 것 같습니다. 다른 형식으로 변환해 주세요.\n");
			return false;
		}
		fmt->audio_codec = codec;
		return true;
	}

	pAudioSource Audio::Source::load(const std::string& file, const std::string& name) {
		std::string memName(name);
		if (memName.size() == 0) { memName = file; }
		if (Audio::n2i.find(memName) != Audio::n2i.end()) { return Audio::src[Audio::n2i[memName]]; }

		AVFormatContext* fmt = avformat_alloc_context();
		if (avformat_open_input(&fmt, file.c_str(), nullptr, nullptr) < 0) {
			fprintf(stderr, "음성 파일이 없거나 로드에 실패했습니다.\n");
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
		int er = avcodec_open2(cctx, nullptr, nullptr);
		SwrContext* resampler = nullptr;

		AVSampleFormat inputFormat = cctx->sample_fmt;

		if (inputFormat < AVSampleFormat::AV_SAMPLE_FMT_U8 || inputFormat > AVSampleFormat::AV_SAMPLE_FMT_S64P) {
			fprintf(stderr, "지원하지 않는 샘플 형식입니다. 파일을 변환하는 것을 고려해 주세요.\n");
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
				cctx->channel_layout == AV_CH_LAYOUT_STEREO)
			) {
			resampler = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_STEREO, (AVSampleFormat)FF_RESAMPLE_FORMAT, STD_SAMPLE_RATE, cctx->channel_layout, inputFormat, sampleRate, 0, nullptr);
			if (swr_init(resampler) < 0) {
				fprintf(stderr, "리샘플러 초기화에 실패했습니다. 소스가 로드되지 않습니다.\n");
				avformat_close_input(&fmt);
				return nullptr;
			}
		}
		av_seek_frame(fmt, -1, 0, AVSEEK_FLAG_FRAME);
		av_frame_free(&tempF);
		struct pubSrc :public Source { pubSrc(AVFormatContext* _1, AVCodecContext* _2, SwrContext* _3, int _4) :Source(_1, _2, _3, _4) {}; };
		n2i[memName] = src.size();
		src.push_back(std::make_shared<pubSrc>(fmt, cctx, resampler, frameCount));
		return *src.rbegin();
	}

	void Audio::Source::drop(const std::string& name) {
		n2i.erase(name);
	}

	void Audio::Source::collect(bool removeUsing) {
		if (removeUsing) {
			n2i.clear();
			for (auto& p : src) { p.reset(); }
			return;
		}
		for (auto iter = n2i.cbegin(); iter != n2i.cend();) {
			auto& p = src[iter->second];
			if (p.use_count() == 1) {
				n2i.erase(iter++);
				p.reset();
			}
			else ++iter;
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
					auto err = swr_convert_frame(resampler, frm2, frm);
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
		if constexpr (OA_AUDIO_WAIT_ON_DRAG)
			if (wait) { memset(output, 0, frameCount * STD_CHANNEL_COUNT * sizeof(STD_SAMPLE_FORMAT)); return PaStreamCallbackResult::paContinue; }
		ringBuffer.read(output, frameCount * STD_CHANNEL_COUNT);
		iCV.notify_one();
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
		std::unique_lock ul(interMutex);
		iCV.wait(ul);
		bool reap = false;
		for (auto& s : src) {
			if (s)s->update();
			else reap = true;
		}
		ringBuffer.addComplete();
		if (reap) { src.erase(std::remove_if(src.begin(), src.end(), [](std::shared_ptr<Source>& x) { return !bool(x); }), src.end()); }
	}

	pAudioSource Audio::Source::get(const std::string& name) {
		if (n2i.find(name) != n2i.end())return src[n2i[name]];
		else return pAudioSource();
	}

	void Audio::Source::update() {
		bool reap = false;
		size_t sz = playing.size();	// 중간 삽입된 것은 다음 업데이트에 적용 (타이밍상 그럴 가능성은 희박)
		for (size_t i = 0; i < sz; i++) {
			auto& strm = playing[i];
			if (strm->update()) {
				strm.reset();
				reap = true;
			}
		}
		if (reap) { playing.erase(std::remove_if(playing.begin(), playing.end(), [](std::shared_ptr<Stream>& x) { return !bool(x); }), playing.end()); }
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
		bool isOver = false;
		while (restSamples < need) {
			STD_SAMPLE_FORMAT* temp;
			int count = src->getFrame(nextFrame++, &temp);
			if (count == -1) {	// 음원 끝까지 재생함
				if (loop) nextFrame = 0;
				else {
					isOver = true;
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
		ringBuffer.add(buffer, restSamples);
		if (isOver) {
			return true;
		}

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
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		strm* nw = new strm(this, loop);
		playing.push_back(std::shared_ptr<strm>(nw));
		return nw;
	}

	Audio::SafeStreamPointer Audio::Source::playSafe(bool loop) {
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		auto sm = std::shared_ptr<strm>(new strm(this, loop));
		playing.push_back(sm);
		return SafeStreamPointer(reinterpret_cast<std::shared_ptr<Stream>&>(sm));
	}

	void Audio::audioThread() {
		while (true) {
			std::unique_lock ul(audioMutex);
			if (noup) {
				aCV.wait(ul);
			}
			update();
			ul.unlock();
		}
	}

	Audio::SafeStreamPointer::SafeStreamPointer(std::shared_ptr<Stream>& strm) :wp(strm) {
	}
}

#endif