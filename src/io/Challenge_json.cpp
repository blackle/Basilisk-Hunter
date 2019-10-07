#include "Challenge_json.h"
#include "SHA256State_json.h"
#include <array_ios.h>

void to_json(json& j, const Challenge& c) {
	j = json{
		{"id", c.id()},
		{"prefix", c.prefix()},
		{"nonce_length", c.nonce_length()},
		{"best_nonce", c.best_nonce()},
		{"best_hash", c.best_hash()},
	};
}

void from_json(const json& j, Challenge& c) {
	c.set_id(j.at("id"));
	c.set_prefix(j.at("prefix"));
	c.set_nonce_length(j.at("nonce_length"));
	c.set_hash_and_nonce(j.at("best_hash"), j.at("best_nonce"));
}
