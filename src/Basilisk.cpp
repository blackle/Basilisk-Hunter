#include "Basilisk.h"
#include <cstring>

Basilisk::Basilisk(const SHA256Impl* sha, const std::string& prefix, int nonce_length)
	: m_sha(sha)
{
	(void) prefix;
	(void) nonce_length;

	m_block_nonce.reset(new SHA256Block("", 0));

	// sha256_init(&m_ctx_initial);
	// m_sha->calc_block(&m_ctx_initial, &m_block_nonce);
}

void Basilisk::step()
{
	m_ctx_working = m_ctx_initial;
	// std::memcpy(&m_ctx_working, &m_ctx_initial, sizeof(sha256_ctx));
	// // increment_nonce(&block_nonce);
	m_sha->calc_block(&m_ctx_working, m_block_nonce.get());

	m_ctx_final.reset();
	m_sha->calc_block(&m_ctx_final, m_block_nonce.get());

	// sha256_digest(&m_ctx_working, m_block_final.x);

	// sha256_init(&m_ctx_final);
	// m_sha->calc_block(&m_ctx_final, &m_block_final);
}
