#include <crypto/SHA256CompressorFactory.h>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	auto best = SHA256CompressorFactory::get_best_impl_name();
	auto compressor = SHA256CompressorFactory::get_impl(best);

	sha256_ctx ctx;
	sha256_init(&ctx);

	sha256_block block;

	compressor->calc_block(&ctx, &block);

	return 0;
}
