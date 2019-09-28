#include "SHA256Tester.h"
#include <chrono>

namespace chrono = std::chrono;

static const SHA256Digest sha256_zero_sum({
	0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
	0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
	0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55,
});

constexpr int BENCHMARK_ROUNDS = 500000;

bool SHA256Tester::verify(const SHA256Impl* impl)
{
	SHA256State state;
	SHA256Block empty_block("", 0);

	impl->calc_block(&state, &empty_block);

	SHA256Digest digest;
	state.digest(&digest);

	return digest == sha256_zero_sum;
}

int SHA256Tester::benchmark(const SHA256Impl* impl)
{
	SHA256State state;
	SHA256Block block("", 0);

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	for (int i = 0; i < BENCHMARK_ROUNDS; i++) {
		impl->calc_block(&state, &block);
	}
	chrono::time_point<chrono::system_clock> stop = chrono::system_clock::now();

	return chrono::duration_cast<chrono::milliseconds>(stop - start).count();
}
