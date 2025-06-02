#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "BattleInfo.h"

class TankBattleInfo : public BattleInfo{
public:
    TankBattleInfo();
    ~TankBattleInfo();


private:
    int player_id;
    int health;
    int x;
    int y;
};

#endif