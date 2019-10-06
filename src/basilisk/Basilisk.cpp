#include "Basilisk.h"
#include <util/NonceUtil.h>

constexpr unsigned MIN_ENTROPY_BYTES = 11; //~64 bits is probably enough entropy

Basilisk::Basilisk(const SHA256Impl* sha, const std::string& prefix, unsigned nonce_length)
	: m_sha(sha)
	, m_nonce_length(nonce_length)
{
	m_challenge = prefix + NonceUtil::build(nonce_length);

	auto residual = m_sha->hash_to_padding(&m_ctx_initial, m_challenge);

	if (residual.length() > nonce_length) {
		throw std::runtime_error("Last block contains non-nonce characters");
	}

	if (residual.length() <= MIN_ENTROPY_BYTES) {
		throw std::runtime_error("Not enough entropy in last block");
	}

	m_block_nonce.reset(new SHA256Block(residual, m_challenge.length()));
	m_block_final.reset(new SHA256Block(32,32));
}

void Basilisk::step()
{
	NonceUtil::increment(m_block_nonce->begin(), m_block_nonce->end());

	m_ctx_working = m_ctx_initial;
	m_sha->calc_block(&m_ctx_working, m_block_nonce.get());

	SHA256Impl::copy_state_into_block(&m_ctx_working, m_block_final.get());

	m_ctx_final.reset();
	m_sha->calc_block(&m_ctx_final, m_block_final.get());
}

std::string Basilisk::challenge()
{
	std::copy(m_block_nonce->begin(), m_block_nonce->content_end(), m_challenge.end()-m_block_nonce->content_length());
	return m_challenge;
}

std::string Basilisk::nonce()
{
	auto c = challenge();
	return c.substr(c.length() - m_nonce_length, m_nonce_length);
}

std::string Basilisk::prefix()
{
	auto c = challenge();
	return c.substr(0, c.length() - m_nonce_length);
}

const SHA256State& Basilisk::final_state() const
{
	return m_ctx_final;
}
