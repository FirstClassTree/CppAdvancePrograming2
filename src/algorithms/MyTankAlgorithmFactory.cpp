#include "../common/algorithms/MyTankAlgorithmFactory.h"
#include "../common/algorithms/ChaserTankAlgorithm.h"
#include "../common/algorithms/SniperTankAlgorithm.h"

MyTankAlgorithmFactory::MyTankAlgorithmFactory() {}

MyTankAlgorithmFactory::~MyTankAlgorithmFactory() {}

std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) {
    // switch between algoirthms
    if(((tank_index +player_index) % 2 )   == 0 ){
        std::cout << "chaser generated" << std::endl;
        return std::make_unique<ChaserTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    } else {
        std::cout << "sniper generated" << std::endl;
        return std::make_unique<SniperTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    }
}