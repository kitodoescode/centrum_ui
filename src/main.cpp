#include <windows.h>
#include <iostream>

#include "misc/logs/logs.h"
#include "overlay/overlay.h"
#include "misc/globals.h"

int main() {
	system("cls");
	logger.Banner(); 
    
    logger.Section("THREAD INITIALIZATION");

    logger.CustomLog(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "OVERLAY", "Launching overlay interface...");
    std::thread overlay(overlay::load_interface);
    overlay.detach();
    logger.CustomLog(FOREGROUND_GREEN | FOREGROUND_INTENSITY, "SUCCESS", "Overlay thread launched");

    logger.CustomLog(FOREGROUND_BLUE | FOREGROUND_INTENSITY, "RUNTIME", "Engine running... Press ENTER to terminate");
    logger.Separator();
    try {
        while (true) {
            if (globals::unattach) {
                logger.CustomLog(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, "SHUTDOWN", "Unattach signal received");
                break;
            }

            if (std::cin.peek() != EOF) {
                char c;
                if (std::cin.get(c)) {
                    if (c == '\n' || c == '\r') {
                        logger.CustomLog(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, "SHUTDOWN", "User requested termination");
                        break;
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (...) {
        logger.CustomLog(FOREGROUND_RED | FOREGROUND_INTENSITY, "ERROR", "Exception in main loop");
    }

    logger.CustomLog(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, "SHUTDOWN", "Initiating cleanup sequence...");

    globals::unattach = true;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    logger.CustomLog(FOREGROUND_GREEN | FOREGROUND_INTENSITY, "SHUTDOWN", "Engine terminated successfully");

	return 0;
}