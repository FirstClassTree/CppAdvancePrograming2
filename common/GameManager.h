#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Entity.h"
#include "GamePlayer.h"
#include "Map.h"
#include "algorithms/MyTankAlgorithmFactory.h"
#include "entities/Mine.h"
#include "entities/Shell.h"
#include "entities/Tank.h"
#include "entities/Wall.h"
#include "utils/Tile.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>

#include "Direction.h"
#include "GamePlayerFactory.h"
#include "../common/OutputPrinter.h"

class GameManager {
public:
  GameManager();
  int load_map(const std::string &map_path);
  Map get_map();

  void subscribe_player(std::shared_ptr<Player> player);
  void subscribe_shell(std::shared_ptr<Shell> shell);
  void subscribe_entity(std::shared_ptr<Entity> entity);
  void subscribe_tank(std::shared_ptr<Tank> tank);
  std::vector<std::shared_ptr<Tank>> get_tanks();
  const std::shared_ptr<Tank> get_tank(int tankIndex,int playerIndex) const;
  const std::vector<std::unique_ptr<Player>> &get_players() const;
  
  std::unique_ptr<SatelliteView> create_satellite_view(int player_id,
                                                       int tank_id) const;



private:
  std::unique_ptr<Map> map;
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::shared_ptr<Entity>> game_entities;
  std::vector<std::shared_ptr<Shell>> game_shells;
  std::vector<std::shared_ptr<Tank>> game_tanks;
  GamePlayerFactory player_factory;
  MyTankAlgorithmFactory tank_algorithm_factory;

  // Collect actions from all living tanks
  std::vector<std::pair<std::shared_ptr<Tank>, ActionRequest>> collect_tank_actions();



  void run();

  // Apply collected actions, update game state and log to OutputPrinter
  void apply_tank_actions(
      const std::vector<std::pair<std::shared_ptr<Tank>, ActionRequest>>& actions,
      OutputPrinter& printer);

  // Update shells, resolve collisions, handle deaths etc.
  void update_game_state();

  // Return true if game should terminate
  bool check_end_conditions(int current_step, int& steps_without_shells);

  // Determine the winner (-1 = tie, otherwise 1 or 2)
  int determine_winner(const std::vector<int>& tanks_per_player);

  void post_load_process();

  static std::ifstream open_map_file(const std::string &map_path);
  static bool parse_map_header(std::istream &in, std::string &name,
                               int &max_steps, int &num_shells, int &rows,
                               int &cols);
  static std::vector<std::vector<Tile>> create_empty_map(int rows, int cols);
  static void populate_map_row(
      const std::string &line, int row_idx, int cols, int max_steps,
      int num_shells, std::vector<std::vector<Tile>> &map,
      std::vector<std::shared_ptr<Tank>> &tanks_out,
      std::vector<std::shared_ptr<Entity>> &entities_out,
      std::vector<std::pair<int, std::pair<int, int>>> &player_spawn_points_out,
      MyTankAlgorithmFactory &tank_algorithm_factory);
  static void fill_remaining_rows(int start_row, int rows, int cols,
                                  std::vector<std::vector<Tile>> &map);
};

#endif