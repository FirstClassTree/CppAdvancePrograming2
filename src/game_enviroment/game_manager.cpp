#include "../../common/GameManager.h"

int extractIntValue(const std::string &line, const std::string &key) {
  std::regex pattern("^\\s*" + key + "\\s*=\\s*(\\d+)\\s*$");
  std::smatch match;
  if (std::regex_match(line, match, pattern)) {
    return std::stoi(match[1]);
  } else {
    throw std::runtime_error("Invalid format for line: " + line);
  }
}

GameManager::GameManager() {}

Map GameManager::get_map() {
  if (!map) {
    // For a test, it might be better to throw or assert
    throw std::runtime_error("GameManager::map is null in get_map()!");
  }
  return *map;
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
  std::vector<std::vector<char>> map(rows, std::vector<char>(cols, ' '));
  int row_idx = 0;
  while (std::getline(map_file, line)) {
    int line_size = static_cast<int>(line.size());
    if (row_idx > cols) {
      // TODO: log error for extra line. this will just pass
    }
    if (line_size < rows) {
      // TODO: log error for line length. this will trim.
      line += std::string(rows - line.size(), ' ');
    } else if (line_size > rows) {
      // TODO: log error for line length. this will trim to spesified length.
      line = line.substr(0, rows);
    }
    for (int i = 0; i < std::min(line_size, rows); i++) {
      char c = line[i];
      switch (c) {
      case '#': {
        // TODO: add wall init.
        map[row_idx][i] = '#';
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
        // Store player position for later initialization
        int player_num = c - '0';
        // player_positions[player_num] = {row_idx, i};
        map[row_idx][i] = c;
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
  while (row_idx < rows) {
    // TODO: log error for missing line.
    ++row_idx;
  }

  this->map =
      std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);
  return 0;
}
