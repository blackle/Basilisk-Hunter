#include <model/SharedChallenge.h>
#include <model/Configuration.h>
#include <model/ConfigurationBuilder.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
#include <io/ServerSession.h>
#include <io/array_ios.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <memory>

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

	std::cout << "Using implementation \"" << config->impl() << "\"" << std::endl;
	std::cout << "Spinning up " << config->threads() << " threads!" << std::endl;
	if (config->limit() > 0) {
		std::cout << "Rate limiting to " << config->limit() << " MH/s" << std::endl;
	}

	std::cout << "Contacting server..." << std::endl;
	ServerSession session(config.get());
	std::vector<Challenge> challenges;
	try {
		challenges = session.get_challenge_list();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//todo: choose randomly
	SharedChallenge shared_challenge(challenges.at(0));
	{
		std::lock_guard<std::mutex> lock(shared_challenge.mutex());
		std::cout << "Running challenge id: " << shared_challenge.id(lock) << std::endl;
		std::cout << "prefix: \"" << shared_challenge.prefix(lock) << "\" nonce_length: " << shared_challenge.nonce_length(lock) << std::endl;
		std::cout << "best hash: " << shared_challenge.solution(lock).hash() << std::endl;
	}

	WorkerPool workers(&shared_challenge, config.get());
	HashSpeedometer speedometer(&workers);
	ElapsedTimer timer;

	//todo: graceful exit on ctrl+c
	while (true) {
		speedometer.start();

		std::this_thread::sleep_for(chrono::seconds(10));

		std::cout << "MH/s: " << speedometer.million_hashes_per_second() << std::endl;
		std::cout << "MH: " << static_cast<uint64_t>(workers.batches())*workers.batch_size() << std::endl;

		//todo: send to server
		auto new_solution = shared_challenge.get_new_solution();
		if (new_solution != Solution::null()) {
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << new_solution.nonce() << " " << new_solution.hash() << std::endl;
		}

		if (timer.elapsed<chrono::minutes>() > 5) {
			//todo: report number of hashes computed every 5 minutes
		}
	}

	return EXIT_SUCCESS;
}
