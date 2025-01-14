#include "../globals.hh"
#include "../Clicker/PatternManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>


using json = nlohmann::json;


void savePattern(const std::string& filePath) {
    try {
        json patternJson = globals.recordedPattern;
        std::ofstream file(filePath);
        file << patternJson.dump(4); // Pretty print with 4 spaces
    }
    catch (const std::exception& e) {
        // Handle errors
    }
}

void loadPattern(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (file) {
            json patternJson;
            file >> patternJson;
            globals.recordedPattern = patternJson.get<std::vector<ClickEvent>>();
        }
    }
    catch (const std::exception& e) {
        // Handle errors
    }
}

void playPattern() {
    globals.playingBack = true;
    for (const auto& click : globals.recordedPattern) {
        if (!globals.playingBack) break;

        SetCursorPos(click.position.x, click.position.y);
        if (click.button == 1) {
            PostMessage(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(click.position.x, click.position.y));
            PostMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(click.position.x, click.position.y));
        }
        else if (click.button == 2) {
            PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(click.position.x, click.position.y));
            PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(click.position.x, click.position.y));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(click.delay));
    }
    globals.playingBack = false;
}