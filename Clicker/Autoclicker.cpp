#include "../globals.hh"
#include "Autoclicker.h"
#include "../ui/ui.hh"
#include <Windows.h>
#include <random>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

// Atomic variables for thread safety
std::atomic<bool> isRunning = true;

std::random_device rd;
std::mt19937 gen(rd());

float Randomization(int min, int max, int cps) {
    std::uniform_int_distribution<int> dis(min, max);
    return static_cast<float>(dis(gen)) / cps;
}

void log(const std::string& message) {
    std::cout << "[LOG]: " << message << std::endl;
}

void processLeftClick() {
    if (globals.leftToggle) {
        PostMessage(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(23));
        PostMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(Randomization(globals.leftRandomMin, globals.leftRandomMax, globals.leftCPS))));
    }
}

void processRightClick() {
    if (globals.rightToggle) {
        PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(23));
        PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(Randomization(globals.rightRandomMin, globals.rightRandomMax, globals.rightCPS))));
    }
}

void processClickEvents() {
    processLeftClick();
    processRightClick();
}

void autoclicker() {
    while (isRunning) {
        processClickEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Prevent 100% CPU usage
    }
    log("Autoclicker thread stopped.");
}

void handleKeyToggles() {
    while (isRunning) {
        if (GetAsyncKeyState(globals.leftBoundKey) & 0x8000) {
            globals.leftToggle = !globals.leftToggle;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Debounce
        }
        if (GetAsyncKeyState(globals.rightBoundKey) & 0x8000) {
            globals.rightToggle = !globals.rightToggle;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Debounce
        }
    }
}

void clickerThread() {
    log("Starting threads...");

    // Using std::thread for modern C++ threading
    std::thread hookThread([] {
        hook::hookmouse();
        });

    std::thread toggleThread(handleKeyToggles);
    std::thread autoclickerThread(autoclicker);

    // Detach threads so they run independently
    hookThread.detach();
    toggleThread.detach();
    autoclickerThread.detach();

    log("Threads started.");
}

// Entry point for controlling the autoclicker
void stopAutoclicker() {
    isRunning = false;
    log("Stopping autoclicker...");
}
