#include "SHA256Impl_Arm.h"
#include "bits_arm/sha256-armv8.h"

#if __ARM_ARCH == 8
#include "cpuinfo_arm.h"
static const cpu_features::ArmFeatures features = cpu_features::GetArmInfo().features;
#endif

void SHA256Impl_Arm::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if __ARM_ARCH == 8
	sha256_armv8(state->data(), block->data(), 1);
#else
	(void) state;
	(void) block;
#endif
}

bool SHA256Impl_Arm::supported() const
{
#if __ARM_ARCH == 8
	return features.sha2;
#else
	return false;
#endif
}
