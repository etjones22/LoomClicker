#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Windows.h>

using json = nlohmann::json;

// Replace with your GitHub repo details
const std::string GITHUB_API_URL = "https://api.github.com/repos/etjones22/LoomClicker/releases/latest";
const std::string CURRENT_VERSION = "B2"; // Replace with your app's current version
const std::string DOWNLOAD_PATH = "Loom.exe";

bool checkForUpdate() {
    try {
        cpr::Response response = cpr::Get(cpr::Url{ GITHUB_API_URL }, cpr::Header{ {"User-Agent", "Clicker-Updater"} });
        if (response.status_code == 200) {
            json release = json::parse(response.text);
            std::string latestVersion = release["tag_name"];
            std::string downloadUrl = release["assets"][0]["browser_download_url"];

            if (latestVersion != CURRENT_VERSION) {
                std::cout << "Update available: " << latestVersion << "\n";
                std::cout << "Downloading update...\n";

                // Download the update
                cpr::Response downloadResponse = cpr::Download(cpr::Url{ downloadUrl }, cpr::DownloadFile{ DOWNLOAD_PATH });
                if (downloadResponse.status_code == 200) {
                    std::cout << "Downloaded new version successfully.\n";
                    return true;
                }
                else {
                    std::cerr << "Failed to download update.\n";
                }
            }
            else {
                std::cout << "You are using the latest version.\n";
            }
        }
        else {
            std::cerr << "Failed to check for updates. HTTP Status: " << response.status_code << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error checking for updates: " << e.what() << "\n";
    }
    return false;
}

void launchNewVersion() {
    ShellExecute(nullptr, "open", DOWNLOAD_PATH.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

int main() {
    if (checkForUpdate()) {
        std::cout << "Launching new version...\n";
        launchNewVersion();
    }
    else {
        std::cout << "No updates available.\n";
    }

    return 0;
}
