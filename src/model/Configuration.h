#pragma once

#include <string>

class Configuration {
public:
	Configuration();

	void set_impl(const std::string& impl);
	const std::string& impl() const;

	void set_threads(unsigned threads);
	unsigned threads() const;

	void set_name(const std::string& name);
	const std::string& name() const;

	void set_server(const std::string& server);
	const std::string& server() const;

	void set_limit(float limit);
	float limit() const;

private:
	std::string m_impl;
	unsigned m_threads;
	std::string m_name;
	std::string m_server;
	float m_limit;
};
