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

  void set_pos(int x, int y);
  void set_direction(Direction direction);
  Direction get_direction() const;

private:
  Direction direction;
  int speed;
};

#endif