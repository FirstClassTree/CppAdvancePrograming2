#include "entities.hpp"

Wall::Wall(int pos_x, int pos_y) : Entity(pos_x, pos_y)
{
    this->health = 2;
}

void Wall::hit(){
    this->health -=1;
}