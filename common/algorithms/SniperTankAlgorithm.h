#ifndef SNIPER_TANK_ALGORITHM_H
#define SNIPER_TANK_ALGORITHM_H

#include "ActionRequest.h"
#include "Direction.h"
#include "TankAlgorithm.h"
#include "algorithms/TankBattleInfo.h"
#include <iostream>
#include <limits>
#include <queue>
enum PHASE {
  SCOUT,
  AIM,
};

struct SniperStep {
  int x, y;
};
struct SniperTarget {
  int x, y;
  bool sync;
};
struct SniperState {
  int x, y;
  Direction direction;
  bool sync;
  PHASE phase;
  int ammo;
  int cooldown;
};

class SniperTankAlgorithm : public TankAlgorithm {
public:
  SniperTankAlgorithm(int player_index, int tank_index, int tank_x, int tank_y);
  ~SniperTankAlgorithm();

  void updateBattleInfo(BattleInfo &battle_info);
  ActionRequest getAction();
  void set_target(std::vector<std::vector<char>> &grid);

private:
  void move_toward(std::vector<std::vector<char>> &grid);
  std::vector<SniperStep> get_candidates(std::vector<std::vector<char>> &grid);
  bool is_in_sight();
  Direction get_direction_from_step(SniperStep cur, SniperStep prv);
  bool aligned();
  void simulate_move();
  void locate_me(std::vector<std::vector<char>> &grid);
  ActionRequest rotate_toward(Direction from, Direction to);
  bool is_path_obstructed(std::vector<std::vector<char>> &grid,
                          const SniperStep &from, const SniperStep &to);
  void find_waypoint(std::vector<std::vector<char>> &grid);
  ActionRequest chooseAction;
  int player_index;
  int tank_index;
  bool dirty = true;
  SniperState currentState;
  SniperTarget currentTarget;
  SniperTarget waypoint;
};

#endif