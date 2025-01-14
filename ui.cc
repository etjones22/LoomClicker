#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../Clicker/PatternManager.h"

#include <thread>

void renderMainPage();
void renderRecordingPage();

void ui::render() {
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        if (globals.currentPage == 0) {
            renderMainPage();
        }
        else if (globals.currentPage == 1) {
            renderRecordingPage();
        }
    }
    ImGui::End();
}

void renderMainPage() {
    // Left Autoclicker
    ImGui::Text("Left Autoclicker:");
    ImGui::Checkbox("Enable Left", &globals.leftToggle);
    ImGui::SliderInt("Left CPS", &globals.leftCPS, 1, 20, "%d CPS");
    ImGui::SliderInt("Left Min Delay (ms)", &globals.leftRandomMin, 1, globals.leftRandomMax - 1);
    ImGui::SliderInt("Left Max Delay (ms)", &globals.leftRandomMax, globals.leftRandomMin + 1, 1000);
    if (ImGui::Button("Bind Left Key")) {
        globals.leftWaitingForKey = true;
    }
    if (globals.leftWaitingForKey) {
        ImGui::Text("Press any key...");
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                globals.leftBoundKey = i;
                globals.leftWaitingForKey = false;
                break;
            }
        }
    }

    // Right Autoclicker
    ImGui::Spacing();
    ImGui::Text("Right Autoclicker:");
    ImGui::Checkbox("Enable Right", &globals.rightToggle);
    ImGui::SliderInt("Right CPS", &globals.rightCPS, 1, 20, "%d CPS");
    ImGui::SliderInt("Right Min Delay (ms)", &globals.rightRandomMin, 1, globals.rightRandomMax - 1);
    ImGui::SliderInt("Right Max Delay (ms)", &globals.rightRandomMax, globals.rightRandomMin + 1, 1000);
    if (ImGui::Button("Bind Right Key")) {
        globals.rightWaitingForKey = true;
    }
    if (globals.rightWaitingForKey) {
        ImGui::Text("Press any key...");
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                globals.rightBoundKey = i;
                globals.rightWaitingForKey = false;
                break;
            }
        }
    }

    // Custom Pattern Option
    ImGui::Spacing();
    ImGui::Checkbox("Enable Custom Pattern", &globals.useCustomPattern);
    if (ImGui::Button("Open Recording/Settings")) {
        globals.currentPage = 1; // Switch to Recording/Settings Page
    }
}

void renderRecordingPage() {
    if (ImGui::Button("Back to Main Page")) {
        globals.currentPage = 0; // Switch to Main Page
    }

    ImGui::Text("Recording and Saving Settings:");

    // Show recording status
    if (globals.recording) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: Recording...");
    }
    else {
        ImGui::Text("Status: Not Recording");
    }

    // Recording controls
    if (ImGui::Button("Start Recording")) {
        globals.recording = true;
        globals.recordedPattern.clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Recording")) {
        globals.recording = false;
    }

    // Show saving/loading status
    static bool saveSuccess = false;
    static bool loadSuccess = false;

    if (saveSuccess) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Pattern saved successfully.");
    }
    if (loadSuccess) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Pattern loaded successfully.");
    }

    // Save/Load buttons
    if (ImGui::Button("Save Pattern")) {
        savePattern(globals.saveFilePath);
        saveSuccess = true;
        loadSuccess = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Pattern")) {
        loadPattern(globals.saveFilePath);
        loadSuccess = true;
        saveSuccess = false;
    }

    // Playback controls
    if (globals.playingBack) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Status: Playing Back...");
    }
    else {
        ImGui::Text("Status: Not Playing Back");
    }

    if (ImGui::Button("Play Pattern")) {
        std::thread(playPattern).detach();
        globals.playingBack = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop Playback")) {
        globals.playingBack = false;
    }
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;

    // colors
    ImGui::StyleColorsDark();

    if (window_pos.x == 0) {
        RECT screen_rect{};
        GetWindowRect(GetDesktopWindow(), &screen_rect);
        screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
        window_pos = (screen_res - window_size) * 0.5f;

        // init images here
    }
}
