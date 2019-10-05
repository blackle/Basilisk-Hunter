#include "ArgumentParser.h"
#include <model/Configuration.h>
#include <iostream>

//weh, I wanted to use constexpr for this whole thing!
#define DEFAULT_SERVER_DOMAIN "http://basilisk.suricrasia.online/"

ArgumentParser::ArgumentParser() : m_parser({{
	{ "help", {"-h", "--help"},
	  "Shows this help message", 0},
	{ "version", {"--version"},
	  "Shows the program version string", 0},
	{ "name", {"--name"},
	  "Sets your name for the leaderboard (utf-8)", 1},
	{ "server", {"--server"},
	  "Sets the url of the challenge server (default: " DEFAULT_SERVER_DOMAIN ")", 1},
	{ "threads", {"--threads"},
	  "Sets the number of threads to run (default: 1)", 1},
	{ "limit", {"--limit"},
	  "Sets a maximum hash rate in MH/s (default: 0, no limit)", 1},
	{ "impl", {"--impl"},
	  "Sets the SHA256 compression function implementation (default: chosen automatically)", 1},
	{ "list-impls", {"--list-impls"},
	  "Lists the supported SHA256 compression function implementations", 0},
}})
{}

Configuration* ArgumentParser::parse(int argc, char** argv) const
{
	argagg::parser_results args = m_parser.parse(argc, argv);

	if (args["help"]) {
		print_help_message(argv[0]);
		return nullptr;
	}

	if (args["version"]) {
		print_help_message(argv[0]); //todo: print version string
		return nullptr;
	}

	if (args["list-impls"]) {
		print_help_message(argv[0]); //todo: list impls
		return nullptr;
	}

	auto config = new Configuration;
	config->set_threads(args["threads"].as<unsigned>(1));
	config->set_name(args["name"].as<std::string>(""));
	config->set_impl(args["impl"].as<std::string>(""));
	config->set_server(args["server"].as<std::string>(DEFAULT_SERVER_DOMAIN));
	config->set_limit(args["limit"].as<float>(0));
	return config;
}

void ArgumentParser::print_help_message(const std::string& program_name) const
{
	std::cerr << "Usage: " << program_name << " [OPTIONS]..." << std::endl;
	std::cerr << "A networked program to search for strings that hash to small values." << std::endl;
	std::cerr << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << m_parser << std::endl;
}
