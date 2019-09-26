#pragma once

#include <crypto/SHA256CompressorInterface.h>

class SHA256AVX1Compressor : public SHA256CompressorInterface {
public:
	virtual void calc_block(sha256_ctx* ctx, sha256_block* block) const override;
	virtual bool supported() const override;
};
