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
#include "utils/Tile.h"

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

  
  std::unique_ptr<SatelliteView> create_satellite_view(int player_id, int tank_id) const;


  
private:
  std::unique_ptr<Map> map;
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::shared_ptr<Entity>> game_entities;
  std::vector<std::weak_ptr<Shell>> game_shells;
  std::vector<std::shared_ptr<Tank>> game_tanks;
  GamePlayerFactory player_factory;







  static std::ifstream open_map_file(const std::string &map_path);
    static bool parse_map_header(std::istream &in, std::string &name,
                                 int &max_steps, int &num_shells,
                                 int &rows, int &cols);
    static std::vector<std::vector<Tile>> create_empty_map(int rows, int cols);
    static void populate_map_row(const std::string &line, int row_idx, int cols,
                                 int max_steps, int num_shells,
                                 std::vector<std::vector<Tile>> &map,
                                 std::vector<std::shared_ptr<Tank>> &tanks_out,
                                 std::vector<std::shared_ptr<Entity>> &entities_out,
                                 std::vector<std::pair<int, std::pair<int, int>>> &player_spawn_points_out);
    static void fill_remaining_rows(int start_row, int rows, int cols,
                                    std::vector<std::vector<Tile>> &map);
  
};


#endif