#pragma once

#include <string>
#include <array>
#include "alignment.h"

constexpr unsigned SHA256_BLOCK_SIZE = 64;

class CRYPTO_ALIGNMENT SHA256Block : public std::array<uint8_t, SHA256_BLOCK_SIZE>
{
public:
	SHA256Block();
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
