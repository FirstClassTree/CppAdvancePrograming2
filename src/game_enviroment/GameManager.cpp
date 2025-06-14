#include "../../common/GameManager.h"
#include "../common/Constants.h"
#include "../common/OutputPrinter.h"
#include "../common/TankAlgorithm.h"
#include "MySatelliteView.h"
#include "entities/Wall.h"
#include "utils/Tile.h"
#include <unordered_map>
#include "../common/Logger.h"
#include "../common/DirectionUtility.h"

GameManager::GameManager()
{
  this->player_factory = GamePlayerFactory();
  this->tank_algorithm_factory = MyTankAlgorithmFactory();
}

GameManager::GameManager(GamePlayerFactory player_factory,
                         MyTankAlgorithmFactory tank_algorithm_factory,
                         bool enable_visual)
    : enable_visualizer(enable_visual),
      player_factory(std::move(player_factory)),
      tank_algorithm_factory(std::move(tank_algorithm_factory)) {}

void GameManager::subscribe_tank(std::shared_ptr<Tank> tank)
{
  this->subscribe_entity(tank);
  this->game_tanks.push_back(tank);
}

const std::vector<std::unique_ptr<Player>> &GameManager::get_players() const
{
  return players;
}

const std::shared_ptr<Tank> GameManager::get_tank(int tankIndex,
                                                  int playerIndex) const
{
  for (const auto &tank : game_tanks)
  {
    if (tank->get_tank_id() == tankIndex &&
        tank->get_owner_id() == playerIndex)
    {
      return tank;
    }
  }
  return nullptr;
}

void GameManager::subscribe_entity(std::shared_ptr<Entity> entity)
{
  this->game_entities.push_back(entity);
}
void GameManager::subscribe_shell(std::shared_ptr<Shell> shell)
{
  this->subscribe_entity(shell);
  this->game_shells.push_back(shell);
}

Map GameManager::get_map()
{

  if (!this->map)
  {
    // For a test, it might be better to throw or assert
    throw std::runtime_error("GameManager::map is null in get_map()!");
  }
  return *this->map;
}

std::vector<std::shared_ptr<Tank>> GameManager::get_tanks()
{
  return this->game_tanks;
}

int GameManager::load_map(const std::string &map_path)
{
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
  while (std::getline(map_file, line))
  {
    if (row_idx >= rows)
    {
      input_errors.emplace_back("Extra rows found in map file. Ignored rows starting from line " + std::to_string(row_idx + 1));
      break;
    }
    populate_map_row(line, row_idx, cols, max_steps, num_shells, map, tanks_out,
                     entities_out, player_spawn_points_out,
                     tank_algorithm_factory);
    ++row_idx;
  }

  if (row_idx < rows)
  {
    input_errors.emplace_back("Missing " + std::to_string(rows - row_idx) +
                              " rows in map. Filled with empty lines.");
    fill_remaining_rows(row_idx, rows, cols, map);
  }

  for (auto &entity : entities_out)
    subscribe_entity(entity);
  for (auto &tank : tanks_out)
    subscribe_tank(tank);

  for (const auto &[player_num, pos] : player_spawn_points_out)
  {
    bool exists = std::any_of(
        players.begin(), players.end(), [player_num](const auto &p)
        {
          auto *gp = dynamic_cast<const GamePlayer *>(p.get());
          return gp && gp->get_id() == player_num; });
    if (!exists)
    {
      players.emplace_back(
          player_factory.create(player_num, rows, cols, max_steps, num_shells));
    }
  }

  this->map =
      std::make_unique<Map>(name, max_steps, num_shells, cols, rows, map);

  this->post_load_process();
  if (!input_errors.empty())
  {
    std::ofstream error_file("input_errors.txt");
    for (const auto &err : input_errors)
    {
      error_file << err << '\n';
    }
  }

  return 0;
}

