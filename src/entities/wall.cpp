#include "entities.hpp"

Wall::Wall(int pos_x, int pos_y) : Entity(pos_x, pos_y)
{
    this->health = 2;
}

void Wall::hit(){
    this->health -=1;
}

Action Wall::colide(Entity* e){
    EntityType type = e->get_type();
    if(type == EntityType::Mine){
        //raise error
    } else if(type == EntityType::Shell){
        return get_none_action();
    } else if(type == EntityType::Wall){
        //raise error
    } else if(type == EntityType::Tank){
        return get_hit_action();
    }
}