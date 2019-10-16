#include "UrlParser.h"
#include <regex>
#include <sstream>

//behold my beautiful regex! some non-urls are accepted but this is just client-side convenience
static const std::regex URL_REGEX{"(?:(https?)://)?([[:alnum:].-]+)(?::([0-9]+))?/?"};

UrlParser::UrlParser(const std::string& url)
	: m_valid(false)
	, m_port(0)
{
	std::smatch match;
	m_valid = std::regex_match(url, match, URL_REGEX);
	if (m_valid) {
		m_schema = match[1];
		m_hostname = match[2];
		std::stringstream port_ss(match[3]);

		if (m_schema.empty()) {
			m_schema = "http";
		}

		if (port_ss.str().empty()) {
			if (m_schema == "https") {
				m_port = 443;
			} else {
				m_port = 80;
			}
		} else {
			port_ss >> m_port;
		}
	}
}

bool UrlParser::is_valid() const {
	return m_valid;
}

const std::string& UrlParser::schema() const {
	return m_schema;
}

const std::string& UrlParser::hostname() const {
	return m_hostname;
}

unsigned UrlParser::port() const {
	return m_port;
}
