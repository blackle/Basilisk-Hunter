#pragma once

class Configuration;

class ConfigurationBuilder {
public:
	ConfigurationBuilder() = delete;
	static const Configuration* build(int argc, char** argv);
};
