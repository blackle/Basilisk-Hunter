#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256State.h>
#include <model/Challenge.h>
#include <model/Configuration.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <array_ios.h>
#include "ArgumentParser.h"

namespace chrono = std::chrono;

//todo: audit when we print to cout vs cerr, and when we should or shouldn't
int main(int argc, char** argv)
{
	ArgumentParser parser;
	Configuration* config; //todo: put in unique_ptr
	try {
		config = parser.parse(argc, argv);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	if (config == nullptr) {
		return EXIT_SUCCESS;
	}

	//todo: make config builder that parses the args and produces a const Configuration*
	if (config->impl().empty()) {
		std::cout << "No implementation chosen, running tests to automatically pick the best one..." << std::endl;
		config->setImpl(SHA256ImplFactory::get_best_impl_name());
		if (config->impl().empty()) {
			std::cerr << "No available implementations! (this is a bug, please report me)" << std::endl;
			return EXIT_FAILURE;
		}
	} else {
		auto test_impl = SHA256ImplFactory::get_impl(config->impl());
		if (test_impl == nullptr || !test_impl->supported()) {
			std::cerr << "The implementation \"" << config->impl() << "\" does not exist or is unsupported." << std::endl;
			return EXIT_FAILURE;
		}
		delete test_impl;
	}

	std::cout << "Using implementation \"" << config->impl() << "\"" << std::endl;
	std::cout << "Spinning up " << config->threads() << " threads!" << std::endl;

	Challenge challenge("basilisk:0000000000:", 64); //todo: initialize hash with data from server

	WorkerPool workers(&challenge, config);
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
