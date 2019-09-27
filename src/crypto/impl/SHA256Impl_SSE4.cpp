#include "SHA256Impl_SSE4.h"
#include "bits/sha256_sse4.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256Impl_SSE4::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
	sha256_sse4(block->x, ctx->s, 1);
}

void SHA256Impl_SSE4::calc_block(SHA256State* state, const SHA256Block* block) const
{
	sha256_sse4(block->data(), state->data(), 1);
}

bool SHA256Impl_SSE4::supported() const
{
	return features.sse4_1;
}
