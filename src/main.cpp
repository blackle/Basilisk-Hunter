#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <basilisk/Basilisk.h>
#include <basilisk/Challenge.h>
#include <basilisk/WorkerPool.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <array_ios.h>

namespace chrono = std::chrono;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto best = SHA256ImplFactory::get_best_impl_name();
	if (best.empty()) {
		return -1;
	}

	int threads = std::thread::hardware_concurrency();
	if (threads <= 0) {
		threads = 1;
	}
	std::cout << "spinning up " << threads << " threads!" << std::endl;

	Challenge challenge("basilisk:0000000000:", 64); //todo: initialize hash with data from server

	WorkerPool workers(&challenge, best, 100000, threads);

	workers.resume();

	unsigned batches = workers.batches_computed(); //todo: incorporate hash rate counting into its own class
	while (true) {
		auto start = chrono::system_clock::now(); //todo: make a utility "elapsed timer" class

		std::this_thread::sleep_for(chrono::seconds(5));

		unsigned new_batches = workers.batches_computed();
		auto end = chrono::system_clock::now();

		float ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		float mhs = (new_batches-batches)/(ms*1000.0) * workers.batch_size();
		std::cout << "MH/s: " << mhs << std::endl;

		std::lock_guard<std::mutex> lock(challenge.mutex());
		if (challenge.is_dirty()) {
			challenge.clear_dirty();
			//todo: send to server
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << challenge.best_nonce() << " " << challenge.best_hash() << std::endl;
		}
		batches = new_batches;
	}

	return 0;
}
