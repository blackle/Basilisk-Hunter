#include "SHA256Impl_ISHAExt.h"
#include "bits_x86_64/sha256_ishaext.h"

#if defined(__x86_64__)
#include "cpuinfo_x86.h"
static const cpu_features::X86Features features = cpu_features::GetX86Info().features;
#endif

void SHA256Impl_ISHAExt::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if defined(__x86_64__) && defined(__SHA__)
	sha256_avx(state->data(), block->data());
#else
	(void)state;
	(void)block;
#endif
}

bool SHA256Impl_ISHAExt::supported() const
{
#if defined(__x86_64__)
	return features.sha;
#else
	return false;
#endif
}
