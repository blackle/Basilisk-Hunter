#pragma once

#include <model/Challenge.h>
#include <crypto/SHA256State.h>
#include <string>
#include <mutex>

class SharedChallenge : public Challenge
{
public:
	SharedChallenge() = delete;
	SharedChallenge(const Challenge& challenge);

	std::mutex& mutex();

	bool is_dirty() const;
	void set_dirty(bool dirty);

private:
	typedef Challenge super;

	bool m_dirty;
	std::mutex m_mutex;
};