#include <fstream>
#include <iostream>
#include "../common/ConfigManager.h"

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

void ConfigManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file) {
        try {
            file >> config_;
        } catch (...) {
            std::cerr << "Invalid JSON format in config file. Using defaults.\n";
            loadDefaults();
        }
    } else {
        std::cerr << "Config file not found. Using defaults.\n";
        loadDefaults();
    }
}

void ConfigManager::loadDefaults() {
    config_ = {
        {"zero_shells_grace_steps", 40},
        {"shoot_cooldown_steps", 4},
        {"shell_speed", 2}
    };
}

int ConfigManager::getZeroShellsGraceSteps() const {
    return config_.value("zero_shells_grace_steps", 40);
}

int ConfigManager::getShootCooldownSteps() const {
    return config_.value("shoot_cooldown_steps", 4);
}

int ConfigManager::getShellSpeed() const {
    return config_.value("shell_speed", 2);
}
