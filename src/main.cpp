#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <Basilisk.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "NonceUtil.h"

namespace chrono = std::chrono;

constexpr int BENCHMARK_ROUNDS = 10000000;

//TODO add ostream operators to sha256 classes

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << NonceUtil::build(1) << std::endl;
	SHA256Block nonceBlock(NonceUtil::build(2), 2);
	for (int k = 0; k < 100; k++){
	NonceUtil::increment(nonceBlock.begin(), nonceBlock.content_end());
	for (auto i = nonceBlock.begin(); i != nonceBlock.content_end(); i++) {
		std::cout << *i;
	}
	std::cout << std::endl;
	}

	SHA256State state;
	SHA256Block block("", 0);

	auto best = SHA256ImplFactory::get_best_impl_name();
	auto compressor = SHA256ImplFactory::get_impl(best).release();
	if (!compressor) {
		return -1;
	}

	compressor->calc_block(&state, &block);
	SHA256Block digest(SHA256_DIGEST_SIZE, SHA256_DIGEST_SIZE);
	state.digest(digest);
	for (auto i = digest.begin(); i != digest.end(); i++) {
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)*i;
	}
	std::cout << std::endl;


	Basilisk basilisk(compressor, "", 0);

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	for (int i = 0; i < BENCHMARK_ROUNDS; i++) {
		basilisk.step();
	}
	chrono::time_point<chrono::system_clock> stop = chrono::system_clock::now();

	auto time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
	std::cout << "time: " << (BENCHMARK_ROUNDS/(float)time * 1000./1000000.) << std::endl;

	return 0;
}
