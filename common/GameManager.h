#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Map.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>

class GameManager {
public:
  GameManager();
  int load_map(const std::string &map_path);
  Map get_map();
private:
  std::unique_ptr<Map> map;
};

#endif