void GameManager::post_load_process()
{
  int tank_ids[10] = {0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0}; // Assumes player IDs 0-9 can have tanks

  for (auto &tank_sp : game_tanks)
  { // Changed variable name tank to tank_sp
    // for clarity (shared_ptr)
    if (!tank_sp)
    {
      continue;
    }

    tank_algorithm_factory.tank_x = tank_sp->get_x();
    tank_algorithm_factory.tank_y = tank_sp->get_y();

    int owner_id = tank_sp->get_owner_id();

    // tank_sp->set_direction(owner_id == 1 ? Direction::L : Direction::R);

    // Basic bounds check for tank_ids array access. Player IDs from maps are
    // usually 1-9. If owner_id is outside 0-9, this logic needs more robust
    // error handling or clarification on how player IDs are managed vs. array
    // indexing.
    if (owner_id < 0 ||
        owner_id >= 10)
    { // Check against 0-9 for tank_ids array
      std::cerr
          << "Error: Tank (owner_id=" << owner_id
          << ") has an ID out of expected range [0-9] for tank_ids array in "
             "post_load_process. Skipping tank association with player.\n";
      // Still proceed to set AI and tank_id if possible, but skip player
      // association. Depending on requirements, might want to skip this tank
      // entirely. For now, we will attempt to set AI and ID if owner_id is
      // valid for create() and set_tank_id(). The create factory method might
      // have its own validation for player_index.
    }
    else
    {
      // Only increment tank_ids[owner_id] if owner_id is a valid index.
      int current_tank_id_for_owner = tank_ids[owner_id];
      tank_sp->set_ai(
          tank_algorithm_factory.create(owner_id, current_tank_id_for_owner));
      tank_sp->set_tank_id(current_tank_id_for_owner);
      tank_ids[owner_id]++;
    }

    GamePlayer *target_player = nullptr;
    for (const auto &player_up :
         this->players)
    { // player_up is std::unique_ptr<Player>
      if (player_up)
      { // Check unique_ptr is not null
        GamePlayer *gp = dynamic_cast<GamePlayer *>(player_up.get());
        if (gp && gp->get_id() == owner_id)
        {
          target_player = gp;
          break;
        }
      }
    }

    if (target_player)
    {
      target_player->tanks.push_back(
          tank_sp); // GamePlayer::tanks is std::vector<std::weak_ptr<Tank>>
    }
    else
    {
      std::cerr << "Error: Player with ID " << owner_id
                << " not found for tank in post_load_process.\n";
      // This may or may not be a critical error depending on game rules.
    }
  }
}

void GameManager::run()
{
  if (!map)
  {
    std::cerr << "GameManager::run - Map not loaded. Aborting." << std::endl;
    return;
  }
  const int max_steps = map->get_max_steps();
  //  Getting the num of players out of size:
  const int num_players = static_cast<int>(players.size());
  OutputPrinter printer(game_tanks.size());

  int steps_without_shells = 0;
  GameEndStatus status;

  Logger &logger = Logger::getInstance();

  // Allowing end before round 0
  status = check_end_conditions(0, steps_without_shells);
  if (status.finished)
  {
    goto end;
  }
  if (enable_visualizer)
    {
      visualizer.add_snapshot(create_satellite_view(-1, -1));
    }

  for (int step = 0; step < max_steps; ++step)
  {
    logger.setRound(step);

    // Phase 1: Collect and apply tank actions, and resolve mine collisons
    auto tank_actions = collect_tank_actions();
    apply_tank_actions(tank_actions, printer);

    // Phase 2: Move shells and resolve their collisons, walls, shell to shell,
    move_shells_stepwise(printer);

    // Optional visualization
    if (enable_visualizer)
    {
      visualizer.add_snapshot(create_satellite_view(-1, -1));
    }

    // // Phase 4: Finalize round output
    printer.finalizeRound();

    // Phase 5: Check termination conditions
    status = check_end_conditions(step, steps_without_shells);
    if (status.finished)
    {
      break;
    }
    // Handle tie due to max steps
    if (step == max_steps - 1)
    {
      status.finished = true;
      status.tie_due_to_steps = true;
    }
  }
end:
  // Phase 6: Final result
  std::vector<int> tanks_per_player(num_players, 0);
  for (const auto &tank : game_tanks)
  {
    if (tank && tank->get_health() > 0)
    {
      int owner = tank->get_owner_id();
      if (owner >= 1 && owner <= num_players)
      {
        tanks_per_player[owner - 1]++;
      }
    }
  }
  int winner = determine_winner(tanks_per_player);

  printer.logResult(tanks_per_player, winner, status.tie_due_to_steps,
                    status.tie_due_to_shells, max_steps);
  printer.writeToFile("output.txt");

  logger.flush();
  if (enable_visualizer)
  {
    visualizer.run();
  }
}

