#include "../../common/GameManager.h"
#include "MySatelliteView.h"
#include "entities/Wall.h"
#include "utils/Tile.h"

// int extractIntValue(const std::string &line, const std::string &key) {
//   std::regex pattern("^\\s*" + key + "\\s*=\\s*(\\d+)\\s*$");
//   std::smatch match;
//   if (std::regex_match(line, match, pattern)) {
//     return std::stoi(match[1]);
//   } else {
//     throw std::runtime_error("Invalid format for line: " + line);
//   }
// }

GameManager::GameManager() {
  this->player_factory = GamePlayerFactory();
  this->tank_algorithm_factory = MyTankAlgorithmFactory();
}

void GameManager::subscribe_tank(std::shared_ptr<Tank> tank) {
  this->subscribe_entity(tank);
  this->game_tanks.push_back(tank);
}

const std::vector<std::unique_ptr<Player>> &GameManager::get_players() const {
  return players;
}

const std::shared_ptr<Tank> GameManager::get_tank(int tankIndex,
                                                  int playerIndex) const {
  for (const auto &tank : game_tanks) {
    if (tank->get_tank_id() == tankIndex &&
        tank->get_owner_id() == playerIndex) {
      return tank;
    }
  }
  return nullptr;
}

void GameManager::subscribe_entity(std::shared_ptr<Entity> entity) {
  this->game_entities.push_back(entity);
}
void GameManager::subscribe_shell(std::shared_ptr<Shell> shell) {
  this->subscribe_entity(shell);
  this->game_shells.push_back(shell);
}

Map GameManager::get_map() {

  if (!this->map) {
    // For a test, it might be better to throw or assert
    throw std::runtime_error("GameManager::map is null in get_map()!");
  }
  return *this->map;
}

std::vector<std::shared_ptr<Tank>> GameManager::get_tanks() {
  return this->game_tanks;
}

int GameManager::load_map(const std::string &map_path) {
  std::ifstream map_file = open_map_file(map_path);
  if (!map_file.is_open())
    return -1;

  std::string name;
  int max_steps, num_shells, rows, cols;
  if (!parse_map_header(map_file, name, max_steps, num_shells, rows, cols))
    return -1;

  auto map = create_empty_map(rows, cols);
  std::vector<std::shared_ptr<Tank>> tanks_out;
  std::vector<std::shared_ptr<Entity>> entities_out;
  std::vector<std::pair<int, std::pair<int, int>>> player_spawn_points_out;

  std::string line;
  int row_idx = 0;
  while (std::getline(map_file, line)) {
    if (row_idx >= rows) {
      std::cerr << "[GameManager::load_map] Warning: extra rows in file\n";
      break;
    }
    populate_map_row(line, row_idx, cols, max_steps, num_shells, map, tanks_out,
                     entities_out, player_spawn_points_out,
                     tank_algorithm_factory);
    ++row_idx;
  }

  // May be unceearry:
  if (row_idx < rows)
    fill_remaining_rows(row_idx, rows, cols, map);

  for (auto &entity : entities_out)
    subscribe_entity(entity);
  for (auto &tank : tanks_out)
    subscribe_tank(tank);

  for (const auto &[player_num, pos] : player_spawn_points_out) {
    bool exists = std::any_of(
        players.begin(), players.end(), [player_num](const auto &p) {
          auto *gp = dynamic_cast<const GamePlayer *>(p.get());
          return gp && gp->get_id() == player_num;
        });
    if (!exists) {
      players.emplace_back(player_factory.create(
          player_num, pos.first, pos.second, max_steps, num_shells));
    }
  }

  this->map =
      std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);

  this->post_load_process();
  return 0;
}

