#include "entities/Shell.h"
#include "EntityType.h"

Shell::Shell(int x, int y, Direction direction)
    : MoveableEntity(x, y, 2, EntityType::SHELL, direction) {}

Shell::~Shell() {}

void Shell::destroy() {
    destroyed = true;
}

bool Shell::is_destroyed() const {
    return destroyed;
}



std::string Shell::serialize() {
  nlohmann::json j;
  j["type"] = "S";
  j["x"] = get_x();
  j["y"] = get_y();
  j["direction"] = get_direction();
  return j.dump();
}

