#include "entities/Tank.h"

Tank::Tank(int x, int y, Direction direction, int player_owner)
    : MoveableEntity(x, y, 1, EntityType::TANK, direction),
      player_owner(player_owner) {}

Tank::~Tank() {}

int Tank::get_id() { return player_owner; }

std::string Tank::serialize() {
  nlohmann::json j;
  j["type"] = "T";
  j["x"] = get_x();
  j["y"] = get_y();
  j["direction"] = get_direction();
  j["player_owner"] = player_owner;
  return j.dump();
}