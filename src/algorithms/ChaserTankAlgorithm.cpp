#include "../../common/algorithms/ChaserTankAlgorithm.h"

#include <iostream>

using namespace std;

ChaserTankAlgorithm::ChaserTankAlgorithm(int player_index, int tank_index,
                                         int tank_x, int tank_y) {
  this->chooseAction = ActionRequest::GetBattleInfo;
  this->player_index = player_index;
  this->tank_index = tank_index;
  this->currentState = {tank_x, tank_y,
                        player_index == 1 ? Direction::L : Direction::R, false};
  this->currentTarget = {0, 0, false};
}

ChaserTankAlgorithm::~ChaserTankAlgorithm() {}

void ChaserTankAlgorithm::updateBattleInfo(BattleInfo &battle_info) {
  auto &tank_info = dynamic_cast<TankBattleInfo &>(battle_info);
  // now call make_decision after gathering the data from the battle_info.
  auto view = tank_info.get_view();
  set_target(view);
  make_decision(view);
  this->dirty = false;
}

void ChaserTankAlgorithm::make_decision(std::vector<std::vector<char>> &grid) {
  //TODO: Segfault after this line!
  auto bfs_grid = make_bfs_grid(grid);
  if (this->currentTarget.sync) {
    bfs_grid[this->currentTarget.x][this->currentTarget.y] = false;
  }
  this->bfs(bfs_grid);
}

void ChaserTankAlgorithm::set_target(std::vector<std::vector<char>> &grid) {

  if (grid.empty() || grid[0].empty()) {
    this->currentTarget.sync = false;
    return;
  }

  long long min_dist_sq = std::numeric_limits<long long>::max();
  bool found_target = false;
  int best_target_x =
      -1; // Temporary variable for the best target's x coordinate
  int best_target_y =
      -1; // Temporary variable for the best target's y coordinate

  size_t num_rows = grid.size();
  size_t num_cols = grid[0].size();
  // Iterate over the grid to find all tanks
  for (size_t r = 0; r < num_rows; ++r) {
    for (size_t c = 0; c < num_cols; ++c) {
      char cell_content = grid[r][c];
      // std::cout << "Cell content: " << cell_content << std::endl;
      //  Check if the cell contains a digit character, which we assume
      //  represents a tank
      if (std::isdigit(cell_content)) {
        int potential_player_id = cell_content - '0';

        // Check if this tank belongs to an enemy player
        if (potential_player_id != this->player_index) {
          long long dr = static_cast<long long>(r) -
                         this->currentState.x; // Difference in rows
          long long dc = static_cast<long long>(c) -
                         this->currentState.y; // Difference in columns

          long long dist_sq = dr * dr + dc * dc; // Squared Euclidean distance

          if (dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            best_target_x = static_cast<int>(r);
            best_target_y = static_cast<int>(c);
            found_target = true;
          }
        }
      }
    }
  }

  if (found_target) {
    this->currentTarget.x = best_target_x;
    this->currentTarget.y = best_target_y;
    this->currentTarget.sync = true;
    std::cout << "Found target at: " << best_target_x << ", " << best_target_y
              << std::endl;
  } else {
    // No enemy tanks found in the current view
    this->currentTarget.sync = false;
    // currentTarget.x and .y will retain their previous values,
    // but .sync = false indicates that this target is no longer valid/acquired.
  }
}

// TODO: make shorter.
void ChaserTankAlgorithm::bfs(std::vector<std::vector<bool>> &grid) {
  const int H = grid.size(), W = grid[0].size();
  // Prioritize straight directions (D, U, R, L) over diagonals
  const int DX[] = {1, -1, 0, 0, 1, 1, -1, -1};
  const int DY[] = {0, 0, 1, -1, 1, -1, 1, -1};

  vector<vector<int>> dist(H, vector<int>(W, -1));
  vector<vector<Step>> parent(H, vector<Step>(W, {-1, -1}));
  queue<Step> q;

  dist[this->currentState.x][this->currentState.y] = 0;
  q.push({this->currentState.x, this->currentState.y});

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();
    if (x == this->currentTarget.x && y == this->currentTarget.y)
      break; // reached

    for (int k = 0; k < 8; ++k) {
      int nx = x + DX[k];
      int ny = y + DY[k];
      if (nx >= 0 && nx < H && ny >= 0 && ny < W && dist[nx][ny] == -1 &&
          !grid[nx][ny]) {
        dist[nx][ny] = dist[x][y] + 1;
        parent[nx][ny] = {x, y};
        q.push({nx, ny});
      }
    }
  }
  if (dist[this->currentTarget.x][this->currentTarget.y] == -1) {
    this->chooseAction = ActionRequest::DoNothing;
    return;
  }
  Step cur{this->currentTarget.x, this->currentTarget.y};
  Step prv = parent[cur.x][cur.y];
  while (!(prv.x == this->currentState.x && prv.y == this->currentState.y)) {
    cur = prv;
    prv = parent[cur.x][cur.y];
  }
  Direction direction = get_direction_from_step(cur, prv);
  this->chooseAction = rotate_toward(this->currentState.direction, direction);
  this->simulate_move();
}

