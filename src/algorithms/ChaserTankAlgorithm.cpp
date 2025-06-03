#include "../../common/algorithms/ChaserTankAlgorithm.h"
#include "ActionRequest.h"

using namespace std;

ChaserTankAlgorithm::ChaserTankAlgorithm(int player_index, int tank_index,int tank_x,int tank_y) {
  this->chooseAction = ActionRequest::GetBattleInfo;
  this->player_index = player_index;
  this->tank_index = tank_index;
  this->currentState = {tank_x,tank_y,Direction::U,false};
  this->currentTarget = {0,0,false};
}

ChaserTankAlgorithm::~ChaserTankAlgorithm() {}

void ChaserTankAlgorithm::updateBattleInfo(BattleInfo &battle_info) {
  auto &tank_info = dynamic_cast<TankBattleInfo &>(battle_info);
  //now call make_decision after gathering the data from the battle_info.

  this->dirty = false;
}

void ChaserTankAlgorithm::make_decision(std::vector<std::vector<char>> &grid) {
  auto bfs_grid = make_bfs_grid(grid);
  this->bfs(bfs_grid);

}

// TODO: make shorter.
void ChaserTankAlgorithm::bfs(std::vector<std::vector<bool>> &grid) {
  const int H = grid.size(), W = grid[0].size();
  const int DX[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  const int DY[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

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

    for (int k = 0; k < 8;
         ++k) { // TODO: make shorter, possibly cut this for loop into function.
      int nx = (x + DX[k] + H) % H;
      int ny = (y + DY[k] + W) % W;
      if (dist[nx][ny] == -1 && !grid[nx][ny]) {
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
  // These static const arrays are assumed to be defined at file scope in ChaserTankAlgorithm.cpp
  const int DX[] = {-1, -1, 0, 1, 1, 1, 0, -1}; // Changes in X (row)
  const int DY[] = {0, 1, 1, 1, 0, -1, -1, -1}; // Changes in Y (col)
  // Their order must match the Direction enum.

  bool state_has_changed = false;

  switch (this->chooseAction) {
  case ActionRequest::MoveForward: {
    int dir_idx = static_cast<int>(this->currentState.direction);
    // Assuming DX and DY are accessible here (e.g., file-scope static const)
    // and their order matches the Direction enum.
    this->currentState.x += DX[dir_idx];
    this->currentState.y += DY[dir_idx];
    // Note: No boundary checks (e.g., map wrapping) are performed here.
    // This simulation updates the tank's belief of its state.
    // The actual state will be synchronized by the next call to updateBattleInfo.
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
    // Shooting does not change position or direction.
    // If ammo were part of State, it might be updated here.
    break;
  case ActionRequest::DoNothing:
    // No change to state.
    break;
  case ActionRequest::GetBattleInfo:
    // This is a request for information, not a tank action that changes its state.
    // The actual state update will happen in updateBattleInfo.
    break;
  default:
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
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
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
    return Direction::DL;
  if (cur.x == prv.x + 1 && cur.y == prv.y - 1)
    return Direction::UR;
  if (cur.x == prv.x - 1 && cur.y == prv.y - 1)
    return Direction::UL;
  return Direction::U; // TODO: throw error here?
}