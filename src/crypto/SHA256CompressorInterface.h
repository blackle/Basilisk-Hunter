#pragma once

#include "SHA256.h"

class SHA256CompressorInterface {
public:
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const = 0;

	bool verify() const;
	int benchmark() const;
};
