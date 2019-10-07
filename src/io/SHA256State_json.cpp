#include "SHA256State_json.h"
#include <string>
#include <sstream>
#include <array_ios.h>

void to_json(json& j, const SHA256State& s) {
	std::ostringstream os;
	os << s;
	j = json{os.str()};
}

void from_json(const json& j, SHA256State& s) {
	std::istringstream is(j.get<std::string>());
	is >> s;
}
