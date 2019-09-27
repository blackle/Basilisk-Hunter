#pragma once

#include <stdint.h>
#include <string>
#include <array>

constexpr int SHA256_STATE_SIZE = 8;
constexpr int SHA256_DIGEST_SIZE = 32;
constexpr int SHA256_BLOCK_SIZE = 64;

class SHA256State : public std::array<uint32_t, SHA256_STATE_SIZE>
{
public:
	SHA256State();

private:
	typedef std::array<uint32_t, SHA256_STATE_SIZE> super;
};

class SHA256Block : public std::array<uint8_t, SHA256_BLOCK_SIZE>
{
public:
	explicit SHA256Block(const std::string& data);
	explicit SHA256Block(const std::string& data, size_type total_length);

	iterator content_end() noexcept;
	const_iterator content_end() const noexcept;

private:
	typedef std::array<uint8_t, SHA256_BLOCK_SIZE> super;

	size_type m_content_end;
};

typedef struct {
	unsigned char x[SHA256_BLOCK_SIZE];
} sha256_block;

typedef struct {
	uint32_t s[8];
} sha256_ctx;

void sha256_init(sha256_ctx * ctx);
void sha256_pad_block(sha256_block * block, int offset, int length);
void sha256_digest(sha256_ctx * ctx, unsigned char* digest);
