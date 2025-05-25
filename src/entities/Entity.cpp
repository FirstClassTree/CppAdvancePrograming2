#include "Entity.h"

Entity::Entity(int x, int y, EntityType type) : x(x), y(y), type(type) {}

Entity::~Entity() {}

int Entity::get_x() { return x; }

int Entity::get_y() { return y; }

EntityType Entity::get_type() { return type; }