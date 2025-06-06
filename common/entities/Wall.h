#ifndef WALL_H
#define WALL_H

#include "../Entity.h"

class Wall : public Entity {
private:
  int health;

public:
  Wall(int x, int y);
  ~Wall() override = default;

  std::string serialize() override;

  void weaken();               // Decreases wall health
  bool is_destroyed() const; // Returns true if health <= 0
};

#endif