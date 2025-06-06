#ifndef DIRECTION_H
#define DIRECTION_H
#include <utility>
#include <vector>

enum class Direction {
  U,
  UR,
  R,
  RD,
  D,
  DL,
  L,
  UL,
};


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


#endif