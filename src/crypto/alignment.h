#pragma once

#if defined(__LP64__) || defined(_WIN64)
#define CRYPTO_ALIGNMENT alignas(8)
#else
#define CRYPTO_ALIGNMENT alignas(4)
#endif
