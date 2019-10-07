#pragma once
#include <model/Challenge.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json& j, const Challenge& c);
void from_json(const json& j, Challenge& c);
