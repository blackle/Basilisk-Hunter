#pragma once

#include <string>

class UrlParser {
public:
	explicit UrlParser(const std::string& url);

	bool is_valid() const;
	const std::string& schema() const;
	const std::string& hostname() const;
	unsigned port() const;

private:
	bool m_valid;
	std::string m_schema;
	std::string m_hostname;
	unsigned m_port;
};
