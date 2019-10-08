#pragma once

#include <vietjtnguyen/argagg.h>

class Configuration;

class ArgumentParser {
public:
	ArgumentParser();

	Configuration* parse(int argc, char** argv) const;
private:
	void print_help_message(const std::string& program_name) const;
	argagg::parser m_parser;
};
