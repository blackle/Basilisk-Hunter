#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256State.h>
#include <basilisk/Basilisk.h>
#include <basilisk/Challenge.h>
#include <basilisk/WorkerPool.h>
#include <util/ElapsedTimer.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <array_ios.h>
#include <argagg.h>

namespace chrono = std::chrono;

int main(int argc, char** argv)
{
	argagg::parser argparser {{
		{ "help", {"-h", "--help"},
		  "shows this help message", 0},
		{ "version", {"-v", "--version"},
		  "shows the program version string", 0},
		{ "name", {"--name"},
		  "sets your name for the leaderboard (utf-8)", 1},
		{ "threads", {"--threads"},
		  "sets the number of threads to run (default 1)", 1},
		{ "set-impl", {"--set-impl"},
		  "sets the sha256 compression function implementation", 1},
		{ "get-impl", {"--get-impls"},
		  "lists the supported sha256 compression function implementations", 0},
	}};

	argagg::parser_results args;
	try {
		args = argparser.parse(argc, argv);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	if (args["help"]) {
		std::cerr << "usage: " << argv[0] << " [OPTIONS]..." << std::endl;
		std::cerr << "A networked program to search for strings that hash to small values." << std::endl;
		std::cerr << std::endl;
		std::cerr << "Options:" << std::endl;
		std::cerr << argparser;
		return 0;
	}

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

	WorkerPool workers(&challenge, best, threads);

	unsigned batches = workers.batches_computed(); //todo: incorporate hash rate counting into its own class
	ElapsedTimer timer;
	while (true) {
		timer.reset();

		std::this_thread::sleep_for(chrono::seconds(10));

		unsigned new_batches = workers.batches_computed();
		float ms = timer.elapsed<chrono::milliseconds>();
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
