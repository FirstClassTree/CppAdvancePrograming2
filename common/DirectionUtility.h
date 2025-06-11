#pragma once
#include <utility>
#include <vector>
#include <algorithm>
#include "../common/Direction.h"
#include "ActionRequest.h"
#include <string>

inline std::pair<int, int> get_direction_offset(Direction d) {
    switch (d) {
        case Direction::U:  return {-1,  0};
        case Direction::D:  return { 1,  0};
        case Direction::L:  return { 0, -1};
        case Direction::R:  return { 0,  1};
        case Direction::UL: return {-1, -1};
        case Direction::UR: return {-1,  1};
        case Direction::DL: return { 1, -1};
        case Direction::RD: return { 1,  1};
        default:            return { 0,  0};
    }
}

// need to calculate by degrees
inline Direction rotate(Direction dir, int degrees) {
    static const std::vector<Direction> dirs = {
        Direction::U, Direction::UR, Direction::R, Direction::RD,
        Direction::D, Direction::DL, Direction::L, Direction::UL
    };
    auto it = std::find(dirs.begin(), dirs.end(), dir);
    if (it == dirs.end()) return dir;

    int index = std::distance(dirs.begin(), it);
    int steps = degrees / 45;
    int new_index = (index + steps + dirs.size()) % dirs.size();
    return dirs[new_index];
}
inline std::string actionRequestToString(ActionRequest action) {
    switch (action) {
        case ActionRequest::MoveForward: return "MoveForward";
        case ActionRequest::MoveBackward: return "MoveBackward";
        case ActionRequest::RotateLeft45: return "RotateLeft45";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::RotateLeft90: return "RotateLeft90";
        case ActionRequest::RotateRight90: return "RotateRight90";
        case ActionRequest::Shoot: return "Shoot";
        case ActionRequest::DoNothing: return "DoNothing";
        case ActionRequest::GetBattleInfo: return "GetBattleInfo";
        default: return "Unknown";
    }
}

