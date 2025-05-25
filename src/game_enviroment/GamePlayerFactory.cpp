#include "GamePlayerFactory.h"

std::unique_ptr<Player> GamePlayerFactory::create(int player_index, size_t x,
                                                  size_t y, size_t max_steps,
                                                  size_t num_shells) const {
  return std::make_unique<GamePlayer>(player_index, x, y, max_steps, num_shells,
                                      "Player" + std::to_string(player_index));
}