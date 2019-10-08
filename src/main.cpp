#include <model/Challenge.h>
#include <model/Configuration.h>
#include <model/ConfigurationBuilder.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
#include <basilisk/ChallengeOperations.h>
#include <io/ServerSession.h>
#include <io/array_ios.h>
#include <util/LockBox.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <memory>

namespace chrono = std::chrono;

//todo: audit when we print to cout vs cerr, and when we should or shouldn't
//todo: maybe make a configurable logger? we should print out the program name on each line of std::cerr
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
	Challenge challenge = challenges.at(0);
	std::cout << "Running challenge id: " << challenge.id() << std::endl;
	std::cout << "prefix: \"" << challenge.prefix() << "\" nonce_length: " << challenge.nonce_length() << std::endl;
	std::cout << "best server hash: " << challenge.solution().hash() << std::endl;

	LockBox<Challenge> challenge_box(challenge);
	WorkerPool workers(&challenge_box, config.get());
	HashSpeedometer speedometer(&workers);
	ElapsedTimer timer;

	//todo: graceful exit on ctrl+c
	while (true) {
		speedometer.start();

		std::this_thread::sleep_for(chrono::seconds(1));

		std::cout << "MH/s: " << speedometer.million_hashes_per_second() << std::endl;

		Solution new_solution = challenge.solution();
		if (ChallengeOperations::get_new_solution(&challenge_box, new_solution)) {
			challenge.set_solution(new_solution);
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << new_solution.nonce() << " " << new_solution.hash() << std::endl;

			try {
				std::cout << "Sending new solution to server..." << std::endl;

				challenge = session.post_challenge(challenge);
				new_solution = challenge.solution();
				if (!ChallengeOperations::reconcile_solutions(&challenge_box, new_solution)) {
					std::cout << "Our solution was the winner!" << std::endl;
				}

				std::cout << "Solution sent!" << std::endl;
			} catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}

		if (timer.elapsed<chrono::minutes>() > 5) {
			//todo: report number of hashes computed every 5 minutes
			timer.start();
		}
	}

	return EXIT_SUCCESS;
}
