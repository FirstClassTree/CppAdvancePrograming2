#pragma once

#include "Player.h"
#include "TankAlgorithm.h"
#include <vector>
#include <memory>

class MyPlayer : public Player {
public:
    MyPlayer(int player_index, size_t x, size_t y, size_t /*max_steps*/, size_t /*num_shells*/);
    ~MyPlayer() override = default;

    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;

    void registerTank(std::shared_ptr<TankAlgorithm> tank_algo);
    int get_id() const;

private:
    int player_index;
    std::vector<std::shared_ptr<TankAlgorithm>> tanks;
};
