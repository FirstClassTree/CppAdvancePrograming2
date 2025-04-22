#ifndef GAME_ALGO_HPP
#define GAME_ALGO_HPP

#include <utils.hpp>
#include <vector>
#include <math.h>
#include <entities.hpp>
#include <set>

using namespace std;
Action chase_enemy(Tank* self, Tank* target, const vector<vector<Tile>>& board);
Action not_so_smart_move(Tank *self, Tank *target, const vector<Shell *> &shells,int w, int h, const vector<vector<Tile>> &board);
Action is_targeted(Tank* self,const vector<Shell*>& shells);
Action rotate_toward(Direction from, Direction to);
bool on_line_of_sight(LivingEntity* self,LivingEntity* target);
struct DFSAction{
    Direction dir;
    int value;
};

#endif