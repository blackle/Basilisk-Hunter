#include "Basilisk.h"
#include "NonceUtil.h"
#include <cstring>

constexpr unsigned MIN_ENTROPY_BYTES = 11; //~64 bits is probably enough entropy

Basilisk::Basilisk(const SHA256Impl* sha, const std::string& prefix, unsigned nonce_length)
	: m_sha(sha)
{
	auto challenge = prefix + NonceUtil::build(nonce_length);

	auto residual = challenge;
	while (residual.length() >= SHA256_BLOCK_SIZE) {
		SHA256Block block(residual.substr(0, SHA256_BLOCK_SIZE));
		m_sha->calc_block(&m_ctx_initial, &block);
		residual.erase(0, SHA256_BLOCK_SIZE);
	}

	if (residual.length() > nonce_length) {
		throw "Last block contains non-nonce characters";
	}

	if (residual.length() <= MIN_ENTROPY_BYTES) {
		throw "Not enough entropy in last block";
	}

	m_block_nonce.reset(new SHA256Block(residual, challenge.length()));
	m_block_final.reset(new SHA256Block(32,32));
}

void Basilisk::step()
{
	NonceUtil::increment(m_block_nonce->begin(), m_block_nonce->end());
	m_ctx_working = m_ctx_initial;

	m_sha->calc_block(&m_ctx_working, m_block_nonce.get());

	m_ctx_working.digest(m_block_final.get());
	m_ctx_final.reset();
	m_sha->calc_block(&m_ctx_final, m_block_final.get());
}
