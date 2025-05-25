#include "entities/Mine.h"

Mine::Mine(int x, int y) : Entity(x, y, EntityType::MINE) {}

Mine::~Mine() {}

std::string Mine::serialize() {
  nlohmann::json j;
  j["type"] = "@";
  j["x"] = get_x();
  j["y"] = get_y();
  return j.dump();
}

