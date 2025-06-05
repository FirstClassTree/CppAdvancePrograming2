#include "./common/Visualizer.h"
#include "./common/MySatelliteView.h"
#include "./common/Direction.h"
#include <memory>
#include <vector>
#include <tuple>
#include <unordered_map>

int main() {
    Visualizer visualizer;

    std::vector<Direction> all_directions = {
        Direction::U, Direction::UR, Direction::R,
        Direction::RD, Direction::D, Direction::DL,
        Direction::L, Direction::UL, Direction::U  // wrap around for 9th
    };

    for (int i = 1; i <= 9; ++i) {
        std::vector<std::vector<char>> grid = {
            {'#', '#', '#', '#', '#'},
            {'#', static_cast<char>('0' + i), ' ', '@', '#'},
            {'#', ' ', '*', ' ', '#'},
            {'#', '#', '#', '#', '#'}
        };
        

        auto view = std::make_unique<MySatelliteView>(grid);
        std::unordered_map<std::pair<size_t, size_t>, Direction> direction_map;
        direction_map[std::make_pair(1, 1)] = all_directions[i - 1];
        view->setDirectionMap(std::move(direction_map));

        visualizer.add_snapshot(std::move(view));
    }

    visualizer.run();
    return 0;
}
