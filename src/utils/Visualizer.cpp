#include "../common/Visualizer.h"
#include "../common/MySatelliteView.h"

#include <iostream>

// Create SatelliteView with paramaters: (-1,-1) so no &
void Visualizer::add_snapshot(std::unique_ptr<SatelliteView> view) {
    snapshots_.push_back(std::move(view));
}

void Visualizer::run() {
    if (snapshots_.empty()) {
        std::cout << "No snapshots to visualize.\n";
        return;
    }

    size_t index = 0;
    draw_view(*snapshots_[index], index);

    // TODO: add interactive ←/→ navigation here
}

void Visualizer::draw_view(const SatelliteView& view, size_t round_index) const {
    const auto* my_view = dynamic_cast<const MySatelliteView*>(&view);
    if (!my_view) return;

    const auto& grid = my_view->get_grid();

    std::cout << "\033[2J\033[1;1H";  // Clear terminal
    std::cout << "-- ROUND " << round_index << " --\n";

    for (const auto& row : grid) {
        for (char c : row) std::cout << c;
        std::cout << '\n';
    }

    std::cout << "\n[←] Previous | [→] Next | [q] Quit\n";
}
