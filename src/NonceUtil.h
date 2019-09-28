#pragma once

#include <crypto/SHA256.h>
#include <string>

class NonceUtil
{
public:
	static std::string build(int length);
	template<typename I>
	static void increment(I begin, I end);
};

template <typename I>
void NonceUtil::increment(I begin, I end)
{
	for (auto i = begin; i != end; i++) {
	        char res =  *i;
                res++;
		switch (res) {
                        case '{':
                                *i = '0';
                                continue;
                        case '[':
                                *i = 'a';
                                return;
                        case ':':
                                *i = 'A';
                                return;
                        default:
                                *i = res;
                                return;
                }
        }
}
