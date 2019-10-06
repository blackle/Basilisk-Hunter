#include "NonceUtil.h"
#include <algorithm>
#include <random>

static constexpr int NONCE_ALPHABET_SIZE = 62;
static constexpr std::array<uint8_t, NONCE_ALPHABET_SIZE> NONCE_ALPHABET ={
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
	'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
	'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '0'
};

std::string NonceUtil::build(int length)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<> dist(0, NONCE_ALPHABET_SIZE-1);

	std::string nonce(length, '0');
	std::generate(nonce.begin(), nonce.end(), [&] {
		return NONCE_ALPHABET[dist(rng)];
	});
	return nonce;
}
