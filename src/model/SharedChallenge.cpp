#include "SharedChallenge.h"

SharedChallenge::SharedChallenge(const Challenge& challenge)
	: super(challenge)
	, m_dirty(false)
{}

std::mutex& SharedChallenge::mutex()
{
	return m_mutex;
}

bool SharedChallenge::is_dirty() const
{
	return m_dirty;
}

void SharedChallenge::set_dirty(bool dirty)
{
	m_dirty = dirty;
}