// Phase 2:
void GameManager::move_shells_stepwise(OutputPrinter& printer)
{
  int rows = map->get_rows();
  int cols = map->get_cols();

  for (int step = 0; step < SHELL_SPEED; ++step)
  {
    // Track positions after this sub-step for shell to shell collisons
    std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>>
        position_map;

    // First move all active shells one sub-step
    for (auto &shell : game_shells)
    {
      if (!shell || shell->is_destroyed())
        continue;

      // Tank colliion before
      auto &current_tile = map->get_tile(shell->get_x(), shell->get_y());
      auto tank_here = current_tile.actor.lock();
      if (tank_here && tank_here->get_health() > 0)
      {
        tank_here->damage();
        if (tank_here->get_health() == 0)
        {
          printer.markTankKilled(tank_here->get_all_tank_index());
          current_tile.actor.reset();
          
        }
        shell->destroy();
        continue;
      }

      auto [dx, dy] = get_direction_offset(shell->get_direction());
      int next_x = (shell->get_x() + dx + rows) % rows;
      int next_y = (shell->get_y() + dy + cols) % cols;

      auto &tile = map->get_tile(next_x, next_y);

      // Wall collision
      auto ground = tile.ground.lock();
      if (ground && ground->get_type() == EntityType::WALL)
      {
        auto wall = std::dynamic_pointer_cast<Wall>(ground);
        if (wall)
        {
          wall->weaken();
          if (wall->is_destroyed())
          {
            tile.ground.reset();
            // Remove wall from game_entities
            game_entities.erase(std::remove_if(game_entities.begin(), game_entities.end(),
                                               [wall](const std::shared_ptr<Entity> &e)
                                               {
                                                 return e == wall;
                                               }),
                                game_entities.end());
          }
        }
        shell->destroy();
        continue;
      }

      // Tank collision
      auto tank = tile.actor.lock();
      if (tank && tank->get_health() > 0)
      {
        tank->damage();

        if (tank->get_health() == 0)
        {
          auto &actual_tile = map->get_tile(tank->get_x(), tank->get_y());
          auto &tile = map->get_tile(tank->get_x(), tank->get_y());
          printer.markTankKilled(tank->get_all_tank_index());
          tile.actor.reset();
        }

        shell->destroy();
        continue;
      }

      auto &prev_tile = map->get_tile(shell->get_x(), shell->get_y());
      if (prev_tile.shell.lock() == shell)
      {
        prev_tile.shell.reset();
      }

      shell->set_pos(next_x, next_y);
      position_map[{next_x, next_y}].push_back(shell);
    }
    // Detect and destroy shells that collide
    for (auto &[pos, shells] : position_map)
    {
      auto &tile = map->get_tile(pos.first, pos.second);
      if (shells.size() > 1)
      {
        for (auto &shell : shells)
        {
          shell->destroy();
          if (tile.shell.lock() == shell)
          {
            tile.shell.reset();
          }
        }
      }
      else
      {
        // Safe: only one shell here
        tile.shell = shells[0];
      }
    }
  }
}

bool try_move_tank(std::shared_ptr<Tank> tank, Map *map, int dx, int dy)
{
  int old_x = tank->get_x();
  int old_y = tank->get_y();

  int new_x = (old_x + dx + map->get_rows()) % map->get_rows();
  int new_y = (old_y + dy + map->get_cols()) % map->get_cols();

  auto &dest_tile = map->get_tile(new_x, new_y);
  auto ground = dest_tile.ground.lock();
  auto other_tank = dest_tile.actor.lock();

  if (!ground || ground->get_type() != EntityType::WALL)
  {
    // Clear old position
    auto &old_tile = map->get_tile(old_x, old_y);

    old_tile.actor.reset();

    // Update tank position and assign to new tile
    tank->set_pos(new_x, new_y);
    dest_tile.actor = tank;

    return true;
  }

  return false;
}

