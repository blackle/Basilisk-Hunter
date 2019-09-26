#pragma once
#include "AbstractSHA256Impl.h"

class SHA256Tester
{
public:
	static bool verify(const AbstractSHA256Impl* impl);
	static int benchmark(const AbstractSHA256Impl* impl);
};
