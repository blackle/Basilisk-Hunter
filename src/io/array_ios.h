#pragma once

#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>

template <typename T, std::size_t SIZE>
std::ostream& operator<<(std::ostream& os, const std::array<T, SIZE>& data)
{
	constexpr std::size_t DIGITS = sizeof(T) * 2;
	std::ios_base::fmtflags f(os.flags());
	for (auto i = data.begin(); i != data.end(); i++) {
		os << std::setfill('0') << std::setw(DIGITS) << std::hex << (int)*i;
	}
	os.flags(f);
	return os;
}

template <typename T, std::size_t SIZE>
std::istream& operator>>(std::istream& is, std::array<T, SIZE>& data)
{
	constexpr std::size_t DIGITS = sizeof(T) * 2;
	for (auto i = data.begin(); i != data.end(); i++) {
		std::string bytes(DIGITS, ' ');
		is.read(&bytes[0], DIGITS);
		std::istringstream bytes_stream(bytes);
		bytes_stream >> std::hex >> *i;
	}
	return is;
}
