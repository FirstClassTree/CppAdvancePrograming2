#include "../common/algorithms/MyTankAlgorithmFactory.h"
#include "../common/algorithms/ChaserTankAlgorithm.h"
MyTankAlgorithmFactory::MyTankAlgorithmFactory() {}

MyTankAlgorithmFactory::~MyTankAlgorithmFactory() {}

std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) {
    return std::make_unique<ChaserTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
}