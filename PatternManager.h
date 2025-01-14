#pragma once
#include <nlohmann/json.hpp>
#include "../globals.hh"

inline void to_json(nlohmann::json& j, const ClickEvent& e) {
    j = nlohmann::json{
        {"position", {e.position.x, e.position.y}},
        {"button", e.button},
        {"delay", e.delay}
    };
}

inline void from_json(const nlohmann::json& j, ClickEvent& e) {
    e.position.x = j.at("position")[0];
    e.position.y = j.at("position")[1];
    e.button = j.at("button").get<int>();
    e.delay = j.at("delay").get<int>();
}

// Declare other functions
void savePattern(const std::string& filePath);
void loadPattern(const std::string& filePath);
void playPattern();
