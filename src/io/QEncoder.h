#pragma once

#include <string>

class QEncoder {
public:
	QEncoder() = delete;
	static std::string encode_utf8(const std::string& str);
};
