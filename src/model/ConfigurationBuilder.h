#pragma once

class Configuration;

class ConfigurationBuilder {
public:
	static const Configuration* build(int argc, char** argv);
};
