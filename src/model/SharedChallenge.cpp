#include "SharedChallenge.h"

SharedChallenge::SharedChallenge(const Challenge& challenge)
	: super(challenge)
	, m_dirty(false)
{}

std::mutex& SharedChallenge::mutex()
{
	return m_mutex;
}

bool SharedChallenge::nominate(SHA256State& hash, const std::string& nonce)
{
	bool ret = super::nominate(hash, nonce);
	m_dirty = m_dirty || ret;
	return ret;
}

bool SharedChallenge::is_dirty() const
{
	return m_dirty;
}

void SharedChallenge::clear_dirty()
{
	m_dirty = false;
}
