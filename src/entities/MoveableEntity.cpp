#include "MoveableEntity.h"


MoveableEntity::~MoveableEntity() {}

Direction MoveableEntity::get_direction() { return direction; }

void MoveableEntity::set_direction(Direction direction) {
    direction = direction;
}

void MoveableEntity::set_pos(int x, int y) {
    this->x = x;
    this->y = y;
}