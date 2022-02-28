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
#include <algorithm>
#include <iterator>
#include <random>

namespace onart {
	/// <summary>
	/// 랜덤 모듈을 간단한 형태로 일부만 추린 네임스페이스입니다. mt19937을 사용합니다.
	/// </summary>
	namespace Random {
		/// <summary>
		/// 모든 난수의 시드를 직접 정합니다. 0은 사용할 수 없습니다.
		/// </summary>
		/// <param name="sd">시드값이며 0을 입력하는 경우 random_device에서 가져옵니다.</param>
		void seed(unsigned sd = 0);
		/// <summary>
		/// 정규 분포에서 float 난수를 얻습니다.
		/// </summary>
		float randomN();
		/// <summary>
		/// 0과 1 사이의 균등 분포에서 난수를 얻습니다.
		/// </summary>
		float random();
		/// <summary>
		/// 주어진 수만큼 0과 1 사이의 균등 분포에서 난수를 얻어 벡터로 리턴합니다.
		/// </summary>
		/// <param name="n">생성할 개수</param>
		std::vector<float> random(size_t n);
		/// <summary>
		/// 0과 주어진 수(포함) 사이의 균등 분포에서 난수를 얻습니다.
		/// </summary>
		int irandom(int end);
		/// <summary>
		/// 주어진 수들(포함) 사이의 균등 분포에서 난수를 얻습니다.
		/// </summary>
		/// <param name="start">범위의 왼쪽 끝값(포함)</param>
		/// <param name="end">범위의 오른쪽 끝값(포함)</param>
		int irandom(int start, int end);
		/// <summary>
		/// 0과 주어진 수(포함) 사이의 균등 분포에서 난수를 주어진 수만큼 얻어 벡터로 리턴합니다.
		/// </summary>
		/// <param name="end">범위의 오른쪽 끝값(포함)</param>
		/// <param name="n">생성할 개수</param>
		std::vector<int> irandoms(int end, size_t n);
		/// <summary>
		/// 주어진 수들(포함) 사이의 균등 분포에서 난수를 주어진 수만큼 얻어 벡터로 리턴합니다.
		/// </summary>
		/// <param name="start">범위의 왼쪽 끝값(포함)</param>
		/// <param name="end">범위의 오른쪽 끝값(포함)</param>
		/// <param name="n">생성할 개수</param>
		std::vector<int> irandoms(int start, int end, size_t n);
		/// <summary>
		/// 주어진 확률로 true를 리턴합니다.
		/// </summary>
		/// <param name="p">true를 리턴할 확률</param>
		bool trial(float p = 0.5f);
		/// <summary>
		/// 전역에서 사용 중인 엔진을 리턴합니다.
		/// </summary>
		std::mt19937& getEngine();
		/// <summary>
		/// 주어진 벡터 혹은 벡터 기반 컨테이너의 값들의 순서를 무작위로 변경합니다.
		/// 원본의 값을 그대로 두고 섞은 결과물을 리턴하는 함수가 아님에 주의하세요.
		/// </summary>
		template <class T> inline void shuffle(std::vector<T>& v) { std::shuffle(v.begin(), v.end(), getEngine()); }
		/// <summary>
		/// 주어진 벡터 혹은 벡터 기반 컨테이너의 값들에서 무작위 n개를 추출하여 리턴합니다.
		/// </summary>
		/// <param name="v">원본 벡터</param>
		/// <param name="n">추출할 수</param>
		template <class T> inline std::vector<T> sample(const std::vector<T>& v, unsigned n) {
			std::vector<T> rv;
			rv.reserve(n);
			std::sample(v.begin(), v.end(), std::back_inserter(rv), n, getEngine());
			return rv;
		}
	}
}

#endif // !__OA_RANDOM_H__


