#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

class Entity;
class Shell;
enum class Direction {
    U,
    UR,
    R,
    DR,
    D,
    DL,
    L,
    UL,
    None,
};
enum class GameStatus{
    Idle,
    Running,
    Winner,
    Tie
};
struct GameState{
    GameStatus status;
    int winner;
};
enum class ActionType{
    Move,
    Shoot,
    Stay,
    Hit,
    None,
    RotateR1,
    RotateR2,
    RotateL1,
    RotateL2,
    Reverse,
};
struct Action{
    ActionType type;
    int x =0;
    int y =0;
};
struct Pos {
    int x;
    int y;
};
struct Tile {
    Entity* ground = nullptr;
    Entity* actor = nullptr;
    Shell* projectile = nullptr;
};

class Logger{
    public:
    explicit Logger();
    void log(std::string line);
    void log_action(Action action,int player_id);
};

std::string game_status_to_string(GameStatus status);
std::string action_to_string(Action action);
Action get_none_action();
Action get_hit_action();
std::pair<int, int> get_direction_delta(Direction dir);
Direction get_direction_between(Pos from, Pos to);
int wrap_pos(int v,int max);

#endif