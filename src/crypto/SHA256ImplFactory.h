#pragma once

#include "SHA256Impl.h"
#include <string>

class SHA256ImplFactory {
public:
	static std::string get_best_impl_name();
	static const SHA256Impl* get_impl(const std::string& impl_name);
};
