#ifndef ENTITY_H
#define ENTITY_H

#include "EntityType.h"
#include "IJson.h"

class Entity : public IJson {
public:
  explicit Entity(int x, int y, EntityType type);
  ~Entity();
  EntityType get_type();
  int get_x();
  int get_y();
  int get_uuid();
  inline static int global_entity_counter = 0;

private:
  int x;
  int y;
  int uuid;
  EntityType type;
  
};

#endif