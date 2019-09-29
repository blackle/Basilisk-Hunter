#pragma once

#include <crypto/SHA256.h>
#include <crypto/SHA256Impl.h>
#include <string>
#include <memory>

class Basilisk {
public:
	explicit Basilisk(const SHA256Impl* sha, const std::string& prefix, unsigned nonce_length);
	void step();
	std::string challenge();
	void digest(SHA256Digest* digest) const;

private:
	const SHA256Impl* m_sha;
	std::string m_challenge;

	SHA256State m_ctx_initial;
	SHA256State m_ctx_working;
	SHA256State m_ctx_final;

	std::unique_ptr<SHA256Block> m_block_nonce;
	std::unique_ptr<SHA256Block> m_block_final;
};