void GameManager::post_load_process() {
  int tank_ids[10] = {0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0}; // Assumes player IDs 0-9 can have tanks

  for (auto &tank_sp : game_tanks) { // Changed variable name tank to tank_sp
                                     // for clarity (shared_ptr)
    if (!tank_sp) {
      continue;
    }

    tank_algorithm_factory.tank_x = tank_sp->get_x();
    tank_algorithm_factory.tank_y = tank_sp->get_y();

    int owner_id = tank_sp->get_owner_id();

    // Basic bounds check for tank_ids array access. Player IDs from maps are
    // usually 1-9. If owner_id is outside 0-9, this logic needs more robust
    // error handling or clarification on how player IDs are managed vs. array
    // indexing.
    if (owner_id < 0 ||
        owner_id >= 10) { // Check against 0-9 for tank_ids array
      std::cerr
          << "Error: Tank (owner_id=" << owner_id
          << ") has an ID out of expected range [0-9] for tank_ids array in "
             "post_load_process. Skipping tank association with player.\n";
      // Still proceed to set AI and tank_id if possible, but skip player
      // association. Depending on requirements, might want to skip this tank
      // entirely. For now, we will attempt to set AI and ID if owner_id is
      // valid for create() and set_tank_id(). The create factory method might
      // have its own validation for player_index.
    } else {
      // Only increment tank_ids[owner_id] if owner_id is a valid index.
      int current_tank_id_for_owner = tank_ids[owner_id];
      tank_sp->set_ai(
          tank_algorithm_factory.create(owner_id, current_tank_id_for_owner));
      tank_sp->set_tank_id(current_tank_id_for_owner);
      tank_ids[owner_id]++;
    }

    GamePlayer *target_player = nullptr;
    for (const auto &player_up :
         this->players) { // player_up is std::unique_ptr<Player>
      if (player_up) {    // Check unique_ptr is not null
        GamePlayer *gp = dynamic_cast<GamePlayer *>(player_up.get());
        if (gp && gp->get_id() == owner_id) {
          target_player = gp;
          break;
        }
      }
    }

    if (target_player) {
      target_player->tanks.push_back(
          tank_sp); // GamePlayer::tanks is std::vector<std::weak_ptr<Tank>>
    } else {
      std::cerr << "Error: Player with ID " << owner_id
                << " not found for tank in post_load_process.\n";
      // This may or may not be a critical error depending on game rules.
    }
  }
}

std::unique_ptr<SatelliteView>
GameManager::create_satellite_view(int player_id, int tank_id) const {
  if (!this->map) {
    std::cerr
        << "Error: GameManager::create_satellite_view called with null map."
        << std::endl;
    throw std::runtime_error(
        "GameManager: Map is not loaded, cannot create satellite view.");
  }

  size_t rows = this->map->get_rows();
  size_t cols = this->map->get_cols();

  if (rows == 0 || cols == 0) {
    std::cerr << "Error: GameManager::create_satellite_view called for map "
                 "with zero dimensions (rows="
              << rows << ", cols=" << cols << ")." << std::endl;
    throw std::runtime_error(
        "GameManager: Map has zero dimensions, cannot create satellite view.");
  }


  // Init to empty spaces
  std::vector<std::vector<char>> view(rows, std::vector<char>(cols, ' '));

  // Add Walls and Mines
  for (const auto &entity : game_entities) {
    if (!entity)
      continue;
    size_t x = entity->get_x();
    size_t y = entity->get_y();
    if (x < rows && y < cols) {
      switch (entity->get_type()) {
      case EntityType::WALL:
        view[x][y] = '#';
        break;
      case EntityType::MINE:
        view[x][y] = '@';
        break;
      default:
        break;
      }
    }
  }
  // Tanks
  for (const auto &tank : game_tanks) {
    if (!tank || tank->get_health() == 0)
      continue;
    size_t x = tank->get_x();
    size_t y = tank->get_y();
    if (x < rows && y < cols) {
      if (tank->get_owner_id() == player_id && tank->get_tank_id() == tank_id) {
        view[x][y] = '%'; // requesting tank
      } else {
        view[x][y] = static_cast<char>('0' + tank->get_owner_id());
      }
    }
  }

  // add Shells if not destroyed
  for (const auto &shell : game_shells) {
    if (!shell || shell->is_destroyed())
      continue;
    size_t x = shell->get_x();
    size_t y = shell->get_y();
    if (x < rows && y < cols) {
      view[x][y] = '*';
    }
  }
  // return the MySatelliteView

  return std::make_unique<MySatelliteView>(view);
}

