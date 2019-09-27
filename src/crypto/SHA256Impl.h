#pragma once

#include "SHA256.h"

class SHA256Impl {
public:
	virtual ~SHA256Impl() = default;
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const = 0;
	virtual void calc_block(SHA256State* state, const SHA256Block* block) const = 0;
	virtual bool supported() const = 0;
};
