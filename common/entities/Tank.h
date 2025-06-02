#ifndef TANK_H
#define TANK_H

#include "../MoveableEntity.h"
#include "../TankAlgorithm.h"

class Tank : public MoveableEntity {
public:
  Tank(int x,int y,Direction direction,int player_owner);
  ~Tank();

  int get_id();
  int get_health();
  int get_tank_id();
  
  std::string serialize() override;


private:
  std::unique_ptr<TankAlgorithm> ai;
  static int global_tank_counter; // static counter shared by all Tanks for id
  int health;
  int player_owner;
  int tank_id;
};

#endif