std::ifstream GameManager::open_map_file(const std::string &map_path) {
  std::cout << "[open_map_file] Attempting to open map file: \"" << map_path
            << "\"\n";

  std::ifstream map_file(map_path);
  if (!map_file.is_open()) {
    std::cerr << "[open_map_file] ERROR: Failed to open map file: \""
              << map_path << "\"\n";
  } else {
    std::cout << "[open_map_file] Successfully opened map file.\n";
  }

  return map_file;
}

// Utility: strip spaces around '=' and parse int
int extractIntValue(const std::string &line, const std::string &expected_key) {
  auto pos = line.find('=');
  if (pos == std::string::npos)
    throw std::runtime_error("Missing '=' in: " + line);

  // Extract and clean the key part
  std::string key = line.substr(0, pos);
  key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

  if (key != expected_key)
    throw std::runtime_error("Expected key '" + expected_key + "', found '" +
                             key + "'");

  // Extract and clean the value part
  std::string value_str = line.substr(pos + 1);
  value_str.erase(std::remove_if(value_str.begin(), value_str.end(), ::isspace),
                  value_str.end());

  return std::stoi(value_str);
}

bool GameManager::parse_map_header(std::istream &in, std::string &name,
                                   int &max_steps, int &num_shells, int &rows,
                                   int &cols) {
  std::string line;
  try {
    if (!std::getline(in, name))
      throw std::runtime_error("Missing map name line");
    if (!std::getline(in, line))
      throw std::runtime_error("Missing MaxSteps line");
    max_steps = extractIntValue(line, "MaxSteps");

    if (!std::getline(in, line))
      throw std::runtime_error("Missing NumShells line");
    num_shells = extractIntValue(line, "NumShells");

    if (!std::getline(in, line))
      throw std::runtime_error("Missing Rows line");
    rows = extractIntValue(line, "Rows");

    if (!std::getline(in, line))
      throw std::runtime_error("Missing Cols line");
    cols = extractIntValue(line, "Cols");

  } catch (const std::exception &e) {
    std::cerr << "[parse_map_header] ERROR: " << e.what() << '\n';
    return false;
  }

  return true;
}

std::vector<std::vector<Tile>> GameManager::create_empty_map(int rows,
                                                             int cols) {
  std::vector<std::vector<Tile>> map(rows, std::vector<Tile>(cols));
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      map[r][c].x = r;
      map[r][c].y = c;
    }
  }

  return map;
}

// This function assumes that map[row_idx] is already initialized with x/y
void GameManager::populate_map_row(
    const std::string &line, int row_idx, int cols, int max_steps,
    int num_shells, std::vector<std::vector<Tile>> &map,
    std::vector<std::shared_ptr<Tank>> &tanks_out,
    std::vector<std::shared_ptr<Entity>> &entities_out,
    std::vector<std::pair<int, std::pair<int, int>>> &player_spawn_points_out,
    MyTankAlgorithmFactory &tank_algorithm_factory) {
  for (int col = 0; col < cols; ++col) {
    Tile &tile = map[row_idx][col];

    char c = (col < static_cast<int>(line.size())) ? line[col] : ' ';

    switch (c) {
    case '#': {
      auto wall = std::make_shared<Wall>(row_idx, col);
      tile.ground = wall;
      entities_out.push_back(wall);
      break;
    }
    case '@': {
      auto mine = std::make_shared<Mine>(row_idx, col);
      tile.shell = mine;
      entities_out.push_back(mine);
      break;
    }
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      int player_num = c - '0';
      Direction dir = (player_num == 1 ? Direction::L : Direction::R);
      auto tank =
          std::make_shared<Tank>(row_idx, col, dir, player_num, nullptr);
      tile.actor = tank;
      tanks_out.push_back(tank);
      player_spawn_points_out.emplace_back(player_num,
                                           std::make_pair(row_idx, col));
      break;
    }
    default:
      // Treated as empty space
      break;
    }
  }
}

