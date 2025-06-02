#ifndef CHASER_TANK_ALGORITHM_H
#define CHASER_TANK_ALGORITHM_H

#include "../TankAlgorithm.h"
#include "TankBattleInfo.h"
#include <vector>
#include <queue>
#include "../Direction.h"

struct Step { int x, y; };

class ChaserTankAlgorithm : public TankAlgorithm {
public:
  ChaserTankAlgorithm();
  ~ChaserTankAlgorithm();

  void updateBattleInfo(BattleInfo &battle_info);
  ActionRequest getAction();
  void make_decision(std::vector<std::vector<char>> &grid, int startX,
                     int startY, int targetX, int targetY,Direction tankDirection);

private:
  ActionRequest chooseAction;
  void bfs(std::vector<std::vector<bool>> &grid, int startX, int startY,
           int targetX, int targetY,Direction tankDirection);
  std::vector<std::vector<bool>>
  make_bfs_grid(std::vector<std::vector<char>> &grid);
  Direction get_direction_from_step(Step cur, Step prv);
  ActionRequest rotate_toward(Direction from, Direction to);
};

#endif