#pragma once

#include <crypto/SHA256Impl.h>
#include <string>

class SHA256Impl_ARMv8 : public SHA256Impl {
public:
	virtual void calc_block(SHA256State* state, const SHA256Block* block) const override;
	virtual bool supported() const override;
	static std::string name();
};
