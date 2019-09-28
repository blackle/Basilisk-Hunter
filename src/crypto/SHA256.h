#pragma once

#include <stdint.h>
#include <string>
#include <array>

constexpr int SHA256_STATE_SIZE = 8;
constexpr int SHA256_DIGEST_SIZE = 32;
constexpr int SHA256_BLOCK_SIZE = 64;

typedef std::array<uint8_t, SHA256_DIGEST_SIZE> SHA256Digest;

class SHA256Block : public std::array<uint8_t, SHA256_BLOCK_SIZE>
{
public:
	explicit SHA256Block(const std::string& data);
	explicit SHA256Block(const std::string& data, size_type total_length);
	explicit SHA256Block(size_type offset, size_type total_length);

	iterator content_end() noexcept;
	const_iterator content_end() const noexcept;

	size_type content_length() const;

private:
	typedef std::array<uint8_t, SHA256_BLOCK_SIZE> super;

	void init_padding(size_type total_length);
	size_type m_content_end;
};

class SHA256State : public std::array<uint32_t, SHA256_STATE_SIZE>
{
public:
	SHA256State();
	void reset();

	void digest(SHA256Digest* digest);
	void digest(SHA256Block* digest);

private:
	typedef std::array<uint32_t, SHA256_STATE_SIZE> super;
};
