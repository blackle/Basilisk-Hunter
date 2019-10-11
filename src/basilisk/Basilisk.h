#pragma once

#include <crypto/SHA256State.h>
#include <crypto/SHA256Block.h>
#include <crypto/SHA256Impl.h>
#include <string>

class Basilisk {
public:
	explicit Basilisk(const SHA256Impl* sha, const std::string& prefix, unsigned nonce_length);
	void step();

	std::string challenge();
	std::string nonce();
	std::string prefix();

	const SHA256State& final_state() const;

private:
	const SHA256Impl* m_sha;
	int m_nonce_length;
	std::string m_challenge;

	SHA256State m_ctx_initial;
	SHA256State m_ctx_working;
	SHA256State m_ctx_final;

	SHA256Block m_block_nonce;
	SHA256Block m_block_final;
};
