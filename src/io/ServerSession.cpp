#include "ServerSession.h"
#include "Challenge_json.h"
#include <model/Configuration.h>
#include <util/NonceUtil.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <crypto/SHA256ImplFactory.h>

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

	//todo: make parser classes...?
	if (response.status_code != 200) {
		throw std::runtime_error("Error getting challenge list.");
	}

	std::unique_ptr<const SHA256Impl> impl(SHA256ImplFactory::get_impl(m_config->impl()));

	auto list = json::parse(response.text);
	for (json::iterator i = list.begin(); i != list.end(); ++i) {
		auto challenge = i->get<Challenge>();
		//todo: put in parser
		auto hash = impl->double_hash(challenge.prefix() + challenge.best_nonce());
		if (hash != challenge.best_hash()) {
			throw std::runtime_error("Server has an invalid solution!");
		}
		if (challenge.best_nonce().length() != challenge.nonce_length()) {
			throw std::runtime_error("Server has an invalid solution!");
		}
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
