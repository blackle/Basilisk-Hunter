#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256State.h>
#include <model/Challenge.h>
#include <model/Configuration.h>
#include <model/ConfigurationBuilder.h>
#include <basilisk/WorkerPool.h>
#include <basilisk/HashSpeedometer.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <array_ios.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace chrono = std::chrono;
using json = nlohmann::json;

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

	auto r = cpr::Get(cpr::Url{config->server()});
	std::cout << r.url << std::endl;
	std::cout << r.status_code << std::endl;
	std::cout << r.header["content-type"] << std::endl;
	auto response_json = json::parse(r.text);
	std::cout << response_json.dump(4) << std::endl;

	std::cout << "Using implementation \"" << config->impl() << "\"" << std::endl;
	std::cout << "Spinning up " << config->threads() << " threads!" << std::endl;
	if (config->limit() > 0) {
		std::cout << "Rate limiting to " << config->limit() << " MH/s" << std::endl;
	}

	Challenge challenge("basilisk:0000000000:", 64); //todo: initialize hash with data from server

	WorkerPool workers(&challenge, config.get());
	HashSpeedometer speedometer(&workers);

	//todo: graceful exit on ctrl+c
	while (true) {
		speedometer.start();

		std::this_thread::sleep_for(chrono::seconds(10));

		std::cout << "MH/s: " << speedometer.million_hashes_per_second() << std::endl;

		std::lock_guard<std::mutex> lock(challenge.mutex());
		if (challenge.is_dirty()) {
			challenge.clear_dirty();
			//todo: send to server
			//note to self: save best_nonce/best_hash and unlock before synchronously communicating with server (or just do async...)
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << challenge.best_nonce() << " " << challenge.best_hash() << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
