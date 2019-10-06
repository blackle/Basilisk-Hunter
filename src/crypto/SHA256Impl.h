#pragma once

#include "SHA256Block.h"
#include "SHA256State.h"

class SHA256Impl {
public:
	virtual ~SHA256Impl() = default;
	virtual void calc_block(SHA256State* state, const SHA256Block* block) const = 0;
	virtual bool supported() const = 0;

	static void copy_state_into_block(const SHA256State* state, SHA256Block* block);
	std::string hash_to_padding(SHA256State* state, const std::string& data) const;

	SHA256State hash(const std::string& data) const;
	SHA256State hash(const SHA256State& data) const;

	SHA256State double_hash(const std::string& data) const;
};
