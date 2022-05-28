/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_Audio2.h"
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
#else

#endif

namespace onart {
	std::vector<pAudioSource2> Audio2::src;
	std::map<std::string, size_t> Audio2::n2i;
	void* Audio2::masterStream;
	float Audio2::master = 1;
	const float& Audio2::masterVolume = Audio2::master;
	int Audio2::Stream::activeStreamCount = 0;
	const int& Audio2::Stream::activeCount = Audio2::Stream::activeStreamCount;
	bool Audio2::noup = true;

	static std::condition_variable aCV, iCV;	// aCV: 오디오 업데이트 조건변수, iCV: 오디오 인터럽트 조건변수
	static std::mutex audioMutex, interMutex;	// audioMutex: 오디오 업데이트 뮤텍스, interMutex: 오디오 인터럽트 뮤텍스
	bool Audio2::wait = false;

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

	void Audio2::init() {
		PaError err = Pa_Initialize();
		if (err != PaErrorCode::paNoError) {
			fprintf(stderr, "\n오디오 초기화에 실패했습니다.\n%s\n", Pa_GetErrorText(err));
			return;
		}
		err = Pa_OpenDefaultStream(&masterStream, 0, 2, paInt16, STD_SAMPLE_RATE, paFramesPerBufferUnspecified, Audio2::playCallback, nullptr);
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

	void Audio2::terminate() {
		Pa_CloseStream(masterStream);
		PaError err = Pa_Terminate();
	}

	void Audio2::setMasterVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		master = v;
	}

