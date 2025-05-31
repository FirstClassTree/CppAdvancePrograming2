#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Direction.h"
#include "Entity.h"
#include "GamePlayer.h"
#include "GamePlayerFactory.h"
#include "Map.h"
#include "entities/Mine.h"
#include "entities/Shell.h"
#include "entities/Tank.h"
#include "entities/Wall.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <tuple>

class GameManager {
public:
  GameManager();
  int load_map(const std::string &map_path);
  Map get_map();

  void subscribe_player(std::shared_ptr<Player> player);
  void subscribe_shell(std::shared_ptr<Shell> shell);
  void subscribe_entity(std::shared_ptr<Entity> entity);
  void subscribe_tank(std::shared_ptr<Tank> tank);

private:
  std::unique_ptr<Map> map;
  GamePlayerFactory player_factory;
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::shared_ptr<Entity>> game_entities;
  std::vector<std::weak_ptr<Shell>> game_shells;
  std::vector<std::weak_ptr<Tank>> game_tanks;

  void add_tank(char c, int row_idx, int i, int max_steps, int num_shells,  std::vector<std::vector<Tile>> &map);
  std::tuple<std::string,std::string, int, int, int, int> parse_map_metadata(std::ifstream &map_file);
  std::vector<std::vector<Tile>> build_map(int rows, int cols);
};

#endif