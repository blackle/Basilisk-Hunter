#include "SHA256Tester.h"
#include "SHA256Impl.h"
#include <io/array_ios.h>
#include <chrono>
#include <sstream>

namespace chrono = std::chrono;

constexpr int BENCHMARK_ROUNDS = 500000;

bool SHA256Tester::verify(const SHA256Impl* impl)
{
	SHA256State state;
	SHA256Block empty_block("", 0);

	impl->calc_block(&state, &empty_block);

	std::ostringstream os;
	os << state;

	return os.str() == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
}

int SHA256Tester::benchmark(const SHA256Impl* impl)
{
	SHA256State state;
	SHA256Block block("", 0);

	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	for (int i = 0; i < BENCHMARK_ROUNDS; i++) {
		impl->calc_block(&state, &block);
	}
	chrono::time_point<chrono::steady_clock> stop = chrono::steady_clock::now();

	return chrono::duration_cast<chrono::milliseconds>(stop - start).count();
}
