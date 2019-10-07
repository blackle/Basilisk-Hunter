#pragma once
#include <crypto/SHA256State.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json& j, const SHA256State& s);
void from_json(const json& j, SHA256State& s);
