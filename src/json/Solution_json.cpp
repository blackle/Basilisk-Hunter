#include "Solution_json.h"
#include "SHA256State_json.h"
#include <array_ios.h>

void to_json(json& j, const Solution& s) {
	j = json{
		{"hash", s.hash()},
		{"nonce", s.nonce()},
	};
}

void from_json(const json& j, Solution& s) {
	s.set_hash(j.at("hash"));
	s.set_nonce(j.at("nonce"));
}
