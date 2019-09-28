#pragma once

extern "C" {
	void sha256_armv8(void *ctx, const void *inp, size_t len);
}
