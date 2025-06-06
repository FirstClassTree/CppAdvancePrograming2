#include "MoveableEntity.h"


MoveableEntity::~MoveableEntity() {}

Direction MoveableEntity::get_direction() { return direction; }

void MoveableEntity::set_pos(int x, int y) {
    this->x = x;
    this->y = y;
}