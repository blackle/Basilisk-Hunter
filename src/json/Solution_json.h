#pragma once
#include <model/Solution.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json& j, const Solution& s);
void from_json(const json& j, Solution& s);
