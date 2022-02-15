/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
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


