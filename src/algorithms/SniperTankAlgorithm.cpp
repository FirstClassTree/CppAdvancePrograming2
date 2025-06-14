#include "../../common/algorithms/SniperTankAlgorithm.h"
#include "ActionRequest.h"
#include <iostream>
#include "../common/Constants.h"

SniperTankAlgorithm::SniperTankAlgorithm(int player_index, int tank_index,
                                         int tank_x, int tank_y)
{
  this->player_index = player_index;
  this->tank_index = tank_index;
  this->currentState = {
      tank_x, tank_y, player_index == 1 ? Direction::L : Direction::R,
      false, SCOUT, 16,
      0};
  this->currentTarget = {0, 0, false};
  this->waypoint = {0, 0, false};
  this->ask_cooldown = 10;
  this->stuck_cooldown = 6;
  this->chooseAction = ActionRequest::GetBattleInfo;
}

void SniperTankAlgorithm::set_target(std::vector<std::vector<char>> &grid)
{
  std::cout << "setting target" << std::endl;

  if (grid.empty() || grid[0].empty())
  {
    this->currentTarget.sync = false;
    return;
  }
  std::vector<SniperStep> candidates = get_candidates(grid);
  if (candidates.empty())
  {
    this->currentTarget.sync = false;
    return;
  }
  for (auto &candidate : candidates)
  {
    if (!this->is_path_obstructed(
            grid, SniperStep{this->currentState.x, this->currentState.y},
            candidate) || this->stuck_cooldown <= 0)
    {
      this->currentTarget = {candidate.x, candidate.y, true};
      std::cout << "set_target to: " << candidate.x << ", " << candidate.y
                << std::endl;
      return;
    }
  }
  std::cout << "no valid target found" << std::endl;
  this->currentTarget.sync = false;
}
bool SniperTankAlgorithm::is_in_sight()
{
  Direction direction = get_direction_from_step(
      SniperStep{this->currentState.x, this->currentState.y},
      SniperStep{this->currentTarget.x, this->currentTarget.y});
  return direction == this->currentState.direction;
}

Direction SniperTankAlgorithm::get_direction_from_step(SniperStep cur,
                                                       SniperStep prv)
{

  int dx = prv.x - cur.x;
  int dy = prv.y - cur.y;

  // Normalize dx and dy to -1, 0, or 1 to get the direction
  int ndx = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
  int ndy = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

  if (ndx == 0 && ndy == 1)
    return Direction::R;
  if (ndx == 0 && ndy == -1)
    return Direction::L;
  if (ndx == 1 && ndy == 0)
    return Direction::D;
  if (ndx == -1 && ndy == 0)
    return Direction::U;
  if (ndx == 1 && ndy == 1)
    return Direction::RD;
  if (ndx == -1 && ndy == 1)
    return Direction::UR;
  if (ndx == 1 && ndy == -1)
    return Direction::DL;
  if (ndx == -1 && ndy == -1)
    return Direction::UL;
  this->waypoint.sync = false;
  return Direction::U;
}

void SniperTankAlgorithm::move_toward(std::vector<std::vector<char>> &grid) {}

std::vector<SniperStep>
SniperTankAlgorithm::get_candidates(std::vector<std::vector<char>> &grid)
{
  std::vector<SniperStep> candidates;
  size_t num_rows = grid.size();
  size_t num_cols = grid[0].size();
  for (size_t r = 0; r < num_rows; ++r)
  {
    for (size_t c = 0; c < num_cols; ++c)
    {
      char cell_content = grid[r][c];
      // Check if the cell contains a digit character, which we assume
      // represents a tank
      if (std::isdigit(cell_content))
      {
        int potential_player_id = cell_content - '0';
        // Exclude self
        if (potential_player_id != this->player_index)
        {
          candidates.push_back({static_cast<int>(r), static_cast<int>(c)});
        }
      }
    }
  }
  return candidates;
}

bool SniperTankAlgorithm::aligned()
{
  int dx = this->currentTarget.x - this->currentState.x;
  int dy = this->currentTarget.y - this->currentState.y;

  if (dx == 0 && dy == 0)
    return true;
  if (dx == dy && dx < 0)
    return true;
  if (dx == dy && dx > 0)
    return true;
  if (dx == 0 && dy > 0)
    return true;
  if (dx == 0 && dy < 0)
    return true;
  if (dy == 0 && dx > 0)
    return true;
  if (dy == 0 && dx < 0)
    return true;
  if (dx == dy * -1 && dx < 0)
    return true;
  if (dx == dy * -1 && dx > 0)
    return true;

  return false;
}

