#include "Challenge.h"

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});

Challenge::Challenge(const std::string& prefix, unsigned nonce_length)
	: m_hash(DEFAULT_MINIMUM)
	, m_dirty(false)
	, m_prefix(prefix)
	, m_nonce_length(nonce_length)
{}

void Challenge::nominate(SHA256State& hash, const std::string& nonce) {
	if (hash < m_hash) {
		m_hash = hash;
		m_nonce = nonce;
		m_dirty = true;
	} else {
		hash = m_hash;
	}
}

const std::string& Challenge::prefix() const {
	return m_prefix;
}

unsigned Challenge::nonce_length() const {
	return m_nonce_length;
}

const SHA256State& Challenge::best_hash() const {
	return m_hash;
}

const std::string& Challenge::best_nonce() const {
	return m_nonce;
}

bool Challenge::is_dirty() const {
	return m_dirty;
}

void Challenge::clear_dirty() {
	m_dirty = false;
}

std::mutex& Challenge::mutex() {
	return m_mutex;
}
