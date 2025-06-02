#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include "IJson.h"
#include "Player.h"

class GamePlayer : public Player, public IJson {
public:
  GamePlayer(int player_index, size_t x, size_t y, size_t max_steps,
             size_t num_shells, std::string name);
  ~GamePlayer();

  std::string serialize() override;

  void updateTankWithBattleInfo(TankAlgorithm &tank,
                                SatelliteView &satellite_view) override;
  int get_id() const;
  

private:
  int id;
  std::string name;
};

#endif