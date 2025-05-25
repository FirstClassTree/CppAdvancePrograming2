#ifndef WALL_H
#define WALL_H

#include "../Entity.h"

class Wall : public Entity {
public:
  Wall(int x, int y);
  ~Wall();

  std::string serialize() override;

};

#endif