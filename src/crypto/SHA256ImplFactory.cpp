#include "SHA256ImplFactory.h"
#include "SHA256Tester.h"
#include "impl/SHA256Impl_Naive.h"
#include "impl/SHA256Impl_SSE4.h"
#include "impl/SHA256Impl_AVX1.h"
#include "impl/SHA256Impl_AVX2.h"
#include "impl/SHA256Impl_ISHAExt.h"
#include "impl/SHA256Impl_ARMv8.h"
#include <iostream>

template<typename T>
static void test_impl(std::string& best_name, int& best_time)
{
	T impl;
	if (impl.supported() && SHA256Tester::verify(&impl)) {
		auto benchmark = SHA256Tester::benchmark(&impl);
		std::cout << T::name() << " " << benchmark << std::endl;
		if (benchmark < best_time || best_time < 0) {
			best_name = T::name();
			best_time = benchmark;
		}
	}
}

std::string SHA256ImplFactory::get_best_impl_name()
{
	std::string best_name;
	int best_time = -1;

	test_impl<SHA256Impl_Naive>    (best_name, best_time);
	test_impl<SHA256Impl_SSE4>     (best_name, best_time);
	test_impl<SHA256Impl_AVX1>     (best_name, best_time);
	test_impl<SHA256Impl_AVX2>     (best_name, best_time);
	test_impl<SHA256Impl_ISHAExt>  (best_name, best_time);
	test_impl<SHA256Impl_ARMv8>    (best_name, best_time);

	return best_name;
}

template<typename T>
static void set_impl(SHA256Impl*& impl, const std::string& impl_name)
{
	if (T::name() == impl_name) {
		impl = new T;
	}
}

const SHA256Impl* SHA256ImplFactory::get_impl(const std::string& impl_name)
{
	SHA256Impl* impl = nullptr;
	set_impl<SHA256Impl_Naive>   (impl, impl_name);
	set_impl<SHA256Impl_SSE4>    (impl, impl_name);
	set_impl<SHA256Impl_AVX1>    (impl, impl_name);
	set_impl<SHA256Impl_AVX2>    (impl, impl_name);
	set_impl<SHA256Impl_ISHAExt> (impl, impl_name);
	set_impl<SHA256Impl_ARMv8>   (impl, impl_name);
	return impl;
}
