#pragma once

#include <model/Challenge.h>
#include <vector>
#include <string>
#include <memory>
#include <yhirose/httplib.h>

class Configuration;

class ServerSession
{
public:
	explicit ServerSession(const Configuration* config);

	std::vector<Challenge> get_challenge_list() const;
	Challenge post_challenge(const Challenge& challenge) const;
	void post_hash_count(uint64_t hashes) const;

private:
	const Configuration* m_config;
	std::unique_ptr<httplib::Client> m_client;
	const std::string m_session_key;
	const httplib::Headers m_headers;
};
