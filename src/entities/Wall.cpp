#include "entities/Wall.h"
#include "Entity.h"
#include "json.hpp"

// 2 is new 1 is weakend
Wall::Wall(int x, int y) : Entity(x, y, EntityType::WALL),health(2){}


std::string Wall::serialize() {
  nlohmann::json j;
  j["type"] = "#";
  j["x"] = get_x();
  j["y"] = get_y();
  return j.dump();
}

void Wall::weaken() {
  if (health > 0) --health;
}

bool Wall::is_destroyed() const {
  return health <= 0;
}