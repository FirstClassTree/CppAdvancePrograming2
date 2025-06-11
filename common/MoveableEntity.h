#ifndef MOVEABLEENTITY_H
#define MOVEABLEENTITY_H

#include "Direction.h"
#include "Entity.h"

class MoveableEntity : public Entity {
public:
  MoveableEntity(int x, int y, int speed, EntityType type, Direction direction)
      : Entity(x, y, type), direction(direction), speed(speed) {}
  ~MoveableEntity();
  virtual std::string serialize() = 0;

  Direction get_direction();
  void set_pos(int x, int y);
  void set_direction(Direction direction);

private:
  Direction direction;
  int speed;
};

#endif