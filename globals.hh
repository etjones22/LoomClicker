#pragma once
#include <Windows.h>
#include <unordered_map>

class c_globals {
public:
    bool active = true;
    char user_name[255] = "user";
    char pass_word[255] = "pass";

    int leftBoundKey = VK_F6;  // Default toggle key for left autoclicker
    int rightBoundKey = VK_F7; // Default toggle key for right autoclicker
    bool leftWaitingForKey = false;
    bool rightWaitingForKey = false;

    // Randomization ranges for left and right clicks
    int leftRandomMin = 260;
    int leftRandomMax = 880;
    int rightRandomMin = 260;
    int rightRandomMax = 880;

    // CPS settings
    int leftCPS = 1;
    int rightCPS = 1;

    // Toggle states
    bool leftToggle = false;
    bool rightToggle = false;
};

inline c_globals globals;
