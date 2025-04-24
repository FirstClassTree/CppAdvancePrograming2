#include "entities.hpp"

Tank::Tank(int x, int y,Direction dir) : LivingEntity(x, y, dir) {
}
EntityType Tank::get_type() const {
    return EntityType::Tank;
}
Tank* Tank::closest_target(std::vector<Tank*> others){
    double range = INT_MAX;
    Tank* target;

    for(Tank* tank : others){
        double distance = sqrt(pow((this->pos_x-tank->pos_x),2)+pow((this->pos_y-tank->pos_y),2));
        if(distance < range){
            range= distance;
            target = tank;
        }
    }
    return target;
}
int Tank::is_reloading(){
    return this->reload_state ==0 ? 0 : 1;
}
void Tank::shoot(){
    this->reload_state = 4;
    this->ammo--;
}
int Tank::get_ammo(){
    return this->ammo;
}
void Tank::tick(){
    if(this->reload_state !=0){
        this->reload_state--;
    }
}
int Tank::is_reverse(){
    return this->reverse_state == 3 ? 1 : 0;
}

void Tank::request_backwards(){
    if(this->reverse_state !=3){
        this->reverse_state++;
    }
}

void Tank::reset_reverse(){
    this->reverse_state = 0;
}

Action Tank::colide(Entity* e){
    EntityType type = e->get_type();
    if(type == EntityType::Mine){
        return get_hit_action();
    } else if(type == EntityType::Shell){
        return get_hit_action();
    } else if(type == EntityType::Wall){
        return get_hit_action();
    } else if(type == EntityType::Tank){
        return get_hit_action();
    }
}
Action Tank::declare_move(){
    return Action{ActionType::None};
}

void Tank::update_pos(int x, int y){
    this->pos_x=x;
    this->pos_y=y;
}
Pos Tank::get_pos(){
    return Pos{this->pos_x,this->pos_y};
}