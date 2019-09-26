#pragma once

#include "SHA256.h"

class AbstractSHA256Impl {
public:
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const = 0;
	virtual bool supported() const = 0;
};
