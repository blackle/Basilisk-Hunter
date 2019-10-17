#include "SHA256ImplFactory.h"
#include "SHA256Impl.h"
#include "SHA256Tester.h"
#include "impl/SHA256Impl_Naive.h"
#include "impl/SHA256Impl_SSE4.h"
#include "impl/SHA256Impl_AVX1.h"
#include "impl/SHA256Impl_AVX2.h"
#include "impl/SHA256Impl_ISHAExt.h"
#include "impl/SHA256Impl_ARMv8.h"
#include <iostream>

template<typename T>
static void test_impl(std::string* best_name, int64_t* best_time)
{
	T impl;
	if (impl.supported() && SHA256Tester::verify(&impl)) {
		auto benchmark = SHA256Tester::benchmark(&impl);
		std::cout << "Benchmarked " << T::name() << ": " << benchmark << " seconds" << std::endl;
		if (benchmark < *best_time || *best_time < 0) {
			*best_name = T::name();
			*best_time = benchmark;
		}
	}
}

std::string SHA256ImplFactory::get_best_impl_name()
{
	std::string best_name;
	int64_t best_time = -1;

	test_impl<SHA256Impl_Naive>    (&best_name, &best_time);
	test_impl<SHA256Impl_SSE4>     (&best_name, &best_time);
	test_impl<SHA256Impl_AVX1>     (&best_name, &best_time);
	test_impl<SHA256Impl_AVX2>     (&best_name, &best_time);
	test_impl<SHA256Impl_ISHAExt>  (&best_name, &best_time);
	test_impl<SHA256Impl_ARMv8>    (&best_name, &best_time);

	return best_name;
}

template<typename T>
static void set_impl(std::shared_ptr<const SHA256Impl>* impl, const std::string& impl_name)
{
	if (T::name() == impl_name) {
		impl->reset(new T);
	}
}

std::shared_ptr<const SHA256Impl> SHA256ImplFactory::get_impl(const std::string& impl_name)
{
	std::shared_ptr<const SHA256Impl> impl;
	set_impl<SHA256Impl_Naive>   (&impl, impl_name);
	set_impl<SHA256Impl_SSE4>    (&impl, impl_name);
	set_impl<SHA256Impl_AVX1>    (&impl, impl_name);
	set_impl<SHA256Impl_AVX2>    (&impl, impl_name);
	set_impl<SHA256Impl_ISHAExt> (&impl, impl_name);
	set_impl<SHA256Impl_ARMv8>   (&impl, impl_name);
	return impl;
}

template <typename T>
static void add_impl(std::vector<SHA256ImplMetadata>* md_list)
{
	SHA256ImplMetadata md;
	md.set_name(T::name());
	T impl;
	if (impl.supported()) {
		md.set_supported(true);
		md.set_working(SHA256Tester::verify(&impl));
	}
	md_list->push_back(md);
}

std::vector<SHA256ImplMetadata> SHA256ImplFactory::impl_list()
{
	std::vector<SHA256ImplMetadata> md_list;

	add_impl<SHA256Impl_Naive>    (&md_list);
	add_impl<SHA256Impl_SSE4>     (&md_list);
	add_impl<SHA256Impl_AVX1>     (&md_list);
	add_impl<SHA256Impl_AVX2>     (&md_list);
	add_impl<SHA256Impl_ISHAExt>  (&md_list);
	add_impl<SHA256Impl_ARMv8>    (&md_list);

	return md_list;
}
