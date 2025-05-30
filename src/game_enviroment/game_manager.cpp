#include "../../common/GameManager.h"
#include "entities/Wall.h"
#include "utils/Tile.h"

int extractIntValue(const std::string &line, const std::string &key) {
  std::regex pattern("^\\s*" + key + "\\s*=\\s*(\\d+)\\s*$");
  std::smatch match;
  if (std::regex_match(line, match, pattern)) {
    return std::stoi(match[1]);
  } else {
    throw std::runtime_error("Invalid format for line: " + line);
  }
}

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
  std::cout << "[GameManager::load_map] Attempting to load map from: \""
            << map_path << "\"" << std::endl; // DEBUG
  std::ifstream map_file(map_path);
  if (!map_file.is_open()) {
    std::cerr << "[GameManager::load_map] ERROR: Failed to open map file: \""
              << map_path << "\"" << std::endl; // DEBUG
    return -1;
  }
  std::cout << "[GameManager::load_map] Successfully opened map file: \""
            << map_path << "\"" << std::endl; // DEBUG
  std::string line;

  std::getline(map_file, line);
  std::string name = line;
  std::getline(map_file, line);
  int max_steps = extractIntValue(line, "MaxSteps");
  std::getline(map_file, line);
  int num_shells = extractIntValue(line, "NumShells");
  std::getline(map_file, line);
  int rows = extractIntValue(line, "Rows");
  std::getline(map_file, line);
  int cols = extractIntValue(line, "Cols");
  std::vector<std::vector<Tile>> map(
      rows, std::vector<Tile>(cols, Tile(0, 0, nullptr, nullptr, nullptr)));
  int row_idx = 0;
  while (std::getline(map_file, line)) {
    if (row_idx >= rows) {
      std::cerr << "[GameManager::load_map] Warning: Map file has more rows "
                   "than specified. Ignoring extra rows."
                << std::endl;
      break;
    }

    for (int i = 0; i < cols; i++) {
      map[row_idx][i].x = row_idx;
      map[row_idx][i].y = i;

      char c = ' ';
      if (i < static_cast<int>(line.size())) {
        c = line[i];
      }

      switch (c) {
      case '#': {
        auto wall = std::make_shared<Wall>(row_idx, i);
        this->subscribe_entity(wall);
        map[row_idx][i].ground = wall;
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
        auto tank = std::make_shared<Tank>(row_idx, i, Direction::U,
                                           player_num); // TODO: fix direction
        
        this->subscribe_tank(tank);
        map[row_idx][i].actor = tank;
        bool player_exists = false;
        for (const auto &player_ptr : this->players) {
          if (player_ptr) {
            if (const GamePlayer *gp =
                    dynamic_cast<const GamePlayer *>(player_ptr.get())) {
              if (gp->get_id() == player_num) {
                player_exists = true;
                break;
              }
            }
          }
        }

        if (!player_exists) {
          auto new_player = this->player_factory.create(player_num, row_idx, i,
                                                        max_steps, num_shells);
          this->players.emplace_back(std::move(new_player));
        }
        break;
      }
      case ' ':
        break;
      default:
        // TODO: log error for invalid character.
        break;
      }
    }
    row_idx++;
  }
  if (row_idx < rows) {
    std::cerr << "[GameManager::load_map] Warning: Map file has fewer rows "
                 "than specified. Remaining rows will be empty."
              << std::endl;
    for (int r = row_idx; r < rows; ++r) {
      for (int c_col = 0; c_col < cols; ++c_col) {
        map[r][c_col].x = r;
        map[r][c_col].y = c_col;
      }
    }
  }

  this->map =
      std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);
  return 0;
}



// int GameManager::load_map(const std::string& map_path) {
//   std::ifstream map_file;
//   if (!open_map_file(map_path, map_file)) return -1;

//   std::string name;
//   int max_steps, num_shells, rows, cols;
//   if (!parse_metadata(map_file, name, max_steps, num_shells, rows, cols)) return -1;

//   auto map = init_empty_map(rows, cols);

//   std::string line;
//   int row_idx = 0;
//   while (std::getline(map_file, line) && row_idx < rows) {
//     parse_map_row(line, row_idx, cols, map);
//     ++row_idx;
//   }

//   if (row_idx < rows) fill_remaining_rows(row_idx, rows, cols, map);

//   this->map = std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);
//   return 0;
// }


// bool open_map_file(const std::string& path ,std::ifstream& file){


// }


// bool parse_metadata(std::ifstream& file, std::string& name, int& max_steps, int& num_shells, int& rows, int& cols){


// }

// std::vector<std::vector<Tile>> init_empty_map(int rows, int cols){

// }

// void parse_map_row(const std::string& line, int row_idx, int cols, std::vector<std::vector<Tile>>& map){

// }

// void create_or_update_player(int player_num, int row, int col){


// }

// void GameManager::fill_remaining_rows(int start_row, int total_rows, int total_cols,
//                                       std::vector<std::vector<Tile>>& map) {
//   std::cerr << "[GameManager::load_map] Warning: Map file has fewer rows "
//                "than specified. Remaining rows will be empty."
//             << std::endl;

//   for (int r = start_row; r < total_rows; ++r) {
//     for (int c = 0; c < total_cols; ++c) {
//       map[r][c].x = r;
//       map[r][c].y = c;
//     }
//   }
// }