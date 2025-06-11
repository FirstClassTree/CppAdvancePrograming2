#include "entities/Tank.h"
#include <iostream>


Tank::Tank(int x, int y, Direction direction, int player_owner,int tank_id, std::unique_ptr<TankAlgorithm> ai)
    : MoveableEntity(x, y, 1, EntityType::TANK, direction),
      player_owner(player_owner), tank_id(tank_id),health(1),backward_state(BackwardState::None){
        
      }

Tank::~Tank() {}



int Tank::get_owner_id() const{ return player_owner; }
int Tank::get_health()const { return health; }
int Tank::get_tank_id() const{ return tank_id; }
int Tank::get_shell_num()const { return this->shell_num; }
int Tank::get_cooldown() const{return shoot_cooldown;} 


TankAlgorithm& Tank::get_ai() const { return *ai; }

void Tank::set_shell_num(int shell_num) {this->shell_num = shell_num;}
void Tank::set_tank_id(int tank_id) { this->tank_id = tank_id; }
void Tank::set_ai(std::unique_ptr<TankAlgorithm> ai) { this->ai = std::move(ai); }



bool Tank::can_shoot() const {
    return shell_num > 0 && shoot_cooldown == 0;
}

void Tank::mark_shot() {
    if (can_shoot()) {
        shell_num--;
        // +1 because each round removes cooldown including shooting round
        shoot_cooldown = SHOOT_COOLDOWN_STEPS;
    }
}

void Tank::tick_cooldown() {
    if (shoot_cooldown > 0) {
        shoot_cooldown--;
    }
}


// Substruct 1 instead if planning on multiple lives
void Tank::damage() {
    this->health = 0;  
}

void Tank::start_backward_sequence() {
    backward_state = BackwardState::Waiting1;
}

void Tank::cancel_backward_sequence() {
        backward_state = BackwardState::None;
}

bool Tank::is_ready_to_move_backward() const {
    return backward_state == BackwardState::ReadyFast;
}

void Tank::advance_backward_state() {
    if (backward_state == BackwardState::Waiting1) {
        backward_state = BackwardState::Waiting2;
    } else if (backward_state == BackwardState::Waiting2) {
        backward_state = BackwardState::ReadyFast;
    }
}

BackwardState Tank::get_backward_state() const {
    return backward_state;
}



std::string Tank::serialize() {
  nlohmann::json j;
  j["type"] = "T";
  j["x"] = get_x();
  j["y"] = get_y();
  j["direction"] = get_direction();
  j["player_owner"] = player_owner;
  return j.dump();
}
//