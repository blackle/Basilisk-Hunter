#include "ServerSession.h"
#include "ChallengeValidator.h"
#include <json/Challenge_json.h>
#include <model/Configuration.h>
#include <util/NonceUtil.h>
#include <util/UrlParser.h>
#include <nlohmann/json.hpp>
#include <version.h>

using json = nlohmann::json;

ServerSession::ServerSession(const Configuration* config)
	: m_config(config)
	, m_session_key(NonceUtil::build(20))
	, m_headers({
		{"Basilisk-Session-Key", m_session_key},
		{"Basilisk-User-Name", httplib::detail::base64_encode(m_config->name())},
		{"User-Agent", USER_AGENT_STRING}
	})
{
	UrlParser parser(config->server());
	if (!parser.is_valid()) {
		throw std::runtime_error("Server url is malformed.");
	}
	m_client.reset(new httplib::Client(parser.hostname().c_str(), parser.port(), 10));
}

//todo: put requests in their own classes, one for each endpoint
std::vector<Challenge> ServerSession::get_challenge_list() const
{
	auto response = m_client->Get("/challenges/", m_headers);

	if (!response || response->status != 200) {
		throw std::runtime_error("Error getting challenge list from server.");
	}

	auto challenge_list = json::parse(response->body).get<std::vector<Challenge>>();

	ChallengeValidator validator(m_config);
	for (const auto &challenge : challenge_list) {
		if (!validator.validate(challenge)) {
			throw std::runtime_error("Server has an invalid solution.");
		}
	}

	return challenge_list;
}

Challenge ServerSession::post_challenge(const Challenge& challenge) const
{
	auto challenge_url = "/challenges/" + challenge.id();
	auto challenge_json = json(challenge).dump();
	auto response = m_client->Post(challenge_url.c_str(), m_headers, challenge_json, "application/json");

	if (!response || response->status != 200) {
		throw std::runtime_error("Error sending challenge to server.");
	}

	auto new_challenge = json::parse(response->body).get<Challenge>();
	ChallengeValidator validator(m_config);
	if (!validator.validate(new_challenge)) {
		throw std::runtime_error("Server has an invalid solution.");
	}

	return new_challenge;
}

void ServerSession::post_hash_count(uint64_t hashes) const
{
	(void) hashes;
}
