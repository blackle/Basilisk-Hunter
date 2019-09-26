#include "SHA256CompressorFactory.h"
#include "SHA256Tester.h"
#include "impl/SHA256NaiveCompressor.h"
#include "impl/SHA256SSE4Compressor.h"
#include "impl/SHA256AVX1Compressor.h"
#include "impl/SHA256AVX2Compressor.h"
#include "impl/SHA256ISHAExtCompressor.h"
#include <iostream>

template<typename T, SHA256ImplName TName>
static void test_impl(SHA256ImplName& best_name, int& best_time)
{
	T impl;
	if (impl.supported() && SHA256Tester::verify(&impl)) {
		auto benchmark = SHA256Tester::benchmark(&impl);
		if (benchmark < best_time || best_time < 0) {
			best_name = TName;
			best_time = benchmark;
		}
	}
}

SHA256ImplName SHA256CompressorFactory::get_best_impl_name()
{
	auto best_name = SHA256ImplName::None;
	int best_time = -1;

	test_impl<SHA256NaiveCompressor, SHA256ImplName::Naive>(best_name, best_time);
	test_impl<SHA256SSE4Compressor, SHA256ImplName::SSE4>(best_name, best_time);
	test_impl<SHA256AVX1Compressor, SHA256ImplName::AVX1>(best_name, best_time);
	test_impl<SHA256AVX2Compressor, SHA256ImplName::AVX2>(best_name, best_time);
	test_impl<SHA256ISHAExtCompressor, SHA256ImplName::ISHAExt>(best_name, best_time);

	return best_name;
}

std::unique_ptr<const SHA256CompressorInterface> SHA256CompressorFactory::get_impl(SHA256ImplName impl_name)
{
	std::unique_ptr<SHA256CompressorInterface> impl;
	switch (impl_name) {
		case SHA256ImplName::Naive:
			impl.reset(new SHA256NaiveCompressor);
			break;
		case SHA256ImplName::SSE4:
			impl.reset(new SHA256SSE4Compressor);
			break;
		case SHA256ImplName::AVX1:
			impl.reset(new SHA256AVX1Compressor);
			break;
		case SHA256ImplName::AVX2:
			impl.reset(new SHA256AVX2Compressor);
			break;
		case SHA256ImplName::ISHAExt:
			impl.reset(new SHA256ISHAExtCompressor);
			break;
		default:
			break;
	}
	return std::move(impl);
}