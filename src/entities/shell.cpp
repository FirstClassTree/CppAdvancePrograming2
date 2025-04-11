#include "entities.hpp"

Shell::Shell(int owner,int pos_x,int pos_y):LivingEntity(pos_x,pos_y){
    this->owner = owner;
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->speed = 2;
}

int Shell::get_owner(){
    return this->owner;
}