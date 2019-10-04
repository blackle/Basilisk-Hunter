#include "Minimizer.h"

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});

Minimizer::Minimizer()
	: m_hash(DEFAULT_MINIMUM)
	, m_dirty(false)
{}

void Minimizer::nominate(SHA256State& hash, const std::string& nonce) {
	if (hash < m_hash) {
		m_hash = hash;
		m_nonce = nonce;
		m_dirty = true;
	} else {
		hash = m_hash;
	}
}

const SHA256State& Minimizer::hash() const {
	return m_hash;
}

const std::string& Minimizer::nonce() const {
	return m_nonce;
}

bool Minimizer::is_dirty() const {
	return m_dirty;
}

void Minimizer::clear_dirty() {
	m_dirty = false;
}

std::mutex& Minimizer::mutex() {
	return m_mutex;
}
