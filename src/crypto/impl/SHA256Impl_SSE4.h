#pragma once

#include <crypto/SHA256Impl.h>

class SHA256Impl_SSE4 : public SHA256Impl {
public:
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const override;
	virtual bool supported() const override;
};
