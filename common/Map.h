#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
class Map {
public:
  Map(std::string name, int max_steps, int num_shells, int width, int height,
      std::vector<std::vector<char>> map);
  ~Map();
  const std::vector<std::vector<char>> &get_map() const;
  void print_map() const;

private:
  std::vector<std::vector<char>> map;
  std::string name;
  int max_steps;
  int num_shells;
  int width;
  int height;
};

#endif