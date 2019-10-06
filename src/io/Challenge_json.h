#pragma once
#include <model/Challenge.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json& j, const Challenge& p);
void from_json(json& j, Challenge& p);
