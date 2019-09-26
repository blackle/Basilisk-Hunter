#include "SHA256CompressorFactory.h"
#include "impl/SHA256NaiveCompressor.h"
#include <iostream>

template<typename T, SHA256ImplName TName>
static void check_impl(SHA256ImplName& best_name, int& best_time)
{
	T impl;
	if (impl.verify()) {
		auto benchmark = impl.benchmark();
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

	check_impl<SHA256NaiveCompressor, SHA256ImplName::Naive>(best_name, best_time);

	std::cout << best_time << std::endl;

	return best_name;
}

std::unique_ptr<const SHA256CompressorInterface> SHA256CompressorFactory::get_impl(SHA256ImplName impl_name)
{
	std::unique_ptr<SHA256CompressorInterface> impl;
	switch (impl_name) {
		case SHA256ImplName::Naive:
			impl.reset(new SHA256NaiveCompressor);
			break;
		default:
			break;
	}
	return std::move(impl);
}