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

const Solution& Solution::null() {
	static Solution null_solution;
	return null_solution;
}

const SHA256State& Solution::hash() const {
	return m_hash;
}

const std::string& Solution::nonce() const {
	return m_nonce;
}

bool operator==(const Solution &c1, const Solution &c2) {
	return c1.hash() == c2.hash();
}
bool operator!=(const Solution &c1, const Solution &c2) {
	return c1.hash() != c2.hash();
}
bool operator< (const Solution &c1, const Solution &c2) {
	return c1.hash() < c2.hash();
}
bool operator> (const Solution &c1, const Solution &c2) {
	return c1.hash() > c2.hash();
}
