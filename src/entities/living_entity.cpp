#include "entities.hpp"

LivingEntity::LivingEntity(int pos_x,int pos_y,Direction dir):Entity(pos_x,pos_y){
    this->entity_dir = dir;
}