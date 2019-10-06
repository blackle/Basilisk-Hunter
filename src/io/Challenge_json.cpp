#include "Challenge_json.h"

void to_json(json& j, const Challenge& c) {
	j = json{
		{"id", c.id()},
		{"prefix", c.prefix()},
		{"nonce_length", c.nonce_length()},
		{"best_nonce", c.best_nonce()},
		{"best_hash", c.best_hash()},
	};
}

void from_json(json& j, Challenge& p) {
	(void) j;
	(void) p;
}
