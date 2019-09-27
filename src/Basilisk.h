#pragma once

#include <crypto/SHA256.h>
#include <crypto/SHA256Impl.h>
#include <string>

class Basilisk {
public:
	explicit Basilisk(const SHA256Impl* sha, const std::string& prefix, int nonce_length);
	void step();

private:
	const SHA256Impl* m_sha;

	sha256_ctx m_ctx_initial;
	sha256_ctx m_ctx_working;
	sha256_ctx m_ctx_final;

	sha256_block m_block_nonce;
	sha256_block m_block_final;
};
