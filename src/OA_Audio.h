#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>

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
		/// <summary>
		/// PCM �����͸� ��� ����� �ҽ��Դϴ�. ����� ��� Stream ��ü�� �����˴ϴ�.
		/// ���/���׷����� �����մϴ�.
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
		private:
			short* ch1 = nullptr, * ch2 = nullptr;	// ch1: ��� ���� ������ ä������ ���׷��� ���� ���� ä��. ch2: ��� ���� �̻��, ���׷��� ���� ���� ä��
			~Source();
		};
		/// <summary>
		/// ���� ����ǰ� �ִ� �Ҹ��Դϴ�.
		/// �ߴ�/�簳/������ �� �� �ֽ��ϴ�.
		/// </summary>
		class Stream {
		public:
		private:
		};
	private:
		
		std::map<std::string, int> source;
		static float master;
	};
}

#endif // !__OA_AUDIO_H__
