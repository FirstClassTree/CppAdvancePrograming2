#include "../../common/GamePlayer.h"

GamePlayer::GamePlayer(int player_index, size_t x, size_t y, size_t max_steps,
                       size_t num_shells, std::string name)
    : Player(player_index, x, y, max_steps, num_shells), id(player_index),
      name(name) {}

GamePlayer::~GamePlayer() {}

int GamePlayer::get_id() const { return id; }

void GamePlayer::updateTankWithBattleInfo(TankAlgorithm &tank,
                                          SatelliteView &satellite_view) {
  // TODO: implement
}

std::string GamePlayer::serialize() {
  nlohmann::json j;
  j["id"] = id;
  j["name"] = name;
  return j.dump();
}