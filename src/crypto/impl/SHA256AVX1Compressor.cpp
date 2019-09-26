#include "SHA256AVX1Compressor.h"
#include "bits/sha256_avx1.h"

void SHA256AVX1Compressor::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_avx(block->x, ctx->s, 1);
}

