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

class GamePlayer1 : public GamePlayer {
public:
  GamePlayer1(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer1();
};

class GamePlayer2 : public GamePlayer {
public:
  GamePlayer2(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer2();
};

class GamePlayer3 : public GamePlayer {
public:
  GamePlayer3(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer3();
};

class GamePlayer4 : public GamePlayer {
public:
  GamePlayer4(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer4();
};

class GamePlayer5 : public GamePlayer {
public:
  GamePlayer5(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer5();
};

class GamePlayer6 : public GamePlayer {
public:
  GamePlayer6(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer6();
};

class GamePlayer7 : public GamePlayer {
public:
  GamePlayer7(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer7();
};

class GamePlayer8 : public GamePlayer {
public:
  GamePlayer8(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer8();
};

class GamePlayer9 : public GamePlayer {
public:
  GamePlayer9(size_t x, size_t y, size_t max_steps,
              size_t num_shells, std::string name,
              std::vector<std::weak_ptr<Tank>> tanks);
  ~GamePlayer9();
};

#endif