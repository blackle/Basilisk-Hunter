#pragma once

class SHA256Impl;

class SHA256Tester
{
public:
	static bool verify(const SHA256Impl* impl);
	static int benchmark(const SHA256Impl* impl);
};
