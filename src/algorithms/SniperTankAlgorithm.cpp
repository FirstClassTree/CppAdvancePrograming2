#include "../../common/algorithms/SniperTankAlgorithm.h"
#include "ActionRequest.h"
#include "TankAlgorithm.h"
#include <iostream>

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
  this->ask_cooldown = 4;
  this->stuck_cooldown = 2;
  this->chooseAction = ActionRequest::GetBattleInfo;
}

void SniperTankAlgorithm::set_target(std::vector<std::vector<char>> &grid)
{

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
            candidate))
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

  std::cout << "no direction found" << std::endl;
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
  if (!this->currentTarget.sync)
  {
    this->waypoint.sync = false;
    return;
  }

  size_t num_rows = grid.size();
  if (num_rows == 0)
  {
    this->waypoint.sync = false;
    return;
  }
  size_t num_cols = grid[0].size();
  if (num_cols == 0)
  {
    this->waypoint.sync = false;
    return;
  }

  std::queue<SniperStep> q;
  q.push({this->currentState.x, this->currentState.y});

  std::vector<std::vector<bool>> visited(num_rows,
                                         std::vector<bool>(num_cols, false));
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
        !is_path_obstructed(grid, current_pos, target_step))
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
    this->ask_cooldown = 4;
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
      this->currentState.cooldown = 4;
      this->currentState.ammo -= 1;
      this->currentState.phase = SCOUT;
      std::cout << "shooting" << std::endl;
      break;
    }
    else
    {
      Direction direction = get_direction_from_step(
          SniperStep{this->currentState.x, this->currentState.y},
          SniperStep{this->currentTarget.x, this->currentTarget.y});
      this->chooseAction =
          rotate_toward(this->currentState.direction, direction);
      std::cout << "rotating to target" << std::endl;
      break;
    }
  }
  case SCOUT:
  {
    std::cout << "in scout" << std::endl;
    if (this->currentState.x == this->waypoint.x &&
        this->currentState.y == this->waypoint.y)
    {
      std::cout << "in scout, waypoint is reached" << std::endl;
      std::cout << "waypoint: " << this->waypoint.x << ", " << this->waypoint.y
                << std::endl;
      std::cout << "state: " << this->currentState.x << ", "
                << this->currentState.y << std::endl;
      if (this->currentState.cooldown == 0)
      {
        std::cout << "in scout, cooldown is 0" << std::endl;
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
  std::cout << "rotate_toward from: " << fi << " to: " << ti << std::endl;
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
    std::cout << "set to scout for alignment *************" << std::endl;
  }
  else
  {
    this->currentState.phase = AIM;
    std::cout << "set to aim" << std::endl;
  }
  if (!this->waypoint.sync)
  {
    find_waypoint(view);
  }
}

SniperTankAlgorithm::~SniperTankAlgorithm() {}