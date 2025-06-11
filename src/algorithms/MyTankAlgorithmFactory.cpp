#include "../common/algorithms/MyTankAlgorithmFactory.h"
#include "../common/algorithms/ChaserTankAlgorithm.h"
#include "../common/algorithms/SniperTankAlgorithm.h"

MyTankAlgorithmFactory::MyTankAlgorithmFactory() {}

MyTankAlgorithmFactory::~MyTankAlgorithmFactory() {}

std::unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) {
    // every 4 sniper tanks
<<<<<<< HEAD
    if((tank_index % 2 ) == 0 ){
=======
    if(((tank_index +1) % 4) == 0 ){
>>>>>>> ffe609fa5683a64988b755ff1ce7e4fbf445ee39
        return std::make_unique<ChaserTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    } else {
        return std::make_unique<SniperTankAlgorithm>(player_index, tank_index,this->tank_x,this->tank_y);
    }
}