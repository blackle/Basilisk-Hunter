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

const SHA256State& Solution::hash() const {
	return m_hash;
}

const std::string& Solution::nonce() const {
	return m_nonce;
}
