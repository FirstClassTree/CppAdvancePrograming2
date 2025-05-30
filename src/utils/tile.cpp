#include "utils/Tile.h"
#include "entities/Tank.h"

Tile::Tile(int x, int y, std::shared_ptr<Entity> ground, std::shared_ptr<Tank> actor, std::shared_ptr<Shell> shell)
    : x(x), y(y), ground(ground), actor(actor), shell(shell) {}

Tile::Tile()
    : x(0), y(0), ground(), actor(), shell() {}
    
Tile::~Tile() {}


std::string Tile::serialize() {
    nlohmann::json j;
    j["x"] = x;
    j["y"] = y;
    j["ground"] = ground.lock()->serialize();
    j["actor"] = actor.lock()->serialize();
    j["shell"] = shell.lock()->serialize();
    return j.dump();
}