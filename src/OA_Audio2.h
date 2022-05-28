#ifndef __OA_AUDIO2_H__
#define __OA_AUDIO2_H__

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <memory>
#include "externals/libvorbis/vorbisfile.h"

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
	class Audio2 {
	public:
		friend class Game;
	public:
		/// <summary>
		/// �б� ���� ������ �����Դϴ�.
		/// </summary>
		static const float& masterVolume;
		/// <summary>
		/// ������ ������ �����մϴ�. ������ 0~1�Դϴ�.
		/// </summary>
		static void setMasterVolume(float v);

		class Stream;
		class SafeStreamPointer;

		/// <summary>
		/// ����� �����͸� �ҷ����� ����� �ҽ��Դϴ�. ����� ��� Stream ��ü�� �����˴ϴ�.
		/// �ε�, ��ε�, ����� �� �� �ֽ��ϴ�.
		/// 32��Ʈ �ε��Ҽ���, �ʴ� 44100����, ���׷����� �ڵ����� �������ϴ�. Ư�� ���� ������ ���� ����ϴ� ���, STD_SAMPLE_RATE�� �������ν� ���� ����� ������ �� �ֽ��ϴ�.
		/// </summary>
		class Source {
			friend class Stream;
			friend class Audio2;
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
			/// ������ �޸𸮰� ȸ���Ǳ� ���̶�, �÷��� ���̴� ��Ʈ���� �Ҹ��� ���߰� ���� ����ϴ��� �Ҹ��� ���� �ʽ��ϴ�.
			/// "���"�� "���"���� �����ϰ� ���� �����͸� �����ϰ� �ִ� ��ü�� ������ ����ϴ� ������ Ĩ�ϴ�. ��ü/�����͸� �Ҹ��Ű�� �ʰ� ����� �����Ϸ���
			/// �ش� ���� �����ͷ� reset() �Ǵ� swap(pAudioSource()) ���� ȣ���ϸ� �˴ϴ�.
			/// ������ �����͸� ������ ��������� �� �����͸� ���� ��� ������ �������� �ʰ� �Ҹ����� ���, ��Ʈ���� ���̻� �Ҹ��� ���� ������ drop���� ������ �Ҹ�Ǵ� ��� �޸𸮰� ȸ���˴ϴ�.
			/// </summary>
			/// <param name="name">������ ���� �̸�</param>
			static void drop(const std::string& name);

			/// <summary>
			/// �ҷ��� ��� ���� �� ���� ���ǰ� ���� ���� ���� ��� �޸𸮿��� �����մϴ�.
			/// ��� ���� ������ [��� ���߰� ����� ������ ��� �޸𸮿��� ȸ��]����, [�״�� ���ܵ�]�� ������ �� �ֽ��ϴ�.
			/// ������ �޸𸮰� ȸ���Ǳ� ���̶�, �÷��� ���̴� ��Ʈ���� �Ҹ��� ���߰� ���� ����ϴ��� �Ҹ��� ���� �ʽ��ϴ�.
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

			/// <summary>
			/// �ҷ��� ������ ����մϴ�. ����� ��Ʈ���� ���� �����Ͱ� ���ϵǸ�, �̸� �̿��� �ߴ�/�簳/������ �� �� �ֽ��ϴ�. ���� ���θ� ������ �� �ֽ��ϴ�.
			/// ���� ������ �ƴ� ��� �ڵ����� �޸𸮰� �����˴ϴ�. ���� �����ʹ� �޸𸮰� ������ ���Ŀ��� �Լ��� ȣ���� �� ������ �̶� �Լ��� ǥ��� �ƹ� ���۵� ���� �ʽ��ϴ�.
			/// </summary>
			SafeStreamPointer playSafe(bool loop = false);
		private:
			OggVorbis_File vf;

			Source(OggVorbis_File*);
			int getSamples(int64_t& offset, int count, int16_t* dst);
			float volume = 1;
			std::vector<std::shared_ptr<Stream>> playing;
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
			int64_t offset = 0;
			Source* src;
			static int activeStreamCount;
			bool stopped = false;
			bool loop;
		};
		/// <summary>
		/// ���� ����ǰ� �ִ� �Ҹ��Դϴ�. ���� ��Ʈ���� ������ ��쿡�� ����� �� ������ ������ ��Ʈ���� ���Ͽ� �Լ��� ȣ���� ��� �ƹ��� ������ ���� �ʽ��ϴ�.
		/// </summary>
		class SafeStreamPointer {
			friend class Source;
		public:
			inline SafeStreamPointer() = default;
			inline void pause() { auto sp = wp.lock(); if (sp)sp->pause(); }
			inline void resume() { auto sp = wp.lock(); if (sp)sp->resume(); }
			inline void end() { auto sp = wp.lock(); if (sp)sp->end(); }
			inline void restart() { auto sp = wp.lock(); if (sp)sp->restart(); }
		private:
			SafeStreamPointer(std::shared_ptr<Stream>&);
			std::weak_ptr<Stream> wp;
		};
	private:
		static bool noup;
		static bool wait;
		static void audioThread();
		static std::vector<std::shared_ptr<Source>> src;
		static std::map<std::string, size_t> n2i;
		static float master;
		static void* masterStream;
		static int playCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeinfo, unsigned long statusFlags, void* userData);
		static void update();
		static void init();
		static void terminate();
		static void allow(bool);
	};

	using pAudioSource2 = std::shared_ptr<Audio2::Source>;
	using pSafeAudioStream = Audio2::SafeStreamPointer;
}

#endif // !__OA_AUDIO2_H__

