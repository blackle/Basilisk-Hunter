#include "SHA256SSE4Compressor.h"
#include "bits/sha256_sse4.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256SSE4Compressor::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_sse4(block->x, ctx->s, 1);
}

bool SHA256SSE4Compressor::supported() const
{
	return features.sse4_1;
}
