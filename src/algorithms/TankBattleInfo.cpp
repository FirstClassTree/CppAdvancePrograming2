#include "../../common/algorithms/TankBattleInfo.h"

TankBattleInfo::TankBattleInfo(int ammo,int tank_x,int tank_y,Direction tank_direction) {
    this->ammo = ammo;
    this->tank_x = tank_x;
    this->tank_y = tank_y;
    this->tank_direction = tank_direction;
}

TankBattleInfo::~TankBattleInfo() {}

int TankBattleInfo::get_ammo() {
    return this->ammo;
}

int TankBattleInfo::get_tank_x() {
    return this->tank_x;
}

int TankBattleInfo::get_tank_y() {
    return this->tank_y;
}

Direction TankBattleInfo::get_tank_direction() {
    return this->tank_direction;
}