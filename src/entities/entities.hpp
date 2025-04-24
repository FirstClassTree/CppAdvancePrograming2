#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "utils.hpp"
#include <vector>
#include <math.h>
enum class EntityType
{
    Wall,
    Tank,
    Mine,
    Shell
};

class Entity
{
public:
    explicit Entity(int x, int y);

    int pos_x;
    int pos_y;
    int id;

    virtual Action colide(Entity *e) = 0;
    virtual EntityType get_type() const = 0;
    virtual void update_pos(int x, int y) = 0;
    virtual Pos get_pos() = 0;
};
class LivingEntity : public Entity
{
public:
    explicit LivingEntity(int pos_x, int pos_y, Direction dir);
    virtual Action declare_move() = 0;

    Direction entity_dir;
};

class Mine : public Entity
{
public:
    explicit Mine(int x, int y);
    void update_pos(int x, int y) override;
    Pos get_pos() override;
    Action colide(Entity *e) override;
    EntityType get_type() const override;
};
class Tank : public LivingEntity
{
public:
    explicit Tank(int pos_x, int pos_y, Direction dir);

    void shoot();
    void request_backwards();
    int is_reverse();
    void reset_reverse();
    int is_reloading();
    int get_ammo();
    void tick();
    Tank *closest_target(std::vector<Tank *> others);
    Action colide(Entity *e) override;
    EntityType get_type() const override;
    Action declare_move() override;
    void update_pos(int x, int y) override;
    Pos get_pos() override;

private:
    int ammo=16;
    int reload_state=0;
    int reverse_state=0;
};
class Wall : public Entity
{
public:
    explicit Wall(int pos_x, int pos_y);
    void hit();
    Action colide(Entity *e) override;
    void update_pos(int x, int y) override;
    Pos get_pos() override;
    EntityType get_type() const override;

    int health;
};
class Shell : public LivingEntity
{
public:
    explicit Shell(int owner, int pos_x, int pos_y, Direction dir);
    int get_owner();
    Action colide(Entity *e) override;
    void update_pos(int x, int y) override;
    Pos get_pos() override;
    EntityType get_type() const override;
    Action declare_move() override;

private:
    int owner;
    int speed;
};
#endif