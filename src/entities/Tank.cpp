#include "entities/Tank.h"


Tank::Tank(int x, int y, Direction direction, int player_owner,int tank_id, std::unique_ptr<TankAlgorithm> ai)
    : MoveableEntity(x, y, 1, EntityType::TANK, direction),
      player_owner(player_owner), tank_id(tank_id){
        
      }

Tank::~Tank() {}

int Tank::get_owner_id() { return player_owner; }
int Tank::get_health() { return health; }
int Tank::get_tank_id() { return tank_id; }
int Tank::get_shell_num() { return shell_num; }

TankAlgorithm& Tank::get_ai() { return *ai; }

void Tank::set_shell_num(int shell_num) {this->shell_num = shell_num;}
void Tank::set_tank_id(int tank_id) { this->tank_id = tank_id; }
void Tank::set_ai(std::unique_ptr<TankAlgorithm> ai) { this->ai = std::move(ai); }

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