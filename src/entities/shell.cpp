#include "entities.hpp"

Shell::Shell(int pos_x,int pos_y,Direction dir):LivingEntity(pos_x,pos_y,dir){
    this->speed = 2;
}
Action Shell::declare_move(){
    std::pair<int,int> dir_d = get_direction_delta(this->entity_dir);
    return Action{ActionType::Move,this->pos_x+dir_d.first,this->pos_y+dir_d.second};
}

void Shell::update_pos(int x, int y){
    this->pos_x =x;
    this->pos_y =y;
}
Pos Shell::get_pos(){
    return Pos{this->pos_x,this->pos_y};
}
EntityType Shell::get_type() const {
    return EntityType::Shell;
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