#ifndef __OA_RANDOM_H__
#define	__OA_RANDOM_H__

#include <vector>

namespace onart {
	/// <summary>
	/// ���� ����� ������ ���·� �Ϻθ� �߸� ���ӽ����̽��Դϴ�. mt19937�� ����մϴ�.
	/// </summary>
	namespace Random {
		/// <summary>
		/// ��� ������ �õ带 ���� ���մϴ�. 0�� ����� �� �����ϴ�.
		/// </summary>
		/// <param name="sd">�õ尪�̸� 0�� �Է��ϴ� ��� random_device���� �����ɴϴ�.</param>
		void seed(unsigned sd = 0);
		/// <summary>
		/// ���� �������� float ������ ����ϴ�.
		/// </summary>
		float randomN();
		/// <summary>
		/// 0�� 1 ������ �յ� �������� ������ ����ϴ�.
		/// </summary>
		float random();
		/// <summary>
		/// �־��� ����ŭ 0�� 1 ������ �յ� �������� ������ ��� ���ͷ� �����մϴ�.
		/// </summary>
		/// <param name="n">������ ����</param>
		std::vector<float> random(int n);
		/// <summary>
		/// 0�� �־��� ��(����) ������ �յ� �������� ������ ����ϴ�.
		/// </summary>
		int irandom(int end);
		/// <summary>
		/// �־��� ����(����) ������ �յ� �������� ������ ����ϴ�.
		/// </summary>
		/// <param name="start">������ ���� ����(����)</param>
		/// <param name="end">������ ������ ����(����)</param>
		int irandom(int start, int end);
		/// <summary>
		/// 0�� �־��� ��(����) ������ �յ� �������� ������ �־��� ����ŭ ��� ���ͷ� �����մϴ�.
		/// </summary>
		/// <param name="end">������ ������ ����(����)</param>
		/// <param name="n">������ ����</param>
		std::vector<int> irandoms(int end, int n);
		/// <summary>
		/// �־��� ����(����) ������ �յ� �������� ������ �־��� ����ŭ ��� ���ͷ� �����մϴ�.
		/// </summary>
		/// <param name="start">������ ���� ����(����)</param>
		/// <param name="end">������ ������ ����(����)</param>
		/// <param name="n">������ ����</param>
		std::vector<int> irandoms(int start, int end, int n);
		/// <summary>
		/// �־��� Ȯ���� true�� �����մϴ�.
		/// </summary>
		/// <param name="p">true�� ������ Ȯ��</param>
		bool trial(float p = 0.5f);
		/// <summary>
		/// �־��� ���� Ȥ�� ���� ��� �����̳��� ������ ������ �������� �����մϴ�.
		/// ������ ���� �״�� �ΰ� ���� ������� �����ϴ� �Լ��� �ƴԿ� �����ϼ���.
		/// </summary>
		template <class T> void shuffle(std::vector<T>& v);
		/// <summary>
		/// �־��� ���� Ȥ�� ���� ��� �����̳��� ���鿡�� ������ n���� �����Ͽ� �����մϴ�.
		/// </summary>
		/// <param name="v">���� ����</param>
		/// <param name="n">������ ��</param>
		template <class T> std::vector<T> sample(const std::vector<T>& v, unsigned n);
	}
}

#endif // !__OA_RANDOM_H__


