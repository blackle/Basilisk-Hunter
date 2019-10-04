#include "SHA256State.h"
#include <algorithm>

static const std::array<uint32_t, SHA256_STATE_SIZE> sha256_h0 = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

SHA256State::SHA256State()
	: super(sha256_h0)
{}

SHA256State::SHA256State(const std::array<uint32_t, SHA256_STATE_SIZE>& init)
	: super(init)
{}

void SHA256State::reset()
{
	std::copy(sha256_h0.begin(), sha256_h0.end(), begin());
}
