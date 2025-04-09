#include "entities.hpp"

class Wall : public Entity
{
public:
    int health;

    explicit Wall(int pos_x, int pos_y) : Entity(pos_x, pos_y)
    {
        this->health = 2;
    }
};