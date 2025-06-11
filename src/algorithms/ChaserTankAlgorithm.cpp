#include "../../common/algorithms/ChaserTankAlgorithm.h"

#include <algorithm>
#include <iostream>

using namespace std;

ChaserTankAlgorithm::ChaserTankAlgorithm(int player_index, int tank_index,
                                         int tank_x, int tank_y) {
  this->chooseAction = ActionRequest::GetBattleInfo;
  this->player_index = player_index;
  this->tank_index = tank_index;
  this->info_cooldown = 0;
  this->currentState = {tank_x, tank_y,
                        player_index == 1 ? Direction::L : Direction::R, false};
  this->currentTarget = {0, 0, false};
}

ChaserTankAlgorithm::~ChaserTankAlgorithm() {}

void ChaserTankAlgorithm::updateBattleInfo(BattleInfo &battle_info) {
  auto &tank_info = dynamic_cast<TankBattleInfo &>(battle_info);
  auto view = tank_info.get_view();
  locate_me(view);
  set_target(view);
  make_decision(view);
  this->dirty = false;
}

void ChaserTankAlgorithm::locate_me(std::vector<std::vector<char>> &grid) {
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == '%') {
        this->currentState.x = r;
        this->currentState.y = c;
        return;
      }
    }
  }
}

void ChaserTankAlgorithm::make_decision(std::vector<std::vector<char>> &grid) {
  if (!this->currentTarget.sync) {
    this->chooseAction = ActionRequest::DoNothing;
    return;
  }

  auto bfs_grid = make_bfs_grid(grid);

  // Invalidate path if next step is blocked
  if (!m_path.empty()) {
    ChaserStep next_step = m_path.front();

    auto inside = [&](int r, int c) {
      return r >= 0 && r < static_cast<int>(bfs_grid.size()) && c >= 0 &&
             c < static_cast<int>(bfs_grid[0].size());
    };

    // The previously stored path might refer to cells that are no longer
    // inside the current view (e.g. because the view window scrolled).  If so
    // we discard the path and recompute it.
    if (!inside(next_step.x, next_step.y) ||
        bfs_grid[next_step.x][next_step.y]) {
      std::queue<ChaserStep> empty;
      m_path.swap(empty); // Path is blocked or invalid, clear it.
    }
  }

  // If no path, calculate one
  if (m_path.empty()) {
    // Our target should be traversable in bfs grid
    auto inside_bfs = [&](int r, int c) {
      return r >= 0 && r < static_cast<int>(bfs_grid.size()) && c >= 0 &&
             c < static_cast<int>(bfs_grid[0].size());
    };

    if (inside_bfs(this->currentTarget.x, this->currentTarget.y)) {
      bfs_grid[this->currentTarget.x][this->currentTarget.y] = false;
    }
    this->bfs(bfs_grid);
  }

  // If we have a path, decide next action
}

void ChaserTankAlgorithm::take_from_queue() {
  if (!m_path.empty()) {
    ChaserStep next_step = m_path.front();
    ChaserStep current_pos = {this->currentState.x, this->currentState.y};
    Direction direction = get_direction_from_step(next_step, current_pos);
    this->chooseAction = rotate_toward(this->currentState.direction, direction);

    // If we are moving forward, it means we are aligned and will move to the
    // next step. So we consume the step from the path.
    if (this->chooseAction == ActionRequest::MoveForward) {
      m_path.pop();
    }
  } else {
    // No path found or could be calculated
    this->chooseAction = ActionRequest::DoNothing;
  }
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
    if (this->currentTarget.x != best_target_x ||
        this->currentTarget.y != best_target_y || !this->currentTarget.sync) {
      std::queue<ChaserStep> empty;
      this->m_path.swap(empty);
    }
    this->currentTarget.x = best_target_x;
    this->currentTarget.y = best_target_y;
    this->currentTarget.sync = true;
    std::cout << "Found target at: " << best_target_x << ", " << best_target_y
              << std::endl;
  } else {
    std::cout << "No target found" << std::endl;
    // No enemy tanks found in the current view
    if (this->currentTarget.sync) { // if there was a target, clear path
      std::queue<ChaserStep> empty;
      this->m_path.swap(empty);
    }
    this->currentTarget.sync = false;
    // currentTarget.x and .y will retain their previous values,
    // but .sync = false indicates that this target is no longer
    // valid/acquired.
  }
}

