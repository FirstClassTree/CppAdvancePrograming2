#include "../../common/Map.h"

Map::Map(std::string name, int max_steps, int num_shells, int width, int height,
         std::vector<std::vector<char>> map)
    : name(name), max_steps(max_steps), num_shells(num_shells), width(width),
      height(height), map(map) {}

Map::~Map() {}

const std::vector<std::vector<char>> &Map::get_map() const { return map; }

void Map::print_map() const {
  std::cout << "Map: " << name << std::endl;
  std::cout << "Max Steps: " << max_steps << std::endl;
  std::cout << "Num Shells: " << num_shells << std::endl;
  std::cout << "Width: " << width << std::endl;
  std::cout << "Height: " << height << std::endl;
  for (const auto &row : map) {
    for (char c : row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
}