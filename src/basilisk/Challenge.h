#pragma once

#include <crypto/SHA256State.h>
#include <mutex>
#include <atomic>

class Challenge {
public:
	Challenge(const std::string& prefix, unsigned nonce_length);
	std::mutex& mutex();

	const std::string& prefix() const;
	unsigned nonce_length() const;

	void nominate(SHA256State& hash, const std::string& nonce);
	const SHA256State& best_hash() const;
	const std::string& best_nonce() const;

	bool is_dirty() const;
	void clear_dirty();

private:
	SHA256State m_hash;
	std::string m_nonce;
	std::atomic_bool m_dirty;

	const std::string m_prefix;
	const unsigned m_nonce_length;

	std::mutex m_mutex;
};
