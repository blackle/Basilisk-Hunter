#include "SHA256.h"
#include <cstring>
#include <assert.h>
// #include <arpa/inet.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

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
	std::copy(data.begin(), data.end(), begin());
	init_padding(total_length);
}

SHA256Block::SHA256Block(size_type offset, size_type total_length)
{
	m_content_end = offset;
	init_padding(total_length);
}

void SHA256Block::init_padding(size_type total_length)
{
	if (m_content_end > (SHA256_BLOCK_SIZE - 9)) {
		throw "Cannot pad this block, not enough room!";
	}

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