// Phase 1
std::vector<std::pair<std::shared_ptr<Tank>, ActionRequest>>
GameManager::collect_tank_actions()
{
  std::vector<std::pair<std::shared_ptr<Tank>, ActionRequest>> actions;

  for (const auto &tank : game_tanks)
  {
    if (!tank || tank->get_health() == 0)
      continue;

    auto &algo = tank->get_ai();

    ActionRequest action = algo.getAction();

    // Handle GetBattleInfo immediately
    if (action == ActionRequest::GetBattleInfo)
    {
      for (const auto &player_up : players)
      {
        if (!player_up)
          continue;
        GamePlayer *player = dynamic_cast<GamePlayer *>(player_up.get());
        if (player && player->get_id() == tank->get_owner_id())
        {
          auto satellite_view =
              create_satellite_view(tank->get_owner_id(), tank->get_tank_id());
          player->updateTankWithBattleInfo(algo, *satellite_view);
          break;
        }
      }
      action = algo.getAction(); // get action again after update
    }
    actions.emplace_back(tank, action);
  }

  return actions;
}

struct TankMove
{
  std::shared_ptr<Tank> tank;
  std::pair<int, int> start;
  std::pair<int, int> end;
};

// Phase 1.b
// This function is naturlly very long, would cause major problems to split further, most other functions were splitted to be under 30-40 lines long.
void GameManager::apply_tank_actions(
    const std::vector<std::pair<std::shared_ptr<Tank>, ActionRequest>> &actions,
    OutputPrinter &printer)
{
  Logger &logger = Logger::getInstance();

  std::vector<TankMove> tank_moves;
  std::map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>> end_positions;
  for (size_t i = 0; i < actions.size(); ++i)
  {
    const auto &[tank, action] = actions[i];

      if (!tank || tank->get_health() == 0) {
      printer.markTankKilled(tank->get_all_tank_index());  // already dead or null, mark it
      continue;                   // skip setting action
        }
    printer.setTankAction(i, action);  // only set if tank is alive
   
    logger.logTankAction(*tank, actionRequestToString(action));

    bool action_applied = false;
    bool in_backward_move_sequence = false;
    if (tank->get_backward_state() == BackwardState::Waiting1 ||
        tank->get_backward_state() == BackwardState::Waiting2)
    {
      in_backward_move_sequence = true;
    }
    switch (action)
    {
    case ActionRequest::MoveForward:
    {
      if (in_backward_move_sequence ||
          tank->get_backward_state() == BackwardState::ReadyFast)
      {
        tank->cancel_backward_sequence();
        break;
      }
      std::pair<int, int> start_pos = {tank->get_x(), tank->get_y()};
      auto [dx, dy] = get_direction_offset(tank->get_direction());
      action_applied = try_move_tank(tank, map.get(), dx, dy);
      if (action_applied)
      {
        std::pair<int, int> end_pos = {tank->get_x(), tank->get_y()};
        tank_moves.push_back({tank, start_pos, end_pos});
        end_positions[end_pos].push_back(tank);

        // Update actor pointer on new tile
        map->get_tile(end_pos.first, end_pos.second).actor = tank;

        //  Optionally clear actor from old tile
        auto &old_tile = map->get_tile(start_pos.first, start_pos.second);

        old_tile.actor.reset();
      }
      break;
    }

    case ActionRequest::MoveBackward:
    {
      auto state = tank->get_backward_state();
      if (state == BackwardState::None)
      {
        tank->start_backward_sequence();
      }
      else if (state == BackwardState::ReadyFast)
      {

        std::pair<int, int> start_pos = {tank->get_x(), tank->get_y()};
        auto [dx, dy] = get_direction_offset(tank->get_direction());
        action_applied = try_move_tank(tank, map.get(), -dx, -dy);

        if (action_applied)
        {
          std::pair<int, int> end_pos = {tank->get_x(), tank->get_y()};
          tank_moves.push_back({tank, start_pos, end_pos});
          end_positions[end_pos].push_back(tank);

          // Update actor pointer on new tile
          map->get_tile(end_pos.first, end_pos.second).actor = tank;

          // Optionally clear actor from old tile
          auto &old_tile = map->get_tile(start_pos.first, start_pos.second);

          old_tile.actor.reset();
        }
      }
      break;
    }

    case ActionRequest::RotateLeft90:
    {
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      Direction new_dir = static_cast<Direction>(
          (static_cast<int>(tank->get_direction()) - 2 + 8) % 8);
      tank->set_direction(new_dir);
      action_applied = true;
      break;
    }

    case ActionRequest::RotateRight90:
    {
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      Direction new_dir = static_cast<Direction>(
          (static_cast<int>(tank->get_direction()) + 2 + 8) % 8);
      tank->set_direction(new_dir);
      action_applied = true;
      break;
    }

    case ActionRequest::RotateLeft45:
    {
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      Direction new_dir = static_cast<Direction>(
          (static_cast<int>(tank->get_direction()) - 1 + 8) % 8);
      tank->set_direction(new_dir);
      action_applied = true;
      break;
    }

    case ActionRequest::RotateRight45:
    {
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      Direction new_dir = static_cast<Direction>(
          (static_cast<int>(tank->get_direction()) + 1 + 8) % 8);
      tank->set_direction(new_dir);
      action_applied = true;
      break;
    }
    case ActionRequest::Shoot:
    {
      if (in_backward_move_sequence)
      {
        break;
      }

      tank->cancel_backward_sequence();
      // Not allowed to shoot — cooldown or no ammo
      if (!tank->can_shoot())
      {
        break;
      }
      // Calculate shell spawn position
      auto [dx, dy] = get_direction_offset(tank->get_direction());
      int shell_x = (tank->get_x() + dx + map->get_rows()) % map->get_rows();
      int shell_y = (tank->get_y() + dy + map->get_cols()) % map->get_cols();
      // Create and register shell
      auto shell =
          std::make_shared<Shell>(shell_x, shell_y, tank->get_direction());
      subscribe_shell(shell); // add to game_shells and game_entities
      map->get_tile(shell_x, shell_y).shell = shell;

      tank->mark_shot(); // reduce shell count, begin cooldown
      action_applied = true;
      break;
    }

    case ActionRequest::DoNothing:
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      action_applied = true;
      break;

    case ActionRequest::GetBattleInfo:
      if (in_backward_move_sequence)
        break;
      tank->cancel_backward_sequence();
      std::cerr << "[apply_tank_actions] WARNING: Reached "
                   "ActionRequest::GetBattleInfo for tank "
                << tank->get_owner_id() << ":" << tank->get_tank_id()
                << " — this should have been handled in collect_tank_actions()."
                << std::endl;
      action_applied = true;
      continue;
    }
    if (in_backward_move_sequence)
    {
      tank->advance_backward_state();
    }

    if (tank->get_health() > 0)
    {
      // tick shooting cooldown
      tank->tick_cooldown();

      // Mine check (only for living tanks)
      auto &tile = map->get_tile(tank->get_x(), tank->get_y());
      auto ground = tile.ground.lock();
      if (ground && ground->get_type() == EntityType::MINE)
      {
        tank->damage(); // or tank->set_health(0);
        if (tank->get_health() == 0)
        {
          printer.markTankKilled(tank->get_all_tank_index());
          tile.actor.reset();
        }
        tile.ground.reset(); // Remove the mine
        // Optionally destroy the mine object if needed
        game_entities.erase(
            std::remove_if(game_entities.begin(), game_entities.end(),
                           [&ground](const std::shared_ptr<Entity> &e)
                           {
                             return e == ground;
                           }),
            game_entities.end());
      }
    }

    if (!action_applied)
    {
      printer.markTankIgnored(tank->get_all_tank_index());
    }
    if (tank->get_health() == 0 && tank )
    {
      printer.markTankKilled(tank->get_all_tank_index());
    }
  }
  // Resolve Tank direct collisions (same tile)
  for (auto &[pos, tanks_here] : end_positions)
  {
    // Scan all tanks, find ones already at this position that didn't move
    for (const auto &tank : game_tanks)
    {
      if (tank && tank->get_health() > 0 &&
          tank->get_x() == pos.first && tank->get_y() == pos.second &&
          std::none_of(tanks_here.begin(), tanks_here.end(),
                       [&](const auto &t)
                       { return t == tank; }))
      {
        tanks_here.push_back(tank);
      }
    }

    if (tanks_here.size() > 1)
    {
      for (auto &tank : tanks_here)
      {
        tank->damage();
        printer.markTankKilled(tank->get_all_tank_index());
        auto &tile = map->get_tile(tank->get_x(), tank->get_y());
        tile.actor.reset();
      }
    }
  }
  // Resolve Tank cross-movement collisions (swapping)
  for (size_t i = 0; i < tank_moves.size(); ++i)
  {
    for (size_t j = i + 1; j < tank_moves.size(); ++j)
    {
      if (tank_moves[i].start == tank_moves[j].end &&
          tank_moves[i].end == tank_moves[j].start)
      {
        auto a = tank_moves[i].tank;
        auto b = tank_moves[j].tank;
        a->damage();
        b->damage();
        if (a->get_health() <= 0)
        {
          auto &tile = map->get_tile(a->get_x(), a->get_y());
          printer.markTankKilled((*a).get_all_tank_index());

          tile.actor.reset();
        }
        if (b->get_health() <= 0)
        {
          auto &tile = map->get_tile(b->get_x(), b->get_y());
          printer.markTankKilled((*b).get_all_tank_index());
          tile.actor.reset();
        }
      }
    }
  }
}

