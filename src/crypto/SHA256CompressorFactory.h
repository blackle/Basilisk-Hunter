#pragma once

#include "SHA256CompressorInterface.h"
#include <memory>

enum class SHA256ImplName {
	None,
	Naive,
};

class SHA256CompressorFactory {
public:

	static SHA256ImplName get_best_impl_name();
	static std::unique_ptr<const SHA256CompressorInterface> get_impl(SHA256ImplName impl);
};