#pragma once

#include "SHA256Impl.h"
#include <memory>

enum class SHA256ImplName {
	None,
	Naive,
	SSE4,
	AVX1,
	AVX2,
	ISHAExt,
	Arm,
};

class SHA256ImplFactory {
public:

	static SHA256ImplName get_best_impl_name();
	static std::unique_ptr<const SHA256Impl> get_impl(SHA256ImplName impl);
};
