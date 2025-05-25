#ifndef GAMEPLAYERFACTORY_H
#define GAMEPLAYERFACTORY_H

#include "GamePlayer.h"
#include "PlayerFactory.h"

class GamePlayerFactory : public PlayerFactory {
public:
  ~GamePlayerFactory() {}
  std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                 size_t max_steps,
                                 size_t num_shells) const override;
};

#endif