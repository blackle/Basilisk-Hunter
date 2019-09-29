#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <Basilisk.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "NonceUtil.h"

namespace chrono = std::chrono;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto best = SHA256ImplFactory::get_best_impl_name();
	auto compressor = SHA256ImplFactory::get_impl(best).release();
	if (!compressor) {
		return -1;
	}

	Basilisk basilisk(compressor, "basilisk|0000000000|", 64);
	SHA256Digest digest;

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	int hashes = 0;
	while (true) {
		basilisk.step();
		basilisk.digest(&digest);
		hashes++;
		if (digest[0] == 0 && digest[1] == 0 && digest[2] == 0) {
			break;
		}
	}
	chrono::time_point<chrono::system_clock> stop = chrono::system_clock::now();

	auto time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
	std::cout << "time: " << (hashes/(float)time * 1000./1000000.) << std::endl;

	std::cout << basilisk.challenge() << " " << digest << std::endl;

	return 0;
}
