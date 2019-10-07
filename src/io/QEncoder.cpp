#include "QEncoder.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <locale>

class QEncoding {
public:
	void operator()(const uint8_t c) {
		if (c > 127 || c == '=' || c == '?') {
			os << "="
			   << std::hex
			   << std::uppercase
			   << std::setfill('0')
			   << std::setw(2)
			   << (int) c;
		} else {
			os << c;
		}
	}

	std::string str() const {
		return os.str();
	}
private:
	std::ostringstream os;
};

std::string QEncoder::encode_utf8(const std::string& str) {
	QEncoding encoding = std::for_each(str.begin(), str.end(), QEncoding());
	return "=?utf-8?Q?" + encoding.str() + "?=";
}
