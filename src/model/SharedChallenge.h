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

	virtual bool nominate(SHA256State& hash, const std::string& nonce) override;

	bool is_dirty() const;
	void clear_dirty();

private:
	typedef Challenge super;

	bool m_dirty;
	std::mutex m_mutex;
};