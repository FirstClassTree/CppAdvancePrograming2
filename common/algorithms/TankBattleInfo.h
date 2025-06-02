#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "../BattleInfo.h"
#include "../Direction.h"
class TankBattleInfo : public BattleInfo{
public:
    TankBattleInfo(int ammo,int tank_x,int tank_y,Direction tank_direction);
    ~TankBattleInfo();

    int get_ammo();
    int get_tank_x();
    int get_tank_y();
    Direction get_tank_direction();


private:
    int player_id;
    int ammo;
    int tank_x;
    int tank_y;
    Direction tank_direction;
};

#endif