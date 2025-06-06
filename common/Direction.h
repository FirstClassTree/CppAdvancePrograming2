#ifndef DIRECTION_H
#define DIRECTION_H
#include <utility>

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



#endif