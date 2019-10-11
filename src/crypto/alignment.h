#pragma once

#ifdef __LP64__
#define CRYPTO_ALIGNMENT alignas(8)
#else
#define CRYPTO_ALIGNMENT alignas(4)
#endif
