#include "../common/Logger.h"
#include "../common/entities/Tank.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../common/algorithms/ChaserTankAlgorithm.h"
#include "../common/algorithms/SniperTankAlgorithm.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setRound(int round) {
    this->stepCount = 0;
    currentRound = round;
}

void Logger::logTankAction(const Tank& tank, const std::string& action) {
    ++stepCount;
    auto& ai = tank.get_ai();
    std::string algoName = "Unknown";

    if (dynamic_cast<const SniperTankAlgorithm*>(&ai)) {
        algoName = "Sniper";
    } else if (dynamic_cast<const ChaserTankAlgorithm*>(&ai)) {
        algoName = "Chaser";
    }

    std::ostringstream oss;
    oss << "[Round " << currentRound << ", Step " << stepCount << "] "
        << "Player " << tank.get_owner_id()
        << " Tank " << tank.get_tank_id()
        << " (" << algoName << ") -> Action: " << action
        << " | Ammo: " << tank.get_shell_num()
        << ", Cooldown: " << tank.get_cooldown();

    logs.push_back(oss.str());

    // Optional: live debugging
    // std::cout << oss.str() << std::endl;
}

void Logger::flush(const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& line : logs) {
        out << line << '\n';
    }
}
