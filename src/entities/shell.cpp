#include "entities.hpp"

Shell::Shell(int owner,int pos_x,int pos_y,Direction dir):LivingEntity(pos_x,pos_y,dir){
    this->owner = owner;
    this->speed = 2;
}

int Shell::get_owner(){
    return this->owner;
}

Action Shell::colide(Entity* e){
    EntityType type = e->get_type();
    if(type == EntityType::Mine){
        return get_none_action();
    } else if(type == EntityType::Shell){
        return get_hit_action();
    } else if(type == EntityType::Wall){
        return get_hit_action();
    } else if(type == EntityType::Tank){
        return get_hit_action();
    }
}