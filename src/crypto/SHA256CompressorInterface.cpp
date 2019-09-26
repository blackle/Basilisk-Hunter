#include "SHA256CompressorInterface.h"
#include <chrono>

static const unsigned char sha256_zero_sum[] = {
	0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
	0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
	0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55,
};

constexpr int BENCHMARK_ROUNDS = 200000;

bool SHA256CompressorInterface::verify() const
{
	sha256_ctx ctx;
	sha256_init(&ctx);
	sha256_block block;
	sha256_pad_block(&block, 0, 0);
	calc_block(&ctx, &block);
	unsigned char digest[SHA256_DIGEST_SIZE];
	sha256_digest(&ctx, digest);

	for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
		if (sha256_zero_sum[i] != digest[i]) {
			return false;
		}
	}

	return true;
}

int SHA256CompressorInterface::benchmark() const
{
	sha256_ctx ctx;
	sha256_init(&ctx);
	sha256_block block;
	sha256_pad_block(&block, 0, 0);

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	for (int i = 0; i < BENCHMARK_ROUNDS; i++) {
		calc_block(&ctx, &block);
	}
	std::chrono::time_point<std::chrono::system_clock> stop = std::chrono::system_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
}