#include "ServerSession.h"
#include "ChallengeValidator.h"
#include "QEncoder.h"
#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256Impl.h>
#include <json/Challenge_json.h>
#include <model/Configuration.h>
#include <util/NonceUtil.h>
#include <memory>
#include <iostream>
#include <nlohmann/json.hpp>
#include <version.h>

using json = nlohmann::json;

ServerSession::ServerSession(const Configuration* config)
	: m_config(config)
	, m_session_key(NonceUtil::build(20))
{}

std::vector<Challenge> ServerSession::get_challenge_list() const
{
	cpr::Session session;
	session.SetOption(make_url("/challenges/"));
	session.SetOption(default_headers());
	auto response = session.Get();

	if (response.status_code != 200) {
		throw std::runtime_error("Error getting challenge list from server.");
	}

	auto challenge_list = json::parse(response.text).get<std::vector<Challenge>>();

	std::unique_ptr<const SHA256Impl> impl(SHA256ImplFactory::get_impl(m_config->impl()));
	for (auto i = challenge_list.begin(); i != challenge_list.end(); i++) {
		if (!ChallengeValidator::validate(*i, impl.get())) {
			throw std::runtime_error("Server has an invalid solution.");
		}
	}

	return challenge_list;
}

Challenge ServerSession::post_challenge(const Challenge& challenge) const
{
	cpr::Session session;
	auto headers = default_headers();
	headers.insert({"Content-Type", "application/json"});
	session.SetOption(make_url("/challenges/" + challenge.id()));
	session.SetOption(headers);
	session.SetOption(cpr::Body{json(challenge).dump()});

	auto response = session.Post();

	if (response.status_code != 200) {
		throw std::runtime_error("Error sending challenge to server.");
	}

	auto new_challenge = json::parse(response.text).get<Challenge>();
	auto impl = SHA256ImplFactory::get_impl(m_config->impl());
	if (!ChallengeValidator::validate(new_challenge, impl)) {
		throw std::runtime_error("Server has an invalid solution.");
	}

	return new_challenge;
}

void ServerSession::post_hash_count(uint64_t hashes) const
{
	(void) hashes;
}

cpr::Header ServerSession::default_headers() const
{
	return cpr::Header{
		{"Basilisk-Session-Key", m_session_key},
		{"Basilisk-User-Name", QEncoder::encode_utf8(m_config->name())},
		{"User-Agent", USER_AGENT_STRING}
	};
}

cpr::Url ServerSession::make_url(const std::string& path) const {
	return cpr::Url{m_config->server() + path};
}
