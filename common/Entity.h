#ifndef ENTITY_H
#define ENTITY_H

#include "IJson.h"
#include "EntityType.h"

class Entity : public IJson {
public:
  explicit Entity(int x, int y, EntityType type);
  ~Entity();
  EntityType get_type();
  int get_x();
  int get_y();

private:
 int x;
 int y;
 EntityType type;
};

#endif