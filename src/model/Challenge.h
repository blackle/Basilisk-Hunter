#pragma once

#include <crypto/SHA256State.h>

//todo: make more like Configuration class with setters and default constructor
class Challenge {
public:
	Challenge();
	Challenge(const std::string& id, const std::string& prefix, unsigned nonce_length);

	void set_id(const std::string& id);
	const std::string& id() const;

	void set_prefix(const std::string& prefix);
	const std::string& prefix() const;

	void set_nonce_length(unsigned length);
	unsigned nonce_length() const;

	//todo: make nonce+hash a "solution" model class?
	void set_hash_and_nonce(const SHA256State& hash, const std::string& nonce);
	const SHA256State& best_hash() const;
	const std::string& best_nonce() const;

	//todo: can this just be defined in the SharedChallenge? make set_hash_and_nonce protected with friends?
	virtual bool nominate(SHA256State& hash, const std::string& nonce);

private:
	SHA256State m_hash;
	std::string m_nonce;

	std::string m_id;
	std::string m_prefix;
	unsigned m_nonce_length;
};
