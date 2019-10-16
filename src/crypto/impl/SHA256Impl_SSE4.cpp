#include "SHA256Impl_SSE4.h"
#include "bits_x86_64/sha256_sse4.h"

#if defined(__x86_64__) || defined(_M_X64)
#include "cpuinfo_x86.h"
static const cpu_features::X86Features features = cpu_features::GetX86Info().features;
#endif

void SHA256Impl_SSE4::calc_block(SHA256State* state, const SHA256Block* block) const
{
#if defined(__x86_64__) || defined(_M_X64)
	sha256_sse4(block->data(), state->data(), 1);
#else
	(void)state;
	(void)block;
#endif
}

bool SHA256Impl_SSE4::supported() const
{
#if defined(__x86_64__) || defined(_M_X64)
	return features.sse4_1;
#else
	return false;
#endif
}

std::string SHA256Impl_SSE4::name()
{
	return "SSE4";
}
