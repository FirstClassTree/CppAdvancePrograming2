#ifndef UTILS_HPP
#define UTILS_HPP

enum class Direction {
    U,
    UR,
    R,
    DR,
    D,
    DL,
    L,
    UL,
};
enum class ActionType{
    Move,
    Shoot,
    Stay,
    Hit,
    None
};
class Logger{};
struct Action{
    ActionType type;
    int x =0;
    int y =0;
};
struct Tile {
    Entity* ground = nullptr;
    Entity* actor = nullptr;
    Shell* projectile = nullptr;
};
Action get_none_action();
Action get_hit_action();
#endif