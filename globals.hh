#pragma once
#include <Windows.h>
#include <vector>
#include <string>

struct ClickEvent {
    POINT position;
    int button; // 1 = Left, 2 = Right
    int delay;  // Delay in milliseconds before the next click
};

class c_globals {
public:
    bool active = true;
    char user_name[255] = "user";
    char pass_word[255] = "pass";

    int leftBoundKey = VK_F6;
    int rightBoundKey = VK_F7;
    bool leftWaitingForKey = false;
    bool rightWaitingForKey = false;

    int leftRandomMin = 260;
    int leftRandomMax = 880;
    int rightRandomMin = 260;
    int rightRandomMax = 880;

    int leftCPS = 1;
    int rightCPS = 1;

    bool leftToggle = false;
    bool rightToggle = false;

    // Recording and playback
    bool recording = false;
    bool playingBack = false;
    std::vector<ClickEvent> recordedPattern;
    std::string saveFilePath = "pattern.json"; // Default file for saving/loading patterns

    // Custom Pattern and Page 
    bool useCustomPattern = false; // Option to use custom click pattern
    int currentPage = 0; // 0 = Main Page, 1 = Recording/Settings Page

};

inline c_globals globals;
