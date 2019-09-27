#include "SHA256Impl_AVX1.h"
#include "bits/sha256_avx1.h"

#if defined(__x86_64__)
#include "cpuinfo_x86.h"
static const cpu_features::X86Features features = cpu_features::GetX86Info().features;
#endif

void SHA256Impl_AVX1::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
#if defined(__x86_64__)
	sha256_avx(block->x, ctx->s, 1);
#else
	(void)ctx;
	(void)block;
#endif
}

void SHA256Impl_AVX1::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if defined(__x86_64__)
	sha256_avx(block->data(), state->data(), 1);
#else
	(void)state;
	(void)block;
#endif
}

bool SHA256Impl_AVX1::supported() const
{
#if defined(__x86_64__)
	return features.avx;
#else
	return false;
#endif
}
