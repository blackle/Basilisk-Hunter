#include "SHA256Impl_AVX1.h"
#include "bits/sha256_avx1.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256Impl_AVX1::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_avx(block->x, ctx->s, 1);
}

bool SHA256Impl_AVX1::supported() const
{
	return features.avx;
}
