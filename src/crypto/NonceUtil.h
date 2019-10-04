#pragma once

#include <array>
#include <string>
#include <type_traits>

class NonceUtil
{
public:
	static std::string build(int length);
	template<typename I, typename = std::enable_if<std::is_same<typename std::iterator_traits<I>::value_type, std::uint8_t>::value>>
	static void increment(I begin, I end) {
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
};

