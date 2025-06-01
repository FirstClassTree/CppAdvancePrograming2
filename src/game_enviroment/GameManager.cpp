#include "../../common/GameManager.h"
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

GameManager::GameManager() { this->player_factory = GamePlayerFactory(); }

void GameManager::subscribe_tank(std::shared_ptr<Tank> tank) {
  this->subscribe_entity(tank);
  this->game_tanks.push_back(tank);
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

int GameManager::load_map(const std::string &map_path) {
    std::ifstream map_file = open_map_file(map_path);
    if (!map_file.is_open()) return -1;

    std::string name;
    int max_steps, num_shells, rows, cols;
    if (!parse_map_header(map_file, name, max_steps, num_shells, rows, cols)) return -1;

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
        populate_map_row(line, row_idx, cols, max_steps, num_shells, map,
                         tanks_out, entities_out, player_spawn_points_out);
        ++row_idx;
    }

    // May be unceearry:
    if (row_idx < rows)
        fill_remaining_rows(row_idx, rows, cols, map);

    for (auto &entity : entities_out) subscribe_entity(entity);
    for (auto &tank : tanks_out) subscribe_tank(tank);

    for (const auto &[player_num, pos] : player_spawn_points_out) {
        bool exists = std::any_of(players.begin(), players.end(),
            [player_num](const auto &p) {
                auto *gp = dynamic_cast<const GamePlayer *>(p.get());
                return gp && gp->get_id() == player_num;
            });
        if (!exists) {
            players.emplace_back(
                player_factory.create(player_num, pos.first, pos.second, max_steps, num_shells));
        }
    }

    this->map = std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);
    return 0;
}


std::ifstream GameManager::open_map_file(const std::string &map_path) {
    std::cout << "[open_map_file] Attempting to open map file: \"" << map_path << "\"\n";

    std::ifstream map_file(map_path);
    if (!map_file.is_open()) {
        std::cerr << "[open_map_file] ERROR: Failed to open map file: \"" << map_path << "\"\n";
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
        throw std::runtime_error("Expected key '" + expected_key + "', found '" + key + "'");

    // Extract and clean the value part
    std::string value_str = line.substr(pos + 1);
    value_str.erase(std::remove_if(value_str.begin(), value_str.end(), ::isspace), value_str.end());

    return std::stoi(value_str);
}

bool GameManager::parse_map_header(std::istream &in, std::string &name, int &max_steps,
                      int &num_shells, int &rows, int &cols) {
    std::string line;
    try {
        if (!std::getline(in, name)) throw std::runtime_error("Missing map name line");
        if (!std::getline(in, line)) throw std::runtime_error("Missing MaxSteps line");
        max_steps = extractIntValue(line, "MaxSteps");

        if (!std::getline(in, line)) throw std::runtime_error("Missing NumShells line");
        num_shells = extractIntValue(line, "NumShells");

        if (!std::getline(in, line)) throw std::runtime_error("Missing Rows line");
        rows = extractIntValue(line, "Rows");

        if (!std::getline(in, line)) throw std::runtime_error("Missing Cols line");
        cols = extractIntValue(line, "Cols");

    } catch (const std::exception &e) {
        std::cerr << "[parse_map_header] ERROR: " << e.what() << '\n';
        return false;
    }

    return true;
}


std::vector<std::vector<Tile>> GameManager::create_empty_map(int rows, int cols) {
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
void GameManager::populate_map_row(const std::string &line, int row_idx, int cols,
                      int max_steps, int num_shells,
                      std::vector<std::vector<Tile>> &map,
                      std::vector<std::shared_ptr<Tank>> &tanks_out,
                      std::vector<std::shared_ptr<Entity>> &entities_out,
                      std::vector<std::pair<int, std::pair<int, int>>> &player_spawn_points_out) {
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
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': {
                int player_num = c - '0';
                Direction dir = (player_num == 1 ? Direction::L : Direction::R);
                auto tank = std::make_shared<Tank>(row_idx, col, dir, player_num);
                tile.actor = tank;
                tanks_out.push_back(tank);
                player_spawn_points_out.emplace_back(player_num, std::make_pair(row_idx, col));
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
//                                            player_num); // TODO: fix direction
        
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
//           auto new_player = this->player_factory.create(player_num, row_idx, i,
//                                                         max_steps, num_shells);
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