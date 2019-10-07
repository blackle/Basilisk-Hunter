#include "SHA256State_json.h"
#include <io/array_ios.h>
#include <string>
#include <sstream>

void to_json(json& j, const SHA256State& s) {
	std::ostringstream os;
	os << s;
	j = json{os.str()};
}

void from_json(const json& j, SHA256State& s) {
	std::istringstream is(j.get<std::string>());
	is >> s;
}