void GameManager::fill_remaining_rows(int start_row, int rows, int cols,
                                      std::vector<std::vector<Tile>> &map) {
  for (int r = start_row; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      map[r][c].x = r;
      map[r][c].y = c;
    }
  }
}

// int GameManager::load_map(const std::string &map_path) {
//   std::cout << "[GameManager::load_map] Attempting to load map from: \""
//             << map_path << "\"" << std::endl; // DEBUG
//   std::ifstream map_file(map_path);
//   if (!map_file.is_open()) {
//     std::cerr << "[GameManager::load_map] ERROR: Failed to open map file: \""
//               << map_path << "\"" << std::endl; // DEBUG
//     return -1;
//   }
//   std::cout << "[GameManager::load_map] Successfully opened map file: \""
//             << map_path << "\"" << std::endl; // DEBUG
//   std::string line;

//   std::getline(map_file, line);
//   std::string name = line;
//   std::getline(map_file, line);
//   int max_steps = extractIntValue(line, "MaxSteps");
//   std::getline(map_file, line);
//   int num_shells = extractIntValue(line, "NumShells");
//   std::getline(map_file, line);
//   int rows = extractIntValue(line, "Rows");
//   std::getline(map_file, line);
//   int cols = extractIntValue(line, "Cols");
//   std::vector<std::vector<Tile>> map(
//       rows, std::vector<Tile>(cols, Tile(0, 0, nullptr, nullptr, nullptr)));
//   int row_idx = 0;
//   while (std::getline(map_file, line)) {
//     if (row_idx >= rows) {
//       std::cerr << "[GameManager::load_map] Warning: Map file has more rows "
//                    "than specified. Ignoring extra rows."
//                 << std::endl;
//       break;
//     }

//     for (int i = 0; i < cols; i++) {
//       map[row_idx][i].x = row_idx;
//       map[row_idx][i].y = i;

//       char c = ' ';
//       if (i < static_cast<int>(line.size())) {
//         c = line[i];
//       }

//       switch (c) {
//       case '#': {
//         auto wall = std::make_shared<Wall>(row_idx, i);
//         this->subscribe_entity(wall);
//         map[row_idx][i].ground = wall;
//         break;
//       }
//       case '1':
//       case '2':
//       case '3':
//       case '4':
//       case '5':
//       case '6':
//       case '7':
//       case '8':
//       case '9': {
//         int player_num = c - '0';
//         auto tank = std::make_shared<Tank>(row_idx, i, Direction::U,
//                                            player_num); // TODO: fix
//                                            direction

//         this->subscribe_tank(tank);
//         map[row_idx][i].actor = tank;
//         bool player_exists = false;
//         for (const auto &player_ptr : this->players) {
//           if (player_ptr) {
//             if (const GamePlayer *gp =
//                     dynamic_cast<const GamePlayer *>(player_ptr.get())) {
//               if (gp->get_id() == player_num) {
//                 player_exists = true;
//                 break;
//               }
//             }
//           }
//         }

//         if (!player_exists) {
//           auto new_player = this->player_factory.create(player_num, row_idx,
//           i,
//                                                         max_steps,
//                                                         num_shells);
//           this->players.emplace_back(std::move(new_player));
//         }
//         break;
//       }
//       case ' ':
//         break;
//       default:
//         // TODO: log error for invalid character.
//         break;
//       }
//     }
//     row_idx++;
//   }
//   if (row_idx < rows) {
//     std::cerr << "[GameManager::load_map] Warning: Map file has fewer rows "
//                  "than specified. Remaining rows will be empty."
//               << std::endl;
//     for (int r = row_idx; r < rows; ++r) {
//       for (int c_col = 0; c_col < cols; ++c_col) {
//         map[r][c_col].x = r;
//         map[r][c_col].y = c_col;
//       }
//     }
//   }

//   this->map =
//       std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);
//   return 0;
// }