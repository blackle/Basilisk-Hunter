#pragma once

#include <crypto/SHA256.h>
#include <string>

class NonceUtil
{
public:
	static std::string build(int length);
	static void increment(SHA256Block& block);
};

