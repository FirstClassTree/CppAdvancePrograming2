#include "entities.hpp"

class Entity{
    public:
        int pos_x;
        int pos_y;

        explicit Entity(int pos_x,int pos_y){
            this->pos_x = pos_x;
            this->pos_y = pos_y;
        }

        virtual void tick(){};
};