#ifndef MY_TANK_ALGORITHM_FACTORY_H
#define MY_TANK_ALGORITHM_FACTORY_H

#include "../TankAlgorithm.h"
#include <memory>

class MyTankAlgorithmFactory {
public:
  MyTankAlgorithmFactory();
  ~MyTankAlgorithmFactory();
  std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index);

  int tank_x=0;
  int tank_y=0;
};

#endif