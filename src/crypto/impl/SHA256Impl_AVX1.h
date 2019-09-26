#pragma once

#include <crypto/AbstractSHA256Impl.h>

class SHA256Impl_AVX1 : public AbstractSHA256Impl {
public:
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const override;
	virtual bool supported() const override;
};
