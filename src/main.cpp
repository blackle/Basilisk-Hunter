#include <model/SharedChallenge.h>
#include <model/Configuration.h>
#include <model/ConfigurationBuilder.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
#include <io/ServerSession.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <memory>
#include <array_ios.h>

namespace chrono = std::chrono;

//todo: audit when we print to cout vs cerr, and when we should or shouldn't
//todo: maybe make a configurable logger?
int main(int argc, char** argv)
{
	std::unique_ptr<const Configuration> config;
	try {
		config.reset(ConfigurationBuilder::build(argc, argv));
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	if (!config) {
		return EXIT_SUCCESS;
	}

	// ServerSession session(config.get());
	// session.get_challenge_list();

	std::cout << "Using implementation \"" << config->impl() << "\"" << std::endl;
	std::cout << "Spinning up " << config->threads() << " threads!" << std::endl;
	if (config->limit() > 0) {
		std::cout << "Rate limiting to " << config->limit() << " MH/s" << std::endl;
	}

	SharedChallenge shared_challenge(Challenge("", "basilisk:0000000000:", 64)); //todo: initialize hash with data from server

	WorkerPool workers(&shared_challenge, config.get());
	HashSpeedometer speedometer(&workers);

	//todo: graceful exit on ctrl+c
	while (true) {
		speedometer.start();

		std::this_thread::sleep_for(chrono::seconds(10));

		std::cout << "MH/s: " << speedometer.million_hashes_per_second() << std::endl;

		std::lock_guard<std::mutex> lock(shared_challenge.mutex());
		if (shared_challenge.is_dirty()) {
			shared_challenge.clear_dirty();
			//todo: send to server
			//note to self: save best_nonce/best_hash and unlock before synchronously communicating with server (or just do async...)
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << shared_challenge.best_nonce() << " " << shared_challenge.best_hash() << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
