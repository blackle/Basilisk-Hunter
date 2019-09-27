#pragma once

#include <crypto/SHA256Impl.h>

class SHA256Impl_Naive : public SHA256Impl {
public:
	virtual void calc_block(SHA256State* state, const SHA256Block* block) const override;
	virtual bool supported() const override;
};
