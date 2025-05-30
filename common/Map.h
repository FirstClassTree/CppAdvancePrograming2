#ifndef MAP_H
#define MAP_H

#include "IJson.h"
#include "utils/Tile.h"
#include <iostream>
#include <string>
#include <vector>

class Map : public IJson {
public:
  Map(std::string name, int max_steps, int num_shells, int width, int height,
      std::vector<std::vector<Tile>> map);
  ~Map();
  std::string serialize() override;

  const std::vector<std::vector<Tile>> &get_map() const;
  void print_map() const;

  int get_rows() const;
  int get_cols() const;
  int get_max_steps() const;

private:
  std::vector<std::vector<Tile>> map;
  std::string name;
  int max_steps;
  int num_shells;
  int width;
  int height;
};



#endif