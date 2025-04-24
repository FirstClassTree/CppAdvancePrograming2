#include "utils.hpp"



std::pair<int, int> get_direction_delta(Direction dir)
{
    switch (dir)
    {
    case Direction::U:
        return {0, -1};
    case Direction::UR:
        return {1, -1};
    case Direction::R:
        return {1, 0};
    case Direction::DR:
        return {1, 1};
    case Direction::D:
        return {0, 1};
    case Direction::DL:
        return {-1, 1};
    case Direction::L:
        return {-1, 0};
    case Direction::UL:
        return {-1, -1};
    }
    return {0, 0}; // fallback
}

int wrap(int v, int max)
{
    return (v % max + max) % max;
}

Direction get_direction_between(Pos from, Pos to)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    if (dx == 0 && dy == 0)
        return Direction::None;

    if (dx == dy && dx < 0)
        return Direction::UL;
    if (dx == dy && dx > 0)
        return Direction::DR;
    if (dx == 0 && dy > 0)
        return Direction::D;
    if (dx == 0 && dy < 0)
        return Direction::U;
    if (dy == 0 && dx > 0)
        return Direction::R;
    if (dy == 0 && dx < 0)
        return Direction::L;
    if (dx == dy * -1 && dx < 0)
        return Direction::DL;
    if (dx == dy * -1 && dx > 0)
        return Direction::UR;

    return Direction::None;
}