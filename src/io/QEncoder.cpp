#include "QEncoder.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <locale>

struct QEncoding {
	std::ostringstream os;
	void operator()(const uint8_t c) {
		if (c > 127 || c == '=' || c == '?') {
			os.str("");
			os << std::hex
			   << std::uppercase
			   << std::setfill('0')
			   << std::setw(2)
			   << (int) c;
			x += "=" + os.str();
		} else {
			x += c;
		}
	}
	std::string x;
};

std::string QEncoder::encode_utf8(const std::string& str) {
	QEncoding encoding = std::for_each(str.begin(), str.end(), QEncoding());
	return "=?utf-8?Q?" + encoding.x + "?=";
}
