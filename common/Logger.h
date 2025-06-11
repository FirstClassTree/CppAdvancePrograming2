#pragma once
#include <string>
#include <vector>

class Tank;  // Forward declaration

class Logger {
public:
    static Logger& getInstance();

    void setRound(int round);

    void logTankAction(const Tank& tank, const std::string& action);

    void flush(const std::string& filename = "log.txt");

private:
    Logger() = default;

    int currentRound = 0;
    int stepCount = 0;
    std::vector<std::string> logs;
};
