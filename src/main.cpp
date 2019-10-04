#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256State.h>
#include <basilisk/Basilisk.h>
#include <basilisk/Challenge.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
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
		{ "impl", {"--impl"},
		  "sets the sha256 compression function implementation", 1},
		{ "get-impl", {"--get-impls"},
		  "lists the supported sha256 compression function implementations", 0},
	}};

	argagg::parser_results args;
	try {
		args = argparser.parse(argc, argv);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	if (args["help"]) {
		std::cerr << "usage: " << argv[0] << " [OPTIONS]..." << std::endl;
		std::cerr << "A networked program to search for strings that hash to small values." << std::endl;
		std::cerr << std::endl;
		std::cerr << "Options:" << std::endl;
		std::cerr << argparser;
		return EXIT_SUCCESS;
	}

	auto best = SHA256ImplFactory::get_best_impl_name();
	if (best.empty()) {
		std::cerr << "No available implementations (this is a bug, please report me)" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Using implementation \"" << best << "\"" << std::endl;

	unsigned threads = args["threads"].as<unsigned>(1);
	std::cout << "spinning up " << threads << " threads!" << std::endl;

	Challenge challenge("basilisk:0000000000:", 64); //todo: initialize hash with data from server

	WorkerPool workers(&challenge, best, threads);
	HashSpeedometer speedometer(&workers);

	while (true) {
		speedometer.start();

		std::this_thread::sleep_for(chrono::seconds(10));

		std::cout << "MH/s: " << speedometer.million_hashes_per_second() << std::endl;

		std::lock_guard<std::mutex> lock(challenge.mutex());
		if (challenge.is_dirty()) {
			challenge.clear_dirty();
			//todo: send to server
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << challenge.best_nonce() << " " << challenge.best_hash() << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
