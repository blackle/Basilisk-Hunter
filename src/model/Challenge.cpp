#include "Challenge.h"

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});

Challenge::Challenge()
	: m_hash(DEFAULT_MINIMUM)
	, m_nonce_length(0)
{}

Challenge::Challenge(const std::string& id, const std::string& prefix, unsigned nonce_length)
	: m_hash(DEFAULT_MINIMUM)
	, m_id(id)
	, m_prefix(prefix)
	, m_nonce_length(nonce_length)
{}

bool Challenge::nominate(SHA256State& hash, const std::string& nonce) {
	if (hash < m_hash) {
		m_hash = hash;
		m_nonce = nonce;
		return true;
	} else {
		hash = m_hash;
		return false;
	}
}

void Challenge::set_id(const std::string& id) {
	m_id = id;
}

const std::string& Challenge::id() const {
	return m_id;
}

void Challenge::set_prefix(const std::string& prefix) {
	m_prefix = prefix;
}

const std::string& Challenge::prefix() const {
	return m_prefix;
}

void Challenge::set_nonce_length(unsigned length) {
	m_nonce_length = length;
}

unsigned Challenge::nonce_length() const {
	return m_nonce_length;
}

void Challenge::set_hash_and_nonce(const SHA256State& hash, const std::string& nonce) {
	m_hash = hash;
	m_nonce = nonce;
}

const SHA256State& Challenge::best_hash() const {
	return m_hash;
}

const std::string& Challenge::best_nonce() const {
	return m_nonce;
}
