#include "SHA256AVX1Compressor.h"
#include "bits/sha256_avx1.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256AVX1Compressor::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_avx(block->x, ctx->s, 1);
}

bool SHA256AVX1Compressor::supported() const
{
	return features.avx;
}