// Phase 4:
GameEndStatus GameManager::check_end_conditions(int current_step,
                                                int &steps_without_shells)
{
  std::vector<int> alive_tanks(MAX_PLAYERS + 1, 0);
  bool any_shells_left = false;

  for (const auto &tank : game_tanks)
  {
    if (!tank || tank->get_health() == 0)
      continue;
    int owner = tank->get_owner_id();
    if (owner >= 1 && owner <= MAX_PLAYERS)
      alive_tanks[owner]++;
    if (tank->get_shell_num())
      any_shells_left = true;
  }

  int alive_players = std::count_if(alive_tanks.begin(), alive_tanks.end(),
                                    [](int c)
                                    { return c > 0; });

  GameEndStatus status;

  if (alive_players == 0 || alive_players == 1)
  {
    status.finished = true;
  }
  else if (!any_shells_left)
  {
    steps_without_shells++;
    if (steps_without_shells >= ZERO_SHELLS_GRACE_STEPS)
    {
      status.finished = true;
      status.tie_due_to_shells = true;
    }
  }
  else
  {
    steps_without_shells = 0;
  }

  return status;
}

// Phase 6
int GameManager::determine_winner(const std::vector<int> &tanks_per_player)
{
  int max_tanks = 0;
  int winner = -1;

  for (size_t i = 0; i < tanks_per_player.size(); ++i)
  {
    if (tanks_per_player[i] > max_tanks)
    {
      max_tanks = tanks_per_player[i];
      winner = static_cast<int>(i + 1); // player index is 1-based
    }
    else if (tanks_per_player[i] == max_tanks && max_tanks > 0)
    {
      winner = -1; // tie if two players have same count
    }
  }

  return winner; // -1 for tie, otherwise winner's ID
}

