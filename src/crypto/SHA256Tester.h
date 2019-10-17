#pragma once

#include <cstdint>

class SHA256Impl;

class SHA256Tester
{
public:
	SHA256Tester() = delete;
	static bool verify(const SHA256Impl* impl);
	static int64_t benchmark(const SHA256Impl* impl);
};
