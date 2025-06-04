#include "../common/Visualizer.h"
#include "../common/MySatelliteView.h"
#include <memory>
#include <vector>

int main() {
    Visualizer visualizer;

    // Example: 3 snapshots with different patterns
    for (int i = 0; i < 3; ++i) {
        std::vector<std::vector<char>> grid = {
            {'#', '#', '#', '#', '#'},
            {'#', static_cast<char>('0' + i), ' ', '@', '#'},
            {'#', ' ', '*', ' ', '#'},
            {'#', '#', '#', '#', '#'}
        };
        auto view = std::make_unique<MySatelliteView>(grid);
        visualizer.add_snapshot(std::move(view));
    }

    visualizer.run();  // Launch visualizer navigation
    return 0;
}
