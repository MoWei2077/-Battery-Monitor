#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    std::string logFilePath = "/storage/emulated/0/Android/电池记录.txt";
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
        return 1;
    }
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::string sj = std::ctime(&currentTime);
        sj.pop_back(); 
        std::string bb;
        std::string dl;
        std::ifstream batteryInfo("/sys/class/power_supply/battery/capacity");
        if (batteryInfo) {
            std::getline(batteryInfo, bb);
            batteryInfo.close();
        } else {
            std::cerr << "Failed to read battery level." << std::endl;
            logFile.close();
            return 1;
        }
        // 电流 以微安进行计算
        std::ifstream currentInfo("/sys/class/power_supply/battery/current_now");
        if (currentInfo) {
            std::getline(currentInfo, dl);
            currentInfo.close();
        } else {
            std::cerr << "Failed to read battery current." << std::endl;
            logFile.close();
            return 1;
        }
        logFile << sj << " " << bb << " " << dl << std::endl;
        logFile.flush();
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }

    logFile.close();
    return 0;
}