ActionRequest ChaserTankAlgorithm::rotate_toward(Direction from, Direction to) {
  auto mod8 = [](int x) { return (x % 8 + 8) % 8; };

  int fi = static_cast<int>(from);
  int ti = static_cast<int>(to);
  std::cout << "rotate_toward from: " << fi << " to: " << ti << std::endl;
  int diff_pos = mod8(ti - fi);
  int diff_neg = mod8(fi - ti);
  if (diff_pos == 0)
    return ActionRequest::MoveForward;

  if (diff_pos < diff_neg) {
    if (diff_pos == 1) {
      return ActionRequest::RotateRight45;
    } else {
      return ActionRequest::RotateRight90;
    }
  } else {
    if (diff_neg == 1) {
      return ActionRequest::RotateLeft45;
    } else {
      return ActionRequest::RotateLeft90;
    }
  }
}

void ChaserTankAlgorithm::simulate_move() {
  // U, UR, R, RD, D, DL, L, UL
  // These static const arrays are assumed to be defined at file scope in
  // ChaserTankAlgorithm.cpp
  const int DX[] = {-1, -1, 0, 1, 1, 1, 0, -1}; // Changes in X (row)
  const int DY[] = {0, 1, 1, 1, 0, -1, -1, -1}; // Changes in Y (col)
  // Their order must match the Direction enum.

  bool state_has_changed = false;

  switch (this->chooseAction) {
  case ActionRequest::MoveForward: {
    int dir_idx = static_cast<int>(this->currentState.direction);
    // Assuming DX and DY are accessible here (e.g., file-scope static const)
    // and their order matches the Direction enum.

    std::cout << "MoveForward from: " << this->currentState.x << ", "
              << this->currentState.y << std::endl;

    this->currentState.x += DX[dir_idx];
    this->currentState.y += DY[dir_idx];
    // Note: No boundary checks (e.g., map wrapping) are performed here.
    // This simulation updates the tank's belief of its state.
    // The actual state will be synchronized by the next call to
    // updateBattleInfo.
    state_has_changed = true;
    std::cout << "MoveForward to: " << this->currentState.x << ", "
              << this->currentState.y << std::endl;
    break;
  }
  case ActionRequest::RotateRight45:
    std::cout << "RotateRight45 simulated" << std::endl;
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) + 1) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateRight90:
    std::cout << "RotateRight90 simulated" << std::endl;
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) + 2) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateLeft45:
    std::cout << "RotateLeft45 simulated" << std::endl;
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) - 1 + 8) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateLeft90:
    std::cout << "RotateLeft90 simulated" << std::endl;
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) - 2 + 8) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::Shoot:
    std::cout << "Shoot simulated" << std::endl;
    // Shooting does not change position or direction.
    // If ammo were part of State, it might be updated here.
    break;
  case ActionRequest::DoNothing:
    std::cout << "DoNothing simulated" << std::endl;
    // No change to state.
    break;
  case ActionRequest::GetBattleInfo:
    std::cout << "GetBattleInfo simulated" << std::endl;
    // This is a request for information, not a tank action that changes its
    // state. The actual state update will happen in updateBattleInfo.
    break;
  default:
    std::cout << "Unknown action simulated" << std::endl;
    // Unknown action, do nothing. Or, consider logging an error.
    break;
  }

  // if (state_has_changed) {
  //   this->currentState.sync = false;
  // }
}

std::vector<std::vector<bool>>
ChaserTankAlgorithm::make_bfs_grid(std::vector<std::vector<char>> &grid) {
  std::vector<std::vector<bool>> bfs_grid(
      grid.size(), std::vector<bool>(grid[0].size(), false));
  for (size_t i = 0; i < grid.size(); i++) {
    for (size_t j = 0; j < grid[i].size(); j++) {
      bfs_grid[i][j] = grid[i][j] != ' ';
    }
  }
  return bfs_grid;
}
ActionRequest ChaserTankAlgorithm::getAction() {
  if (this->dirty)
    return ActionRequest::GetBattleInfo;
  return this->chooseAction;
}

Direction ChaserTankAlgorithm::get_direction_from_step(Step cur, Step prv) {
  std::cout << "get_direction_from_step cur: " << cur.x << ", " << cur.y
            << " prv: " << prv.x << ", " << prv.y << std::endl;
  if (cur.x == prv.x && cur.y == prv.y + 1)
    return Direction::R;
  if (cur.x == prv.x && cur.y == prv.y - 1)
    return Direction::L;
  if (cur.x == prv.x + 1 && cur.y == prv.y)
    return Direction::D;
  if (cur.x == prv.x - 1 && cur.y == prv.y)
    return Direction::U;
  if (cur.x == prv.x + 1 && cur.y == prv.y + 1)
    return Direction::RD;
  if (cur.x == prv.x - 1 && cur.y == prv.y + 1)
    return Direction::UR;
  if (cur.x == prv.x + 1 && cur.y == prv.y - 1)
    return Direction::DL;
  if (cur.x == prv.x - 1 && cur.y == prv.y - 1)
    return Direction::UL;
  return Direction::U; // TODO: throw error here?
}