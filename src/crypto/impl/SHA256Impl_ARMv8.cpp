#include "SHA256Impl_ARMv8.h"
#include "bits_arm/sha256-armv8.h"

#if __ARM_ARCH == 8
#if defined(__aarch64__)
#include "cpuinfo_aarch64.h"
static const cpu_features::Aarch64Features features = cpu_features::GetAarch64Info().features;
#else
#include "cpuinfo_arm.h"
static const cpu_features::ArmFeatures features = cpu_features::GetArmInfo().features;
#endif
#endif

void SHA256Impl_ARMv8::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if __ARM_ARCH == 8
	sha256_armv8(state->data(), block->data(), 1);
#else
	(void) state;
	(void) block;
#endif
}

bool SHA256Impl_ARMv8::supported() const
{
#if __ARM_ARCH == 8
	return features.sha2;
#else
	return false;
#endif
}
