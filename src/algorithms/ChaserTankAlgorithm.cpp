#include "../../common/algorithms/ChaserTankAlgorithm.h"
#include "ActionRequest.h"

using namespace std;

ChaserTankAlgorithm::ChaserTankAlgorithm() {
  this->chooseAction = ActionRequest::DoNothing;
}

ChaserTankAlgorithm::~ChaserTankAlgorithm() {}

void ChaserTankAlgorithm::updateBattleInfo(BattleInfo &battle_info) {
  auto &tank_info = dynamic_cast<TankBattleInfo &>(battle_info);
}

void ChaserTankAlgorithm::make_decision(std::vector<std::vector<char>> &grid,
                                        int startX, int startY, int targetX,
                                        int targetY, Direction tankDirection) {
  auto bfs_grid = make_bfs_grid(grid);
  this->bfs(bfs_grid, startX, startY, targetX, targetY, tankDirection);
}

// TODO: make shorter.
void ChaserTankAlgorithm::bfs(std::vector<std::vector<bool>> &grid, int startX,
                              int startY, int targetX, int targetY,
                              Direction tankDirection) {
  const int H = grid.size(), W = grid[0].size();
  const int DX[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  const int DY[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

  vector<vector<int>> dist(H, vector<int>(W, -1));
  vector<vector<Step>> parent(H, vector<Step>(W, {-1, -1}));
  queue<Step> q;

  dist[startX][startY] = 0;
  q.push({startX, startY});

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();
    if (x == targetX && y == targetY)
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
  if (dist[targetX][targetY] == -1) {
    this->chooseAction = ActionRequest::DoNothing;
    return;
  }
  Step cur{targetX, targetY};
  Step prv = parent[cur.x][cur.y];
  while (!(prv.x == startX && prv.y == startY)) {
    cur = prv;
    prv = parent[cur.x][cur.y];
  }
  Direction direction = get_direction_from_step(cur, prv);
  this->chooseAction = rotate_toward(tankDirection, direction);
}

ActionRequest ChaserTankAlgorithm::rotate_toward(Direction from, Direction to) {
  auto mod8 = [](int x) { return (x % 8 + 8) % 8; };

  int fi = static_cast<int>(from);
  int ti = static_cast<int>(to);

  int diff_pos = mod8(ti - fi);
  int diff_neg = mod8(fi - ti);
  if (diff_pos == 0)
    return ActionRequest::DoNothing;

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
ActionRequest ChaserTankAlgorithm::getAction() { return this->chooseAction; }

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