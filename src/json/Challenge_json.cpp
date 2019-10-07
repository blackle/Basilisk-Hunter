#include "Challenge_json.h"
#include "Solution_json.h"

void to_json(json& j, const Challenge& c) {
	j = json{
		{"id", c.id()},
		{"prefix", c.prefix()},
		{"nonce_length", c.nonce_length()},
		{"solution", c.solution()},
	};
}

void from_json(const json& j, Challenge& c) {
	c.set_id(j.at("id"));
	c.set_prefix(j.at("prefix"));
	c.set_nonce_length(j.at("nonce_length"));
	c.set_solution(j.at("solution"));
}
