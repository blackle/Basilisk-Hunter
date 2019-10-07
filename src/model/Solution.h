#pragma once

#include <crypto/SHA256State.h>

class Solution
{
public:
	Solution();
	Solution(const SHA256State& hash, const std::string& nonce);

	const SHA256State& hash() const;
	const std::string& nonce() const;

private:
	SHA256State m_hash;
	std::string m_nonce;
};