// Call with (-1,-1) if called for visualisersa
std::unique_ptr<SatelliteView>
GameManager::create_satellite_view(int player_id, int tank_id) const
{
  if (!this->map)
  {
    std::cerr
        << "Error: GameManager::create_satellite_view called with null map."
        << std::endl;
    throw std::runtime_error(
        "GameManager: Map is not loaded, cannot create satellite view.");
  }
  size_t rows = this->map->get_rows();
  size_t cols = this->map->get_cols();
  // for visualizer in case (-1,-1):
  std::unordered_map<std::pair<size_t, size_t>, Direction> direction_map;

  if (rows == 0 || cols == 0)
  {
    std::cerr << "Error: GameManager::create_satellite_view called for map "
                 "with zero dimensions (rows="
              << rows << ", cols=" << cols << ")." << std::endl;
    throw std::runtime_error(
        "GameManager: Map has zero dimensions, cannot create satellite view.");
  }

  // Init to empty spaces
  std::vector<std::vector<char>> view(rows, std::vector<char>(cols, ' '));

  // Add Walls and Mines
  for (const auto &entity : game_entities)
  {
    if (!entity)
      continue;
    size_t x = entity->get_x();
    size_t y = entity->get_y();
    if (x < rows && y < cols)
    {
      switch (entity->get_type())
      {
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
  for (const auto &tank : game_tanks)
  {
    if (!tank || tank->get_health() == 0)
      continue;
    size_t x = tank->get_x();
    size_t y = tank->get_y();
    if (x < rows && y < cols)
    {
      if (tank->get_owner_id() == player_id && tank->get_tank_id() == tank_id)
      {
        view[x][y] = '%'; // requesting tank
      }
      else
      {
        view[x][y] = static_cast<char>('0' + tank->get_owner_id());
      }
      // Add direction for visualizer
      if (player_id == -1 && tank_id == -1)
      {
        // Assume satellite_view is declared later, so we collect this info now
        direction_map.emplace(std::make_pair(x, y), tank->get_direction());
      }
    }
  }

  // add Shells if not destroyed
  for (const auto &shell : game_shells)
  {
    if (!shell || shell->is_destroyed())
      continue;
    size_t x = shell->get_x();
    size_t y = shell->get_y();
    if (x < rows && y < cols)
    {
      view[x][y] = '*';
    }
  }
  // return the MySatelliteView
  // Print the view for debugging

  auto satellite_view = std::make_unique<MySatelliteView>(view);
  if (player_id == -1 && tank_id == -1)
  {
    satellite_view->setDirectionMap(std::move(direction_map));
  }
  return satellite_view;
}

std::ifstream GameManager::open_map_file(const std::string &map_path)
{
  std::cout << "[open_map_file] Attempting to open map file: \"" << map_path
            << "\"\n";

  std::ifstream map_file(map_path);
  if (!map_file.is_open())
  {
    std::cerr << "[open_map_file] ERROR: Failed to open map file: \""
              << map_path << "\"\n";
  }
  else
  {
    std::cout << "[open_map_file] Successfully opened map file.\n";
  }

  return map_file;
}

// Utility: strip spaces around '=' and parse int
int extractIntValue(const std::string &line, const std::string &expected_key)
{
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
                                   int &cols)
{
  std::string line;
  try
  {
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
  }
  catch (const std::exception &e)
  {
    std::cerr << "[parse_map_header] ERROR: " << e.what() << '\n';
    return false;
  }

  return true;
}

std::vector<std::vector<Tile>> GameManager::create_empty_map(int rows,
                                                             int cols)
{
  std::vector<std::vector<Tile>> map(rows, std::vector<Tile>(cols));
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
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
    MyTankAlgorithmFactory &tank_algorithm_factory)
{
  if (static_cast<int>(line.size()) > cols)
  {
    this->input_errors.emplace_back("Row " + std::to_string(row_idx) +
                                    " has more columns than expected. Ignored extra characters.");
  }
  if (static_cast<int>(line.size()) < cols)
  {
    this->input_errors.emplace_back("Row " + std::to_string(row_idx) +
                                    " is shorter than expected. Filled missing columns with spaces.");
  }

  for (int col = 0; col < cols; ++col)
  {
    Tile &tile = map[row_idx][col];

    char c = (col < static_cast<int>(line.size())) ? line[col] : ' ';

    switch (c)
    {
    case '#':
    {
      auto wall = std::make_shared<Wall>(row_idx, col);
      tile.ground = wall;
      entities_out.push_back(wall);
      break;
    }
    case '@':
    {
      auto mine = std::make_shared<Mine>(row_idx, col);
      tile.ground = mine;
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
    case '9':
    {
      int player_num = c - '0';
      Direction dir = (player_num == 1 ? Direction::L : Direction::R);
      // init tank_id to -1 temporarly
      auto tank =
          std::make_shared<Tank>(row_idx, col, dir, player_num, -1, nullptr);
      tank->set_direction(dir);
      tank->set_shell_num(num_shells);
      std::cout << "[Tank Init] Tank for player " << player_num << " at ("
                << row_idx << ", " << col << ") initialized with "
                << tank->get_shell_num() << " shells." << std::endl;
      tile.actor = tank;
      tanks_out.push_back(tank);
      player_spawn_points_out.emplace_back(player_num,
                                           std::make_pair(row_idx, col));
      break;
    }
    default:
      // Treated as empty space
      if (c != ' ')
      { // Don't warn on actual empty space
        this->input_errors.emplace_back(
            "Unknown character '" + std::string(1, c) +
            "' at row " + std::to_string(row_idx) +
            ", col " + std::to_string(col) +
            ". Treated as empty.");
      }
      break;
    }
  }
}

void GameManager::fill_remaining_rows(int start_row, int rows, int cols,
                                      std::vector<std::vector<Tile>> &map)
{
  for (int r = start_row; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      map[r][c].x = r;
      map[r][c].y = c;
    }
  }
}
