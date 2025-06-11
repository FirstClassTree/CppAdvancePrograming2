#include "../../common/GamePlayer.h"
#include <iostream>
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
  std::vector<std::vector<char>> view(map_width, std::vector<char>(map_height));
  for (std::size_t r = 0; r < map_width; ++r)
    for (std::size_t c = 0; c < map_height; ++c) {
      view[r][c] = satellite_view.getObjectAt(r, c);
      //std::cout << "View at " << r << ", " << c << ": " << view[r][c] << std::endl;
    }

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


GamePlayer1::GamePlayer1(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(1, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer1::~GamePlayer1() {}

GamePlayer2::GamePlayer2(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(2, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer2::~GamePlayer2() {}

GamePlayer3::GamePlayer3(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(3, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer3::~GamePlayer3() {}

GamePlayer4::GamePlayer4(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(4, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer4::~GamePlayer4() {}

GamePlayer5::GamePlayer5(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(5, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer5::~GamePlayer5() {}

GamePlayer6::GamePlayer6(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(6, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer6::~GamePlayer6() {}

GamePlayer7::GamePlayer7(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(7, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer7::~GamePlayer7() {}

GamePlayer8::GamePlayer8(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(8, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer8::~GamePlayer8() {}

GamePlayer9::GamePlayer9(size_t x, size_t y, size_t max_steps,
                         size_t num_shells, std::string name,
                         std::vector<std::weak_ptr<Tank>> tanks)
    : GamePlayer(9, x, y, max_steps, num_shells, name, tanks) {}

GamePlayer9::~GamePlayer9() {}