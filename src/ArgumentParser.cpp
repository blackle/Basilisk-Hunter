#include "ArgumentParser.h"
#include <basilisk/Configuration.h>
#include <iostream>

ArgumentParser::ArgumentParser() : m_parser({{
	{ "help", {"--help"},
	  "shows this help message", 0},
	{ "version", {"--version"},
	  "shows the program version string", 0},
	{ "name", {"--name"},
	  "sets your name for the leaderboard (utf-8)", 1},
	{ "threads", {"--threads"},
	  "sets the number of threads to run (default 1)", 1},
	{ "impl", {"--impl"},
	  "sets the sha256 compression function implementation", 1},
	{ "list-impls", {"--list-impls"},
	  "lists the supported sha256 compression function implementations", 0},
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
	config->setThreads(args["threads"].as<unsigned>(1));
	config->setName(args["name"].as<std::string>(""));
	config->setImpl(args["impl"].as<std::string>(""));
	return config;
}

void ArgumentParser::print_help_message(const std::string& program_name) const
{
	std::cerr << "usage: " << program_name << " [OPTIONS]..." << std::endl;
	std::cerr << "A networked program to search for strings that hash to small values." << std::endl;
	std::cerr << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << m_parser << std::endl;
}
