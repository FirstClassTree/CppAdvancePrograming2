#ifndef MINE_H
#define MINE_H

#include "../Entity.h"

class Mine : public Entity {
public:
  Mine(int x, int y);
  ~Mine();

  std::string serialize() override;

};

#endif