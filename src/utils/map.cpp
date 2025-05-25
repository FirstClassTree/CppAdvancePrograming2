#include "../../common/Map.h"
#include "utils/Tile.h"

Map::Map(std::string name, int max_steps, int num_shells, int width, int height,
         std::vector<std::vector<Tile>> map)
    : name(name), max_steps(max_steps), num_shells(num_shells), width(width),
      height(height), map(map) {}

Map::~Map() {}

const std::vector<std::vector<Tile>> &Map::get_map() const { return map; }

void Map::print_map() const {
  std::cout << "Map: " << name << std::endl;
  std::cout << "Max Steps: " << max_steps << std::endl;
  std::cout << "Num Shells: " << num_shells << std::endl;
  std::cout << "Width: " << width << std::endl;
  std::cout << "Height: " << height << std::endl;
  for (const auto &row : map) {
    for (Tile tile : row) {
      if (!tile.ground.expired()) {
        std::cout << (tile.ground.lock()->get_type() == EntityType::WALL ? "#"
                                                                         : "@");
        continue;
      } else if (!tile.actor.expired()) {
        std::cout << tile.actor.lock()->get_id();
      } else if (!tile.shell.expired()) {
        std::cout << "S";
      } else {
        std::cout << " ";
        std::cout << std::endl;
      }
    }
  }
}

std::string Map::serialize() {
  nlohmann::json j;
  j["name"] = name;
  j["max_steps"] = max_steps;
  j["num_shells"] = num_shells;
  j["width"] = width;
  j["height"] = height;
  j["map"] = "placeholder"; // TODO: implement map serialization
  return j.dump();
}