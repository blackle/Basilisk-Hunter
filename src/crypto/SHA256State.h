#pragma once

#include <array>
#include "alignment.h"

constexpr unsigned SHA256_STATE_SIZE = 8;

class CRYPTO_ALIGNMENT SHA256State : public std::array<uint32_t, SHA256_STATE_SIZE>
{
public:
	SHA256State();
	explicit SHA256State(const std::array<uint32_t, SHA256_STATE_SIZE>& init);
	void reset();

private:
	typedef std::array<uint32_t, SHA256_STATE_SIZE> super;
};

bool operator<(const SHA256State& a, const SHA256State& b);
