#include "SHA256Impl_Naive.h"
#include <stdint.h>
#include <array>
#include <cstring>
#include <crypto/bswap.h>

//code by goshhhy from the original terrible-hash-inverter project
static uint32_t ror32( uint32_t input, size_t shifts ) {
	return ( input >> shifts ) | ( input << ( 32 - shifts ) );
}

static const uint32_t sha256_constants[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void SHA256Impl_Naive::calc_block(SHA256State* state, const SHA256Block* block) const
{
	uint32_t i, s0, s1;
	std::array<uint32_t, 64> w;
	SHA256State h(*state);
	// load data into message schedule
	for ( i = 0; i < 16; i++ )
		w[i] = bswap32( reinterpret_cast<const uint32_t*>(block->data())[i] );

	// fill remainder of message schedule
	for ( i = 16; i < 64; i++ ) {
		s0 = ror32( w[i - 15], 7 ) ^ ror32( w[i - 15], 18 ) ^ ( w[i - 15] >> 3 );
		s1 = ror32( w[i - 2], 17 ) ^ ror32( w[i - 2], 19 ) ^ ( w[i - 2] >> 10 );
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
	}

	// the actual transform
	for ( i = 0; i < 64; i++ ) {
		s1 = ror32( h[4], 6 ) ^ ror32( h[4], 11 ) ^ ror32( h[4], 25 );
		uint32_t ch = ( h[4] & h[5] ) ^ ( ( ~h[4] ) & h[6] );
		uint32_t t1 = h[7] + s1 + ch + sha256_constants[i] + w[i];
		s0 = ror32( h[0], 2 ) ^ ror32( h[0], 13 ) ^ ror32( h[0], 22 );
		uint32_t maj = ( h[0] & h[1] ) ^ ( h[0] & h[2] ) ^ ( h[1] & h[2] );
		uint32_t t2 = s0 + maj;

		h[7] = h[6];
		h[6] = h[5];
		h[5] = h[4];
		h[4] = h[3] + t1;
		h[3] = h[2];
		h[2] = h[1];
		h[1] = h[0];
		h[0] = t1 + t2;
	}
	// add results back to digest
	for ( i = 0; i < 8; i++ )
		(*state)[i] += h[i];

}

bool SHA256Impl_Naive::supported() const
{
	return true;
}

std::string SHA256Impl_Naive::name()
{
	return "naive";
}
