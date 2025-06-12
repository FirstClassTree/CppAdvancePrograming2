#pragma once
#include <string>
#include "json.hpp"

class ConfigManager {
public:
    static ConfigManager& instance();

    void loadFromFile(const std::string& filename);

    int getZeroShellsGraceSteps() const;
    int getShootCooldownSteps() const;
    int getMaxPlayers() const;
    int getShellSpeed() const;

private:
    ConfigManager() = default;
    void loadDefaults();

    nlohmann::json config_;
};
