#pragma once

#include "SHA256Impl.h"

enum class SHA256ImplName {
	None,
	Naive,
	SSE4,
	AVX1,
	AVX2,
	ISHAExt,
	ARMv8,
};

class SHA256ImplFactory {
public:

	static SHA256ImplName get_best_impl_name();
	static const SHA256Impl* get_impl(SHA256ImplName impl);
};
