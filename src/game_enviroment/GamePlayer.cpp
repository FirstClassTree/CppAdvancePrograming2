#include "../../common/GamePlayer.h"
#include <vector>

GamePlayer::GamePlayer(int player_index, size_t x, size_t y, size_t max_steps,
                       size_t num_shells, std::string name,
                       std::vector<std::weak_ptr<Tank>> tanks)
    : Player(player_index, x, y, max_steps, num_shells), id(player_index),
      name(name), tanks(tanks), map_width(x), map_height(y),
      player_index(player_index), max_steps(max_steps), num_shells(num_shells) {
}

GamePlayer::~GamePlayer() {}

void GamePlayer::pruneDeadTanks() {
  this->tanks.erase(std::remove_if(this->tanks.begin(), this->tanks.end(),
                                   [](const std::weak_ptr<Tank> &wp) {
                                     return wp.expired();
                                   }),
                    this->tanks.end());
}
// TODO: this will fail when the player have no tanks so it must relay on the
// game manager to not let dead players play.
std::weak_ptr<Tank> GamePlayer::get_tank(int tank_index) {
  for (auto &tank : this->tanks) {
    if (tank.lock()->get_tank_id() == tank_index) {
      return tank;
    }
  }
  return std::weak_ptr<Tank>();
}

int GamePlayer::get_id() const { return id; }

std::vector<std::vector<char>>
GamePlayer::extract_view(SatelliteView &satellite_view) {
  std::vector<std::vector<char>> view(map_height, std::vector<char>(map_width));

  for (std::size_t r = 0; r < map_height; ++r)
    for (std::size_t c = 0; c < map_width; ++c)
      view[r][c] = satellite_view.getObjectAt(r, c);

  return view;
}

void GamePlayer::updateTankWithBattleInfo(TankAlgorithm &tank,
                                          SatelliteView &satellite_view) {

  this->pruneDeadTanks();
  auto view = extract_view(satellite_view);
  TankBattleInfo info(this->num_shells, view);
  tank.updateBattleInfo(info);
}

std::string GamePlayer::serialize() {
  nlohmann::json j;
  j["id"] = id;
  j["name"] = name;
  return j.dump();
}