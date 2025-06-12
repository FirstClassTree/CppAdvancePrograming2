#include "MoveableEntity.h"
#include <iostream>


MoveableEntity::~MoveableEntity() {}

Direction MoveableEntity::get_direction() const { return this->direction; }

void MoveableEntity::set_direction(Direction direction) {
    this->direction = direction;
}

void MoveableEntity::set_pos(int x, int y) {
    this->x = x;
    this->y = y;
}