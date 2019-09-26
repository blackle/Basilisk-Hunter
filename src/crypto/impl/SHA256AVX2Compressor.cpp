#include "SHA256AVX2Compressor.h"
#include "bits/sha256_avx2_rorx2.h"

void SHA256AVX2Compressor::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_rorx(block->x, ctx->s, 1);
}

bool SHA256AVX2Compressor::supported() const
{
	return false;
}
