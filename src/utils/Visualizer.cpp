#include "../../common/Visualizer.h"
#include "../../common/MySatelliteView.h"
// #include "../../common/InputHelper.h"

#include <iostream>
#include <ncurses.h>

// Create SatelliteView with paramaters: (-1,-1) so no &
void Visualizer::add_snapshot(std::unique_ptr<SatelliteView> view) {
    snapshots_.push_back(std::move(view));
}

void Visualizer::run() {
    if (snapshots_.empty()) {
        std::cout << "No snapshots to visualize.\n";
        return;
    }

    initscr();             // NCurses init
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    size_t index = 0;
    draw_view(*snapshots_[index], index);

    int ch;
    while ((ch = getch()) != 'q') {
        if (ch == KEY_RIGHT && index + 1 < snapshots_.size()) {
            index++;
            draw_view(*snapshots_[index], index);
        } else if (ch == KEY_LEFT && index > 0) {
            index--;
            draw_view(*snapshots_[index], index);
        }
    }

    endwin();  // Restore terminal
}



void Visualizer::draw_view(const SatelliteView& view, size_t round_index) const {
    const auto* my_view = dynamic_cast<const MySatelliteView*>(&view);
    if (!my_view) return;

    const auto& grid = my_view->get_grid();

    clear();  // Clear ncurses screen
    mvprintw(0, 0, "-- ROUND %zu --", round_index);


    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            mvaddch(i + 2, j+1, grid[i][j]);
        }
    }

    mvprintw(grid.size() + 3, 0, "[<-] Previous | [->] Next | [q] Quit");
    refresh();
}
