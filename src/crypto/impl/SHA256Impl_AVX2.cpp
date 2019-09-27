#include "SHA256Impl_AVX2.h"
#include "bits/sha256_avx2_rorx2.h"

#if defined(__x86_64__)
#include "cpuinfo_x86.h"
static const cpu_features::X86Features features = cpu_features::GetX86Info().features;
#endif

void SHA256Impl_AVX2::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
#if defined(__x86_64__)
	sha256_rorx(block->x, ctx->s, 1);
#else
	(void)ctx;
	(void)block;
#endif
}

void SHA256Impl_AVX2::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if defined(__x86_64__)
	sha256_rorx(block->data(), state->data(), 1);
#else
	(void)state;
	(void)block;
#endif
}

bool SHA256Impl_AVX2::supported() const
{
#if defined(__x86_64__)
	return features.avx2;
#else
	return false;
#endif
}
