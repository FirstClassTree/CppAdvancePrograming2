#include "entities.hpp"

class Mine: public Entity{
    public:
        explicit Mine(int pos_x,int pos_y): Entity(pos_x,pos_y){
            
            this->pos_x = pos_x;
            this->pos_y = pos_y;
        }
};