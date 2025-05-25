#include "entities/Wall.h"
#include "Entity.h"
#include "json.hpp"

Wall::Wall(int x, int y) : Entity(x, y, EntityType::WALL) {}

Wall::~Wall() {}


std::string Wall::serialize() {
  nlohmann::json j;
  j["type"] = "#";
  j["x"] = get_x();
  j["y"] = get_y();
  return j.dump();
}