#include "../../common/algorithms/TankBattleInfo.h"

TankBattleInfo::TankBattleInfo(int ammo, std::vector<std::vector<char>> view) {
  this->ammo = ammo;
  this->view = view;
}

TankBattleInfo::~TankBattleInfo() {}

int TankBattleInfo::get_ammo() const { return this->ammo; }

std::vector<std::vector<char>> TankBattleInfo::get_view() { return this->view; }