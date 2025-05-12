#include "entities.hpp"

Wall::Wall(int pos_x, int pos_y) : Entity(pos_x, pos_y)
{
    this->health = 2;
}

void Wall::hit(){
    this->health -=1;
}
EntityType Wall::get_type() const {
    return EntityType::Wall;
}
void Wall::update_pos(int x, int y){
    this->pos_x=x;
    this->pos_y=y;
}
Pos Wall::get_pos(){
    return Pos{this->pos_x,this->pos_y};
}

Action Wall::colide(Entity* e){
    EntityType type = e->get_type();
    if(type == EntityType::Mine){
        return get_none_action();
    } else if(type == EntityType::Shell){
        return get_none_action();
    } else if(type == EntityType::Wall){
        return get_none_action();
    } else if(type == EntityType::Tank){
        return get_hit_action();
    }
    return get_none_action();
}