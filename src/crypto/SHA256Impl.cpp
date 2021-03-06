#include "SHA256Impl.h"
#include "bswap.h"

void SHA256Impl::copy_state_into_block(const SHA256State* state, SHA256Block* block) {
#if defined(__LP64__) || defined(_WIN64)
	//todo: this could potentially be faster with AVX2's _mm256_shuffle_epi8
	//don't worry about alignment here, CRYPTO_ALIGNMENT should take care of it
	auto block_alias = reinterpret_cast<uint64_t*>(block->data());
	auto state_alias = reinterpret_cast<const uint64_t*>(state->data());
	for (unsigned i = 0; i < SHA256_STATE_SIZE/2; i++) {
		block_alias[i] = bswap64((state_alias[i] << 32u) | (state_alias[i] >> 32u));
	}
#else
	auto block_alias = reinterpret_cast<uint32_t*>(block->data());
	auto state_alias = reinterpret_cast<const uint32_t*>(state->data());
	for (unsigned i = 0; i < SHA256_STATE_SIZE; i++) {
		block_alias[i] = bswap32(state_alias[i]);
	}
#endif
}

std::string SHA256Impl::hash_to_padding(SHA256State* state, const std::string& data) const
{
	auto residual = data;
	while (residual.length() >= SHA256_BLOCK_SIZE) {
		SHA256Block block(residual.substr(0, SHA256_BLOCK_SIZE));
		calc_block(state, &block);
		residual.erase(0, SHA256_BLOCK_SIZE);
	}
	return residual;
}

SHA256State SHA256Impl::hash(const std::string& data) const
{
	SHA256State state;

	auto residual = hash_to_padding(&state, data);
	SHA256Block padding_block(residual, data.length());
	calc_block(&state, &padding_block);

	return state;
}

SHA256State SHA256Impl::hash(const SHA256State& data) const
{
	SHA256State state;

	SHA256Block padding_block(32, 32);
	copy_state_into_block(&data, &padding_block);
	calc_block(&state, &padding_block);

	return state;
}

SHA256State SHA256Impl::double_hash(const std::string& data) const {
	return hash(hash(data));
}