#include "OA_Random.h"

#include <random>
#include <algorithm>
#include <iterator>

namespace onart {
	namespace Random {
		std::random_device rd;
		std::mt19937 gen(rd());
		
		void seed(unsigned sd) {
			if (sd) gen.seed(sd);
			else gen.seed(rd());
		}

		float random() {
			static std::uniform_real_distribution<float> x;
			return x(gen);
		}

		float randomN() {
			static std::normal_distribution<float> n;
			return n(gen);
		}

		std::vector<float> random(int n) {
			std::vector<float> rv(n);
			for (float& f : rv) f = random();
			return rv;
		}

		int irandom(int start, int end) {
			std::uniform_int_distribution<int> x(start, end);
			return x(gen);
		}

		int irandom(int end) {
			return irandom(0, end);
		}

		std::vector<int> irandoms(int end, int n) {
			std::vector<int> rv(n);
			for (int& i : rv) i = irandom(0, end);
			return rv;
		}

		std::vector<int> irandoms(int start, int end, int n) {
			std::vector<int> rv(n);
			for (int& i : rv) i = irandom(start, end);
			return rv;
		}

		bool trial(float f) {
			return random() < f;
		}

		template <class T> void shuffle(std::vector<T>& v) {
			std::shuffle(v.begin(), v.end(), gen);
		}

		template <class T> std::vector<T> sample(const std::vector<T>& v, unsigned n) {
			std::vector<T> rv;
			rv.reserve(n);
			std::sample(v.begin(), v.end(), std::back_inserter(rv), n, gen);
			return rv;
		}
	}
}