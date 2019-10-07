#include "Solution.h"

static const SHA256State DEFAULT_HASH({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});

Solution::Solution()
	: m_hash(DEFAULT_HASH)
{}

Solution::Solution(const SHA256State& hash, const std::string& nonce)
	: m_hash(hash)
	, m_nonce(nonce)
{}

void Solution::set_hash(const SHA256State& hash) {
	m_hash = hash;
}

const SHA256State& Solution::hash() const {
	return m_hash;
}

void Solution::set_nonce(const std::string& nonce) {
	m_nonce = nonce;
}

const std::string& Solution::nonce() const {
	return m_nonce;
}
