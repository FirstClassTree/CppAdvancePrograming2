#ifndef TANK_H
#define TANK_H

#include "../MoveableEntity.h"
#include "../TankAlgorithm.h"

class Tank : public MoveableEntity {
public:
  std::unique_ptr<TankAlgorithm> ai;

  Tank(int x, int y, Direction direction, int player_owner,
       std::unique_ptr<TankAlgorithm> ai);
  ~Tank();

  int get_owner_id();
  int get_health();
  int get_tank_id();

  void set_tank_id(int tank_id);
  void set_ai(std::unique_ptr<TankAlgorithm> ai);
  TankAlgorithm& get_ai();

  std::string serialize() override;

private:
  int health;
  int player_owner;
  int tank_id;
  static int global_tank_counter;
};

#endif