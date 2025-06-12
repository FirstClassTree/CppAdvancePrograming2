#include "GamePlayerFactory.h"

std::unique_ptr<Player> GamePlayerFactory::create(int player_index, size_t x,
                                                  size_t y, size_t max_steps,
                                                  size_t num_shells) const {
  
  // The Diffrent GamePlayer classes all produce diffrent AI allocations, causing uniuqe concrete GamePlayer Classes
  switch (player_index) {
  case 1:
    return std::make_unique<GamePlayer1>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 2:
    return std::make_unique<GamePlayer2>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 3:
    return std::make_unique<GamePlayer3>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 4:
    return std::make_unique<GamePlayer4>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 5:
    return std::make_unique<GamePlayer5>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 6:
    return std::make_unique<GamePlayer6>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 7:
    return std::make_unique<GamePlayer7>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 8:
    return std::make_unique<GamePlayer8>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  case 9:
    return std::make_unique<GamePlayer9>(
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  default:
        return std::make_unique<GamePlayer>(player_index,
        x, y, max_steps, num_shells, "Player" + std::to_string(player_index),
        std::vector<std::weak_ptr<Tank>>());
  }
}