#pragma once

#include <crypto/SHA256.h>
#include <mutex>

class Minimizer {
public:
	Minimizer();
	std::mutex& mutex();

	void nominate(SHA256State& hash, const std::string& nonce);
	const SHA256State& hash() const;
	const std::string& nonce() const;

	bool is_dirty() const;
	void clear_dirty();

private:
	SHA256State m_hash;
	std::string m_nonce;
	std::mutex m_mutex;
	bool m_dirty;
};
