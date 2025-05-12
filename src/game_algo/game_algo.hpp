#ifndef GAME_ALGO_HPP
#define GAME_ALGO_HPP

#include "utils.hpp"
#include <vector>
#include <math.h>
#include "entities.hpp"
#include <set>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;
Action chase_enemy(Tank* self, Tank* target, const vector<vector<Tile>>& board);
Action not_so_smart_move(Tank *self, Tank *target, const vector<Shell *> &shells,int w, int h, const vector<vector<Tile>> &board);
Action is_targeted(Tank* self,const vector<Shell*>& shells, int w, int h);
Action rotate_toward(Direction from, Direction to);
pair<int,int> find_shortest(int w, int h, int sx, int sy, int gx, int gy);
bool on_line_of_sight(LivingEntity* self,LivingEntity* target);
Action chase_enemy_bfs(Tank *self, Tank *target, const vector<vector<Tile>> &board);
struct DFSAction{
    Direction dir;
    int value;
};
set<pair<int, int>> get_shell_danger_zones(const vector<Shell *> &shells, int w, int h);
#endif