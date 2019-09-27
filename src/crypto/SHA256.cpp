#include "SHA256.h"
#include <cstring>
#include <assert.h>
#include <arpa/inet.h>
#include <algorithm>
#include <iostream>

// static constexpr uint32_t sha256_h0_old[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
static const std::array<uint32_t, SHA256_STATE_SIZE> sha256_h0 = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

SHA256State::SHA256State()
	: super(sha256_h0)
{}

void SHA256State::reset()
{
	std::copy(sha256_h0.begin(), sha256_h0.end(), begin());
}

void SHA256State::digest(SHA256Digest& digest) {
	(void) digest;
}

void SHA256State::digest(SHA256Block& digest) {
	(void) digest;
}

//endianness is annoying, eh?
static constexpr std::array<uint8_t, 8> pack_64(uint64_t num) {
	return {
		static_cast<uint8_t>((num >> (8*7)) & 0xFF),
		static_cast<uint8_t>((num >> (8*6)) & 0xFF),
		static_cast<uint8_t>((num >> (8*5)) & 0xFF),
		static_cast<uint8_t>((num >> (8*4)) & 0xFF),
		static_cast<uint8_t>((num >> (8*3)) & 0xFF),
		static_cast<uint8_t>((num >> (8*2)) & 0xFF),
		static_cast<uint8_t>((num >> (8*1)) & 0xFF),
		static_cast<uint8_t>((num >> (8*0)) & 0xFF),
	};
}

SHA256Block::SHA256Block(const std::string& data)
{
	m_content_end = data.length();
	if (m_content_end != SHA256_BLOCK_SIZE) {
		throw "Block data is not exactly 64 bytes!";
	}

	std::copy(data.begin(), data.end(), begin());
}

SHA256Block::SHA256Block(const std::string& data, size_type total_length)
{
	m_content_end = data.length();
	if (m_content_end > (SHA256_BLOCK_SIZE - 9)) {
		throw "Cannot pad this block, not enough room!";
	}

	std::copy(data.begin(), data.end(), begin());

	std::fill(content_end(), end(), 0);
	*content_end() = 0x80;

	auto packed_length = pack_64(total_length * 8);
	std::copy(packed_length.begin(), packed_length.end(), begin()+(SHA256_BLOCK_SIZE-8));
}

SHA256Block::iterator SHA256Block::content_end() noexcept
{
	return begin()+m_content_end;
}

SHA256Block::const_iterator SHA256Block::content_end() const noexcept
{
	return begin()+m_content_end;
}

SHA256Block::size_type SHA256Block::content_length() const
{
	return m_content_end;
}

// void sha256_init(sha256_ctx *ctx) {
// 	(void)ctx;
// 	std::memcpy(ctx->s, sha256_h0_old, sizeof(sha256_h0_old));
// }
// void sha256_pad_block(sha256_block * block, int offset, int length) {
// 	assert((SHA256_BLOCK_SIZE - 9) >= offset);

// 	int len_b = length << 3;
// 	int pm_len = 1 << 6;

// 	std::memset(block->x + offset, 0, pm_len - offset);
// 	block->x[offset] = 0x80;
// 	uint32_t* encoded_len = (uint32_t*)(block->x + pm_len - 4);
// 	*encoded_len = htonl(len_b);
// }

// void sha256_digest(sha256_ctx * ctx, unsigned char* digest) {
// 	uint32_t* digest_32 = (uint32_t*)digest;
// 	for (int i = 0 ; i < 8; i++) {
// 		digest_32[i] = htonl(ctx->s[i]);
// 	}
// }
