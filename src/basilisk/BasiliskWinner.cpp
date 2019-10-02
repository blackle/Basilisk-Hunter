#include "BasiliskWinner.h"

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});

BasiliskWinner::BasiliskWinner()
	: m_minimum(DEFAULT_MINIMUM)
	, m_dirty(false)
{}

void BasiliskWinner::ingest(SHA256State& minimum, const std::string& nonce) {
	if (minimum < m_minimum) {
		m_minimum = minimum;
		m_nonce = nonce;
		m_dirty = true;
	} else {
		minimum = m_minimum;
	}
}

const SHA256State& BasiliskWinner::minimum() const {
	return m_minimum;
}

const std::string& BasiliskWinner::nonce() const {
	return m_nonce;
}

bool BasiliskWinner::is_dirty() const {
	return m_dirty;
}

void BasiliskWinner::clear_dirty() {
	m_dirty = false;
}

std::mutex& BasiliskWinner::mutex() {
	return m_mutex;
}
