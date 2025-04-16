#ifndef GAME_ALGO_HPP
#define GAME_ALGO_HPP

#include <utils.hpp>
#include <vector>
#include <math.h>
#include <entities.hpp>

using namespace std;
Action chase_enemy(Tank* self, Tank* target, const vector<vector<Tile>>& board);
Action not_so_smart_move();
vector<Entity*> scan_proximity();
vector<Entity*> scan_directions();
vector<Entity*> on_line_of_sight();
struct DFSAction{
    Direction dir;
    int value;
};

#endif