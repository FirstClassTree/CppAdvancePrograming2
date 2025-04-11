#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "utils.hpp"

class Entity{
    public:
        explicit Entity(int x, int y);

        int pos_x;
        int pos_y;

        virtual void tick();
        virtual void colide();

};
class LivingEntity: public Entity{
    public:
    explicit LivingEntity(int pos_x,int pos_y);

    const int speed;
    int speed_counter;
};

class Mine: public LivingEntity {
    public:
        explicit Mine(int x, int y);
};
class Tank: public LivingEntity{
    public:
        explicit Tank(int pos_x,int pos_y);

        void shoot();
        void request_backwards();
        int is_reverse();
        int is_reloading();
        int get_ammo();
        void tick() override;

    private:
        int ammo;
        int reload_state;
        Direction tank_direction;
        int reverse_state;
};
class Wall:public LivingEntity{
    public:
        explicit Wall(int pos_x,int pos_y);
        void hit();

        int health;
};
class Shell: public LivingEntity{
    public:
        explicit Shell(int owner,int pos_x,int pos_y);
        int get_owner();
        void tick() override;
    private:
        int owner;
        int speed;
};

#endif