	void Audio2::allow(bool go) {
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

	void Audio2::Source::setVolume(float v) {
		if (v > 1)v = 1;
		else if (v < 0)v = 0;
		volume = v;
	}

	pAudioSource2 Audio2::Source::load(const std::string& file, std::string name) {
		if (name.size() == 0) { name = file; }
		if (Audio2::n2i.find(name) != Audio2::n2i.end()) { return Audio2::src[Audio2::n2i[name]]; }
		OggVorbis_File vf;
		if (ov_fopen(file.c_str(), &vf) != 0) {
			fprintf(stderr, "유효하지 않은 파일입니다.");
			return pAudioSource2();
		}
		vorbis_info* vi = ov_info(&vf, -1);
		if (vi->channels != 2 || vi->rate != STD_SAMPLE_RATE) {
			fprintf(stderr, "조건에 맞지 않는 Vorbis 파일입니다. 성능을 위해 샘플 레이트 %d, 채널 수 %d를 맞춰 주세요.\n", STD_SAMPLE_RATE, 2);
			return pAudioSource2();
		}
		Audio2::n2i[name] = src.size();
		struct pubSrc :public Source { pubSrc(OggVorbis_File* _1) :Source(_1) {}; };
		src.push_back(std::make_shared<pubSrc>(&vf));
		return *src.rbegin();
	}

	/// <summary>
	/// 가상 vorbis파일에 대한 읽기 콜백
	/// </summary>
	static size_t ovread(void* ptr, size_t size, size_t nmemb, void* datasource) {

	}

	/// <summary>
	/// 가상 vorbis파일에 대한 탐색 콜백
	/// </summary>
	static int ovseek(void* datasource, ogg_int64_t offset, int whence) {

	}

	/// <summary>
	/// 가상 vorbis파일에 대한 닫기 콜백
	/// </summary>
	static int ovclose(void* datasource) {

	}

	/// <summary>
	/// 가상 vorbis파일에 대한 위치 알림 콜백
	/// </summary>
	static long ovtell(void* datasource) {

	}

	pAudioSource2 Audio2::Source::load(const void* mem, size_t size, const std::string& name) {
		if (Audio2::n2i.find(name) != Audio2::n2i.end()) { return Audio2::src[Audio2::n2i[name]]; }
		return pAudioSource2();	// 진행중
	}

	Audio2::Source::Source(OggVorbis_File* src) :vf(*src) { }

	Audio2::Source::~Source() { ov_clear(&vf); }

	void Audio2::Source::drop(const std::string& name) {
		n2i.erase(name);
	}

	void Audio2::Source::collect(bool removeUsing) {
		if (removeUsing) {
			n2i.clear();
			for (auto& p : Audio2::src) { p.reset(); }
			return;
		}
		for (auto iter = n2i.cbegin(); iter != n2i.cend();) {
			auto& p = Audio2::src[iter->second];
			if (p.use_count() == 1) {
				n2i.erase(iter++);
				p.reset();
			}
			else ++iter;
		}
	}

	int Audio2::Source::getSamples(int64_t& offset, int count, int16_t* dst) {
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
	int Audio2::playCallback(const void* input, void* output, unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData) {
		if constexpr (OA_AUDIO_WAIT_ON_DRAG)
		if (wait) { memset(output, 0, frameCount * 2 * sizeof(int16_t)); return PaStreamCallbackResult::paContinue; }
		ringBuffer.read(output, frameCount * 2);
		iCV.notify_one();
		return PaStreamCallbackResult::paContinue;	// 정지 신호 외에 정지하지 않음
	}
	
	Audio2::Stream::Stream(Source* src, bool loop)
		:src(src), loop(loop) {
		activeStreamCount++;
	}

	Audio2::Stream::~Stream() {
		if (!stopped) activeStreamCount--;
	}

	void Audio2::update() {
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

	pAudioSource2 Audio2::Source::get(const std::string& name) {
		if (n2i.find(name) != n2i.end())return src[n2i[name]];
		else return pAudioSource2();
	}

	void Audio2::Source::update() {
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
		if (Audio2::Stream::activeCount == 0) memset(body + readIndex, 0, r1 * sizeof(int16_t));
		readIndex += count;
		if (readIndex >= RINGBUFFER_SIZE) {
			readIndex = count - r1;
			memcpy((int16_t*)out + r1, body, readIndex * sizeof(int16_t));
			if (Audio2::Stream::activeCount == 0) memset(body, 0, readIndex * sizeof(int16_t));
		}
	}

	bool Audio2::Stream::update() {
		if (stopped) return false;
		if (offset == -1) return true;
		unsigned long need = ringBuffer.writable();
		bool isOver = false;
		constexpr unsigned long BUF_LEN = 2048;
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
				if (src->volume * Audio2::masterVolume != 1.0f) {
					mulAll(temp, src->volume * Audio2::masterVolume, cursor);
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

	void Audio2::Stream::pause() {
		if (!stopped) { 
			stopped = true;
			activeStreamCount--;
		}
	}

	void Audio2::Stream::resume() {
		if (stopped) {
			stopped = false;
			activeStreamCount++;
		}
	}

	void Audio2::Stream::restart() {
		offset = 0;
	}

	void Audio2::Stream::end() {
		loop = false;
		offset = -1;
	}

	Audio2::Stream* Audio2::Source::play(bool loop) {
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		strm* nw = new strm(this, loop);
		playing.push_back(std::shared_ptr<strm>(nw));
		return nw;
	}

	Audio2::SafeStreamPointer Audio2::Source::playSafe(bool loop) {
		struct strm :public Stream { strm(Source* _1, bool _2) :Stream(_1, _2) {} };
		auto sm = std::shared_ptr<strm>(new strm(this, loop));
		playing.push_back(sm);
		return SafeStreamPointer(reinterpret_cast<std::shared_ptr<Stream>&>(sm));
	}

	void Audio2::audioThread() {
		while (true) {
			std::unique_lock ul(audioMutex);
			if (noup) {
				aCV.wait(ul);
			}
			update();
			ul.unlock();
		}
	}

	Audio2::SafeStreamPointer::SafeStreamPointer(std::shared_ptr<Stream>& strm) :wp(strm) {  }
}