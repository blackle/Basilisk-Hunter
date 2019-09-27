#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <Basilisk.h>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace chrono = std::chrono;

constexpr int BENCHMARK_ROUNDS = 10000000;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	SHA256State state;
	SHA256Block block("", 0);
	for (auto i = block.begin(); i != block.end(); i++) {
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)*i << " ";
	}
	std::cout << std::endl;

	auto best = SHA256ImplFactory::get_best_impl_name();
	auto compressor = SHA256ImplFactory::get_impl(best).release();
	if (!compressor) {
		return -1;
	}

	compressor->calc_block(&state, &block);
	for (auto i = state.begin(); i != state.end(); i++) {
		std::cout << std::setfill('0') << std::setw(8) << std::hex << (int)*i;
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
