#pragma once

#if defined(_MSC_VER)
#define bswap64 _byteswap_uint64
#define bswap32 _byteswap_ulong
#else
#define bswap64 __builtin_bswap64
#define bswap32 __builtin_bswap32
#endif
