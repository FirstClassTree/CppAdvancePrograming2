#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Entity.h"
#include "Map.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include "GamePlayer.h"
#include "entities/Shell.h"
#include "entities/Tank.h"
#include "entities/Wall.h"
#include "entities/Mine.h"
#include "entities/Shell.h"
#include "Direction.h"
#include "GamePlayerFactory.h"

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
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::shared_ptr<Entity>> game_entities;
  std::vector<std::weak_ptr<Shell>> game_shells;
  std::vector<std::weak_ptr<Tank>> game_tanks;
  GamePlayerFactory player_factory;
};
void fill_remaining_rows(int start_row, int total_rows, int total_cols,
                         std::vector<std::vector<Tile>>& map);

#endif