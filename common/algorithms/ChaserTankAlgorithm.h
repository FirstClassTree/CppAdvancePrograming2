#ifndef CHASER_TANK_ALGORITHM_H
#define CHASER_TANK_ALGORITHM_H

#include "../TankAlgorithm.h"
#include "TankBattleInfo.h"
#include <vector>
#include <queue>
#include "../Direction.h"

struct Step { int x, y; };
struct Target { int x, y; bool sync;};
struct State { int x, y; Direction direction; bool sync;}; //TODO: add ammo.

class ChaserTankAlgorithm : public TankAlgorithm {
public:
  ChaserTankAlgorithm(int player_index, int tank_index,int tank_x,int tank_y);
  ~ChaserTankAlgorithm();

  void updateBattleInfo(BattleInfo &battle_info);
  ActionRequest getAction();
  void make_decision(std::vector<std::vector<char>> &grid);

private:
  ActionRequest chooseAction;
  void set_target(std::vector<std::vector<char>> &grid);
  void bfs(std::vector<std::vector<bool>> &grid);
  std::vector<std::vector<bool>>
  make_bfs_grid(std::vector<std::vector<char>> &grid);
  Direction get_direction_from_step(Step cur, Step prv);
  ActionRequest rotate_toward(Direction from, Direction to);

  void simulate_move();
  
  int player_index;
  int tank_index;
  bool dirty = true;
  State currentState;
  Target currentTarget;
};

#endif