#include "entities.hpp"

Action Tank::colide(Entity* e){
    EntityType type = e->get_type();
    if(type == EntityType::Mine){
        return get_hit_action();
    } else if(type == EntityType::Shell){
        return get_hit_action();
    } else if(type == EntityType::Wall){
        return get_hit_action();
    } else if(type == EntityType::Tank){
        return get_hit_action();
    }
}