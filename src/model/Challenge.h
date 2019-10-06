#pragma once

#include <crypto/SHA256State.h>

//todo: make more like Configuration class with setters and default constructor
class Challenge {
public:
	Challenge(const std::string& id, const std::string& prefix, unsigned nonce_length);

	const std::string& id() const;
	const std::string& prefix() const;
	unsigned nonce_length() const;

	virtual bool nominate(SHA256State& hash, const std::string& nonce);
	const SHA256State& best_hash() const;
	const std::string& best_nonce() const;

private:
	SHA256State m_hash;
	std::string m_nonce;

	const std::string m_id;
	const std::string m_prefix;
	const unsigned m_nonce_length;
};
