#include "ServerSession.h"
#include <json/Challenge_json.h>
#include <model/Configuration.h>
#include <util/NonceUtil.h>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ServerSession::ServerSession(const Configuration* config)
	: m_config(config)
	, m_session_key(NonceUtil::build(20))
{}

std::vector<Challenge> ServerSession::get_challenge_list() const
{
	cpr::Session session;
	init_session(session, "/challenges/");
	auto response = session.Get();

	if (response.status_code != 200) {
		throw std::runtime_error("Error getting challenge list from server.");
	}

	//todo: validate challenges here
	auto challenge_list = json::parse(response.text).get<std::vector<Challenge>>();

	return challenge_list;
}

void ServerSession::update_challenge(Challenge& challenge) const
{
	cpr::Session session;
	init_session(session, "/challenges/" + challenge.id());
	(void) challenge;
}

void ServerSession::send_hash_count(uint64_t hashes) const
{
	(void) hashes;
}

void ServerSession::init_session(cpr::Session& session, const std::string& path) const {
	//todo: set user agent to string based on version
	session.SetOption(cpr::Header{
		{"Basilisk-Session-Key", m_session_key},
		{"Basilisk-User-Name", m_config->name()},
		{"User-Agent", "basilisk-client/0.0.0"}
	});
	session.SetOption(cpr::Url{m_config->server() + path});
}
