#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

void ui::render() {
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
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
    }
    ImGui::End();
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
