#ifndef TANK_H
#define TANK_H

#include "../MoveableEntity.h"
#include "../TankAlgorithm.h"

class Tank : public MoveableEntity {
public:
  Tank(int x,int y,Direction direction,int player_owner);
  ~Tank();

  int get_id();
  
  std::string serialize() override;


private:
  std::unique_ptr<TankAlgorithm> ai;
  int health;
  int player_owner;
};

#endif