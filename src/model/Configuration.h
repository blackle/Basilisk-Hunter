#pragma once

#include <string>

class Configuration {
public:
	Configuration();

	void setImpl(const std::string& impl);
	const std::string& impl() const;

	void setThreads(unsigned threads);
	unsigned threads() const;

	void setName(const std::string& name);
	const std::string& name() const;

private:
	std::string m_impl;
	unsigned m_threads;
	std::string m_name;
};
