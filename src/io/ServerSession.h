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
	void update_challenge(Challenge& challenge) const;
	void send_hash_count(uint64_t hashes) const;

private:
	void init_session(cpr::Session& session, const std::string& path) const;

	const Configuration* m_config;
	const std::string m_session_key;
};