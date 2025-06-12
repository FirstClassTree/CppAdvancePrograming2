// common/Constants.cpp
#include "../common/Constants.h"
#include "../common/ConfigManager.h"


int ZERO_SHELLS_GRACE_STEPS = 40;
int SHOOT_COOLDOWN_STEPS = 4;
int SHELL_SPEED = 2;

void initialize_config() {
    ZERO_SHELLS_GRACE_STEPS = ConfigManager::instance().getZeroShellsGraceSteps();
    SHOOT_COOLDOWN_STEPS = ConfigManager::instance().getShootCooldownSteps();
    SHELL_SPEED = ConfigManager::instance().getShellSpeed();
}
