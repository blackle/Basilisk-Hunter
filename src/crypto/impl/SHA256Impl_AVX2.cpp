#include "SHA256Impl_AVX2.h"
#include "bits/sha256_avx2_rorx2.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256Impl_AVX2::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_rorx(block->x, ctx->s, 1);
}

void SHA256Impl_AVX2::calc_block(SHA256State* state, const SHA256Block* block) const
{
	sha256_rorx(block->data(), state->data(), 1);
}

bool SHA256Impl_AVX2::supported() const
{
	return features.avx2;
}
