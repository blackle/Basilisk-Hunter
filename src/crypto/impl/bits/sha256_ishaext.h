#pragma once
#include <stdint.h>

extern "C" {
	void sha256_ishaext(uint32_t state[8], const uint8_t input[]);
}