bool SniperTankAlgorithm::is_path_obstructed(
    std::vector<std::vector<char>> &grid, const SniperStep &from,
    const SniperStep &to)
{
  // Check if there is a wall ('#') or an allied tank between from and to
  int x0 = from.x;
  int y0 = from.y;
  int x1 = to.x;
  int y1 = to.y;

  // Bresenham's line algorithm for grid traversal
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  auto inside = [&](int r, int c) -> bool
  {
    return r >= 0 && r < static_cast<int>(grid.size()) &&
           c >= 0 && c < static_cast<int>(grid[r].size());
  };

  while (true)
  {
    if (!(x0 == from.x && y0 == from.y))
    {
      if (!inside(x0, y0)) // <-- new
        return true;       // wandered off the board ⇒ blocked

      char cell = grid[x0][y0]; // now guaranteed in-bounds
      if (cell == '#')
        return true; // wall
      if (std::isdigit(cell) &&
          (cell - '0') == player_index)
        return true; // allied tank
    }

    if (x0 == x1 && y0 == y1)
      break; // reached target

    e2 = 2 * err;
    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
  return false;
}

void SniperTankAlgorithm::find_waypoint(std::vector<std::vector<char>> &grid)
{
  std::cout << "finding waypoint" << std::endl;
  size_t num_rows = grid.size();
  if (num_rows == 0)
  {
    std::cout << "num_rows is 0" << std::endl;
    this->waypoint.sync = false;
    return;
  }
  size_t num_cols = grid[0].size();
  if (num_cols == 0)
  {
    std::cout << "num_cols is 0" << std::endl;
    this->waypoint.sync = false;
    return;
  }

  std::queue<SniperStep> q;
  q.push({this->currentState.x, this->currentState.y});

  std::vector<std::vector<bool>> visited(num_rows,
                                         std::vector<bool>(num_cols, false));

  std::cout << "visited size: " << visited.size() << std::endl;
  
  if (this->currentState.x >= 0 &&
      this->currentState.x < static_cast<int>(num_rows) &&
      this->currentState.y >= 0 &&
      this->currentState.y < static_cast<int>(num_cols))
  {
    visited[this->currentState.x][this->currentState.y] = true;
  }
  else
  {
    this->waypoint.sync = false;
    return;
  }

  SniperStep target_step = {this->currentTarget.x, this->currentTarget.y};
  std::cout << "exploring" << std::endl;
  while (!q.empty())
  {
    SniperStep current_pos = q.front();
    q.pop();

    int dx_align = this->currentTarget.x - current_pos.x;
    int dy_align = this->currentTarget.y - current_pos.y;
    bool is_aligned_from_pos =
        (dx_align == 0 || dy_align == 0 || abs(dx_align) == abs(dy_align));

    if (is_aligned_from_pos &&
        (!is_path_obstructed(grid, current_pos, target_step) || this->stuck_cooldown <= 0))
    {
      this->waypoint = {current_pos.x, current_pos.y, true};
      std::cout << "found waypoint: " << current_pos.x << ", " << current_pos.y
                << std::endl;
      return;
    }

    // Explore neighbors (up, down, left, right)
    const int dx_move[] = {0, 0, 1, -1};
    const int dy_move[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; ++i)
    {
      int next_x = current_pos.x + dx_move[i];
      int next_y = current_pos.y + dy_move[i];

      if (next_x >= 0 && next_x < static_cast<int>(num_rows) && next_y >= 0 &&
          next_y < static_cast<int>(num_cols) && !visited[next_x][next_y])
      {
        visited[next_x][next_y] = true;
        char cell = grid[next_x][next_y];
        if (cell != '#' && !std::isdigit(cell))
        {
          q.push({next_x, next_y});
        }
      }
    }
  }

  this->waypoint.sync = false;
}

void SniperTankAlgorithm::simulate_move()
{
  // U, UR, R, RD, D, DL, L, UL
  // These static const arrays are assumed to be defined at file scope in
  // ChaserTankAlgorithm.cpp
  const int DX[] = {-1, -1, 0, 1, 1, 1, 0, -1}; // Changes in X (row)
  const int DY[] = {0, 1, 1, 1, 0, -1, -1, -1}; // Changes in Y (col)
  // Their order must match the Direction enum.

  bool state_has_changed = false;

  switch (this->chooseAction)
  {
  case ActionRequest::MoveForward:
  {
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

    break;
  }
}

void SniperTankAlgorithm::locate_me(std::vector<std::vector<char>> &grid)
{
  for (size_t r = 0; r < grid.size(); ++r)
  {
    for (size_t c = 0; c < grid[r].size(); ++c)
    {
      if (grid[r][c] == '%')
      {
        int x = static_cast<int>(r);
        int y = static_cast<int>(c);
        if (this->currentState.x == x && this->currentState.y == y)
        {
          continue;
        }
        else
        {
          this->currentState.x = x;
          this->currentState.y = y;
          this->dirty = true;
        }
        return;
      }
    }
  }
}

ActionRequest SniperTankAlgorithm::getAction()
{
  if (this->dirty)
  {
    this->ask_cooldown = 8;
    this->currentTarget.sync = false;
    this->dirty = false;
    this->waypoint.sync = false;
    return ActionRequest::GetBattleInfo;
    
  }

  switch (this->currentState.phase)
  {

  case AIM:
  {
    if (this->is_in_sight())
    {
      this->chooseAction = ActionRequest::Shoot;
      this->currentState.cooldown = SHOOT_COOLDOWN_STEPS;
      this->currentState.ammo -= 1;
      this->currentState.phase = SCOUT;
      break;
    }
    else
    {
      Direction direction = get_direction_from_step(
          SniperStep{this->currentState.x, this->currentState.y},
          SniperStep{this->currentTarget.x, this->currentTarget.y});
      this->chooseAction =
          rotate_toward(this->currentState.direction, direction);
      break;
    }
  }
  case SCOUT:
  {
    if (this->currentState.x == this->waypoint.x &&
        this->currentState.y == this->waypoint.y)
    {
      if (this->currentState.cooldown == 0)
      {
        this->currentState.phase = AIM;
      }
      this->chooseAction = ActionRequest::DoNothing;
      break;
    }
    if (this->waypoint.sync)
    {
      Direction direction = get_direction_from_step(
          SniperStep{this->currentState.x, this->currentState.y},
          SniperStep{this->waypoint.x, this->waypoint.y});
      this->chooseAction =
          rotate_toward(this->currentState.direction, direction);
      break;
    }
    else
    {
      this->chooseAction = ActionRequest::GetBattleInfo;
      if (this->stuck_cooldown > 0)
      {
        this->stuck_cooldown -= 1;
      }
    }
  }
  }
  if (this->currentState.cooldown > 0)
  {
    this->currentState.cooldown -= 1;
  }
  if (this->ask_cooldown > 0)
  {
    this->ask_cooldown -= 1;
  } else {
    this->dirty = true;
  }

  this->simulate_move();
  return this->chooseAction;
}

ActionRequest SniperTankAlgorithm::rotate_toward(Direction from, Direction to)
{
  auto mod8 = [](int x)
  { return (x % 8 + 8) % 8; };

  int fi = static_cast<int>(from);
  int ti = static_cast<int>(to);
  int diff_pos = mod8(ti - fi);
  int diff_neg = mod8(fi - ti);
  if (diff_pos == 0)
    return ActionRequest::MoveForward;

  if (diff_pos < diff_neg)
  {
    if (diff_pos == 1)
    {
      return ActionRequest::RotateRight45;
    }
    else
    {
      return ActionRequest::RotateRight90;
    }
  }
  else
  {
    if (diff_neg == 1)
    {
      return ActionRequest::RotateLeft45;
    }
    else
    {
      return ActionRequest::RotateLeft90;
    }
  }
}

void SniperTankAlgorithm::updateBattleInfo(BattleInfo &battle_info)
{
  this->dirty = false;
  if (this->stuck_cooldown > 0)
  {
    this->stuck_cooldown -= 1;
  }
  if (this->currentState.ammo == 0)
  {
    this->chooseAction = ActionRequest::DoNothing;
    return;
  }

  // set relevant data.
  auto &tank_info = dynamic_cast<TankBattleInfo &>(battle_info);
  auto view = tank_info.get_view();
  if (!this->currentTarget.sync)
  {
    set_target(view);
  }
  locate_me(view);
  if (!this->aligned())
  {
    this->currentState.phase = SCOUT;
  }
  else
  {
    this->currentState.phase = AIM;
  }
  if (!this->waypoint.sync)
  {
    find_waypoint(view);
  }
}

SniperTankAlgorithm::~SniperTankAlgorithm() {}