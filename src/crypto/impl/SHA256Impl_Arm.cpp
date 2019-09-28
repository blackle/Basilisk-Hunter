#include "SHA256Impl_Arm.h"
#include "bits_arm/sha256-armv8.h"

void SHA256Impl_Arm::calc_block(SHA256State* state, const SHA256Block* block) const
{
	sha256_armv8(state->data(), block->data(), 1);
}

bool SHA256Impl_Arm::supported() const
{
	return true;
}
