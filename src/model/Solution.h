#pragma once

#include <crypto/SHA256State.h>

class Solution
{
public:
	Solution();
	Solution(const SHA256State& hash, const std::string& nonce);
	static const Solution& null();

	const SHA256State& hash() const;
	const std::string& nonce() const;

private:
	SHA256State m_hash;
	std::string m_nonce;
};

bool operator==(const Solution &c1, const Solution &c2);
bool operator!=(const Solution &c1, const Solution &c2);
bool operator< (const Solution &c1, const Solution &c2);
bool operator> (const Solution &c1, const Solution &c2);
