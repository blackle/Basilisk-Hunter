#pragma once

#include <model/Challenge.h>
#include <vector>
#include <string>
#include <cpr/cpr.h>

class Configuration;

class ServerSession
{
public:
	explicit ServerSession(const Configuration* config);

	std::vector<Challenge> get_challenge_list() const;
	Challenge post_challenge(const Challenge& challenge) const;
	void post_hash_count(uint64_t hashes) const;

private:
	cpr::Header default_headers() const;
	cpr::Url make_url(const std::string& path) const;

	const Configuration* m_config;
	const std::string m_session_key;
};
