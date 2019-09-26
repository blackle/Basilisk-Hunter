#pragma once
#include "SHA256CompressorInterface.h"

class SHA256Tester
{
public:
	static bool verify(const SHA256CompressorInterface* impl);
	static int benchmark(const SHA256CompressorInterface* impl);
};
