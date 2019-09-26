#include "SHA256Impl_ISHAExt.h"
#include "bits/sha256_ishaext.h"
#include "cpuinfo_x86.h"

static const cpu_features::X86Features features = cpu_features::GetX86Info().features;

void SHA256Impl_ISHAExt::calc_block(sha256_ctx* ctx, sha256_block* block) const
{
#if defined(__SHA__)
	sha256_ishaext(ctx->s, block->x);
#else
	(void)ctx;
	(void)block;
#endif
}

bool SHA256Impl_ISHAExt::supported() const
{
	return features.sha;
}
