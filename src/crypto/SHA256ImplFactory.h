#pragma once

#include <string>
#include <vector>
#include <memory>
#include "SHA256ImplMetadata.h"

class SHA256Impl;

class SHA256ImplFactory {
public:
	SHA256ImplFactory() = delete;
	static std::string get_best_impl_name();
	static std::shared_ptr<const SHA256Impl> get_impl(const std::string& impl_name);
	static std::vector<SHA256ImplMetadata> impl_list();
};
