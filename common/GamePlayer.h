#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include "IJson.h"
#include "Player.h"
#include "algorithms/TankBattleInfo.h"
#include "entities/Tank.h"

class GamePlayer : public Player, public IJson {
public:
  GamePlayer(int player_index, size_t x, size_t y, size_t max_steps,
             size_t num_shells, std::string name, std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer();

  std::string serialize() override;

  void updateTankWithBattleInfo(TankAlgorithm &tank,
                                SatelliteView &satellite_view) override;
  int get_id() const;

  void pruneDeadTanks();
  std::vector<std::vector<char>> extract_view(SatelliteView &satellite_view);

  std::vector<std::weak_ptr<Tank>> tanks;
  std::weak_ptr<Tank> get_tank(int tank_index);
  

private:
  int id;
  int map_width,map_height;
  int player_index;
  int max_steps;
  int num_shells;
  std::string name;
};

#endif