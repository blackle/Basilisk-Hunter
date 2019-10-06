#include "ServerSession.h"
#include <model/Configuration.h>
#include <util/NonceUtil.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ServerSession::ServerSession(const Configuration* config)
	: m_config(config)
	, m_session_key(NonceUtil::build(20))
{}

//todo: separate challenge data from mutex
std::vector<Challenge> ServerSession::get_challenge_list() const
{
	std::vector<Challenge> challenge_list;
	cpr::Session session;
	init_session(session, "/challenges/");
	auto response = session.Get();

	//todo: make parser classes
	if (response.status_code != 200) {
		throw std::runtime_error("Error getting challenge list.");
	}

	auto list = json::parse(response.text);

	if (!list.is_array()) {
		throw std::runtime_error("Challenge list response is not a list.");
	}

	for (json::iterator i = list.begin(); i != list.end(); ++i) {
		auto o = *i;
		if (!o.is_object()) {
			throw std::runtime_error("Challenge object is not an object.");
		}

		Challenge challenge(o["id"], o["prefix"], o["nonce_length"]);
		std::string hash = o["prefix"].get<std::string>() + o["best_nonce"].get<std::string>();
		// challenge.nominate(hash, o["best_nonce"])
		challenge_list.push_back(challenge);
	}

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
	session.SetOption(cpr::Parameters{{"Basilisk-Session-Key", m_session_key}});
	session.SetOption(cpr::Parameters{{"Basilisk-User-Name", m_config->name()}});
	//todo: set user agent to string based on version
	session.SetOption(cpr::Parameters{{"User-Agent", "basilisk-client/0.0.0"}});
	session.SetOption(cpr::Url{m_config->server() + path});
}
