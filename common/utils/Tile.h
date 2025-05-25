#ifndef TILE_H
#define TILE_H

#include "../Entity.h"
#include "../entities/Shell.h"
#include "../entities/Tank.h"
#include "IJson.h"

class Tile : public IJson {
public:
  Tile(int x, int y, std::shared_ptr<Entity> ground,
       std::shared_ptr<Tank> actor, std::shared_ptr<Shell> shell);
  ~Tile();

  std::string serialize() override;


  int x;
  int y;
  std::weak_ptr<Entity> ground;
  std::weak_ptr<Tank> actor;
  std::weak_ptr<Shell> shell;
};

#endif