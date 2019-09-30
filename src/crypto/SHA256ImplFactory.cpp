#include "SHA256ImplFactory.h"
#include "SHA256Tester.h"
#include "impl/SHA256Impl_Naive.h"
#include "impl/SHA256Impl_SSE4.h"
#include "impl/SHA256Impl_AVX1.h"
#include "impl/SHA256Impl_AVX2.h"
#include "impl/SHA256Impl_ISHAExt.h"
#include "impl/SHA256Impl_ARMv8.h"
#include <iostream>

template<typename T, SHA256ImplName TName>
static void test_impl(SHA256ImplName& best_name, int& best_time)
{
	T impl;
	if (impl.supported() && SHA256Tester::verify(&impl)) {
		auto benchmark = SHA256Tester::benchmark(&impl);
		std::cout << (int)TName << " " << benchmark << std::endl;
		if (benchmark < best_time || best_time < 0) {
			best_name = TName;
			best_time = benchmark;
		}
	}
}

SHA256ImplName SHA256ImplFactory::get_best_impl_name()
{
	auto best_name = SHA256ImplName::None;
	int best_time = -1;

	test_impl<SHA256Impl_Naive, SHA256ImplName::Naive>(best_name, best_time);
	test_impl<SHA256Impl_SSE4, SHA256ImplName::SSE4>(best_name, best_time);
	test_impl<SHA256Impl_AVX1, SHA256ImplName::AVX1>(best_name, best_time);
	test_impl<SHA256Impl_AVX2, SHA256ImplName::AVX2>(best_name, best_time);
	test_impl<SHA256Impl_ISHAExt, SHA256ImplName::ISHAExt>(best_name, best_time);
	test_impl<SHA256Impl_ARMv8, SHA256ImplName::ARMv8>(best_name, best_time);

	return best_name;
}

const SHA256Impl* SHA256ImplFactory::get_impl(SHA256ImplName impl_name)
{
	switch (impl_name) {
		case SHA256ImplName::Naive:
			return new SHA256Impl_Naive;
		case SHA256ImplName::SSE4:
			return new SHA256Impl_SSE4;
		case SHA256ImplName::AVX1:
			return new SHA256Impl_AVX1;
		case SHA256ImplName::AVX2:
			return new SHA256Impl_AVX2;
		case SHA256ImplName::ISHAExt:
			return new SHA256Impl_ISHAExt;
		case SHA256ImplName::ARMv8:
			return new SHA256Impl_ARMv8;
		default:
			break;
	}
	return nullptr;
}
