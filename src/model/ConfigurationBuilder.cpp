#include "ConfigurationBuilder.h"
#include "Configuration.h"
#include <io/ArgumentParser.h>
#include <util/UrlParser.h>
#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256Impl.h>
#include <stdexcept>
#include <iostream>

const Configuration* ConfigurationBuilder::build(int argc, char** argv) {
	ArgumentParser arg_parser;
	Configuration* config = arg_parser.parse(argc, argv);
	if (config == nullptr) {
		return nullptr;
	}

	UrlParser url_parser(config->server());
	if (!url_parser.is_valid()) {
		throw std::runtime_error("Server url is invalid.");
	}

	if (config->threads() == 0) {
		throw std::runtime_error("Thread count must be non-zero.");
	}

	if (config->impl().empty()) {
		std::cout << "No implementation chosen, running tests to automatically pick the best one..." << std::endl;
		config->set_impl(SHA256ImplFactory::get_best_impl_name());
		if (config->impl().empty()) {
			throw std::runtime_error("No available implementations! (this is a bug, please report me)");
		}
	} else {
		auto test_impl = SHA256ImplFactory::get_impl(config->impl());
		if (test_impl == nullptr || !test_impl->supported()) {
			throw std::runtime_error("The implementation \"" + config->impl() + "\" does not exist or is unsupported.");
		}
		delete test_impl;
	}

	return config;
}
