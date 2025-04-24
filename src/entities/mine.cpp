#include "entities.hpp"

Mine::Mine(int pos_x,int pos_y): Entity(pos_x,pos_y){
            
    this->pos_x = pos_x;
    this->pos_y = pos_y;
}
void Mine::update_pos(int x, int y){
}
Pos Mine::get_pos(){
    return Pos{this->pos_x,this->pos_y};
}
EntityType Mine::get_type() const {
    return EntityType::Mine;
}
Action Mine::colide(Entity* e){
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