#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "../BattleInfo.h"
#include "../Direction.h"
#include <vector>
class TankBattleInfo : public BattleInfo {
public:
  TankBattleInfo(int ammo, std::vector<std::vector<char>> view);
  ~TankBattleInfo();

  int get_ammo() const;
  int get_tank_x() const;
  int get_tank_y() const;
  std::vector<std::vector<char>> get_view();

private:
  int player_id;
  int ammo;
  std::vector<std::vector<char>> view;
};

#endif