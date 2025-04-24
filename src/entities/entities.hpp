#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "utils.hpp"
#include <vector>
#include <math.h>


enum class EntityType{
    Wall,Tank,Mine,Shell
};

class Entity{
    public:
        explicit Entity(int x, int y);

        int pos_x;
        int pos_y;

        virtual Action colide(Entity* e);
        virtual EntityType get_type() const;
        virtual void update_pos(int x,int y);
        virtual Pos get_pos();

};
class LivingEntity: public Entity{
    public:
    explicit LivingEntity(int pos_x,int pos_y,Direction dir);
    virtual Action declare_move();

    Direction entity_dir;
};

class Mine: public Entity {
    public:
        explicit Mine(int x, int y);
        Action colide(Entity* e) override;
};
class Tank: public LivingEntity{
    public:
        explicit Tank(int pos_x,int pos_y);

        void shoot();
        void request_backwards();
        int is_reverse();
        void reset_reverse();
        int is_reloading();
        int get_ammo();
        void tick();
        Tank* closest_target(std::vector<Tank*> others);
        Action colide(Entity* e) override;

    private:
        int ammo;
        int reload_state;
        Direction tank_direction;
        int reverse_state;
};
class Wall:public Entity{
    public:
        explicit Wall(int pos_x,int pos_y);
        void hit();
        Action colide(Entity* e) override;

        int health;
};
class Shell: public LivingEntity{
    public:
        explicit Shell(int owner,int pos_x,int pos_y,Direction dir);
        int get_owner();
        Action colide(Entity* e) override;
    private:
        int owner;
        int speed;
};
#endif