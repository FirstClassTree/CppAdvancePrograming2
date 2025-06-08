#ifndef CHASER_TANK_ALGORITHM_H
#define CHASER_TANK_ALGORITHM_H

#include "../Direction.h"
#include "../TankAlgorithm.h"
#include "ActionRequest.h"
#include "TankBattleInfo.h"
#include <limits>
#include <queue>
#include <vector>

struct ChaserStep {
  int x, y;
};
struct ChaserTarget {
  int x, y;
  bool sync;
};
struct ChaserState {
  int x, y;
  Direction direction;
  bool sync;
}; // TODO: add ammo.

class ChaserTankAlgorithm : public TankAlgorithm {
public:
  ChaserTankAlgorithm(int player_index, int tank_index, int tank_x, int tank_y);
  ~ChaserTankAlgorithm();

  void updateBattleInfo(BattleInfo &battle_info);
  ActionRequest getAction();
  void make_decision(std::vector<std::vector<char>> &grid);

private:
  ActionRequest chooseAction;
  void locate_me(std::vector<std::vector<char>> &grid);
  void set_target(std::vector<std::vector<char>> &grid);
  void bfs(std::vector<std::vector<bool>> &grid);
  std::vector<std::vector<bool>>
  make_bfs_grid(std::vector<std::vector<char>> &grid);
  Direction get_direction_from_step(ChaserStep cur, ChaserStep prv);
  ActionRequest rotate_toward(Direction from, Direction to);

  void simulate_move();

  int player_index;
  int tank_index;
  bool dirty = true;
  ChaserState currentState;
  ChaserTarget currentTarget;
};

#endif