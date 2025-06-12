// common/Constants.h
#pragma once
// For ouput
constexpr int MAX_PLAYERS = 9;

extern int ZERO_SHELLS_GRACE_STEPS;
extern int SHOOT_COOLDOWN_STEPS;
extern int SHELL_SPEED;

void initialize_config();

// Pre adding config, revert to this if dislike config file:
//constexpr int ZERO_SHELLS_GRACE_STEPS = 40;
//constexpr int SHOOT_COOLDOWN_STEPS = 4;
//constexpr int MAX_PLAYERS = 9;
//constexpr int SHELL_SPEED = 2;