// TODO: make shorter.
void ChaserTankAlgorithm::bfs(std::vector<std::vector<bool>> &grid) {
  // Defensive-programming: make sure the grid is non-empty before accessing
  // grid[0].  If the grid is empty we cannot run a BFS anyway, so just return
  // early.
  if (grid.empty() || grid[0].empty()) {
    return;
  }

  const int H = static_cast<int>(grid.size());
  const int W = static_cast<int>(grid[0].size());

  // Helper lambda to test that a coordinate is inside the grid bounds.
  auto inside = [H, W](int r, int c) {
    return r >= 0 && r < H && c >= 0 && c < W;
  };

  // Bail-out if either the current position or the requested target are not
  // inside the current view.  This situation can occasionally happen when the
  // simulated state drifts outside the newly received view or when for some
  // reason we failed to locate our own tank ("%") in the view.
  if (!inside(this->currentState.x, this->currentState.y) ||
      !inside(this->currentTarget.x, this->currentTarget.y)) {
    return;
  }

  // Prioritize straight directions (D, U, R, L) over diagonals
  const int DX[] = {1, -1, 0, 0, 1, 1, -1, -1};
  const int DY[] = {0, 0, 1, -1, 1, -1, 1, -1};

  vector<vector<int>> dist(H, vector<int>(W, -1));
  vector<vector<ChaserStep>> parent(H, vector<ChaserStep>(W, {-1, -1}));
  queue<ChaserStep> q;

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
      if (inside(nx, ny) && dist[nx][ny] == -1 && !grid[nx][ny]) {
        dist[nx][ny] = dist[x][y] + 1;
        parent[nx][ny] = {x, y};
        q.push({nx, ny});
      }
    }
  }

  // Clear any old path before creating a new one.
  std::queue<ChaserStep> empty;
  m_path.swap(empty);

  if (!inside(this->currentTarget.x, this->currentTarget.y) ||
      dist[this->currentTarget.x][this->currentTarget.y] == -1) {
    return; // No path found or target out of bounds
  }

  std::vector<ChaserStep> reversed_path;
  ChaserStep cur{this->currentTarget.x, this->currentTarget.y};
  while (cur.x != this->currentState.x || cur.y != this->currentState.y) {
    reversed_path.push_back(cur);
    cur = parent[cur.x][cur.y];
    if (cur.x == -1 || cur.y == -1) { // Should not happen if path was found
      std::queue<ChaserStep> empty_q;
      empty_q.push({this->currentState.x, this->currentState.y});
      m_path.swap(empty_q); // Error, clear path
      return;
    }
  }
  std::reverse(reversed_path.begin(), reversed_path.end());

  for (const auto &step : reversed_path) {
    m_path.push(step);
  }
}

ActionRequest ChaserTankAlgorithm::rotate_toward(Direction from, Direction to) {
  auto mod8 = [](int x) { return (x % 8 + 8) % 8; };

  int fi = static_cast<int>(from);
  int ti = static_cast<int>(to);
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
    this->currentState.x += DX[dir_idx];
    this->currentState.y += DY[dir_idx];
    state_has_changed = true;

    break;
  }
  case ActionRequest::RotateRight45:
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) + 1) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateRight90:
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) + 2) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateLeft45:
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) - 1 + 8) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::RotateLeft90:
    this->currentState.direction = static_cast<Direction>(
        (static_cast<int>(this->currentState.direction) - 2 + 8) % 8);
    state_has_changed = true;
    break;
  case ActionRequest::Shoot:
    break;
  case ActionRequest::DoNothing:
    break;
  case ActionRequest::GetBattleInfo:

    break;
  default:
    std::cout << "Unknown action simulated" << std::endl;
    break;
  }
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
  if (this->info_cooldown <= 0) {
    this->info_cooldown = 4;
    return ActionRequest::GetBattleInfo;
  }
  if (this->dirty)
    return ActionRequest::GetBattleInfo;
  take_from_queue();
  this->info_cooldown--;
  this->simulate_move();
  return this->chooseAction;
}

Direction ChaserTankAlgorithm::get_direction_from_step(ChaserStep cur,
                                                       ChaserStep prv) {
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