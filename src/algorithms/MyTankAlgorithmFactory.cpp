#include "../common/algorithms/MyTankAlgorithmFactory.h"
#include "../common/algorithms/ChaserTankAlgorithm.h"
#include "../common/algorithms/SniperTankAlgorithm.h"

MyTankAlgorithmFactory::MyTankAlgorithmFactory() {}

MyTankAlgorithmFactory::~MyTankAlgorithmFactory() {}

std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) {
    // every 4 sniper tanks
    if((tank_index % 4 + 1) == 0 ){
        return std::make_unique<ChaserTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    } else {
        return std::make_unique<SniperTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    }
}