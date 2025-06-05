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
    start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);         // Player 1 - Red
    init_pair(2, COLOR_BLUE, COLOR_BLACK);        // Player 2 - Blue
    init_pair(3, COLOR_GREEN, COLOR_BLACK);       // Player 3 - Green
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);      // Player 4 - Yellow
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);     // Player 5 - Magenta
    init_pair(6, COLOR_CYAN, COLOR_BLACK);        // Player 6 - Cyan
    init_pair(7, COLOR_BLACK, COLOR_YELLOW);      // Player 7 - Black on Yellow (standout)
    init_pair(8, COLOR_BLACK, COLOR_CYAN);        // Player 8 - Black on Cyan (cool contrast)
    init_pair(9, COLOR_BLACK, COLOR_MAGENTA);     // Player 9 - Black on Magenta

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

// note \\ is escape charchter so we need two
char direction_to_char(Direction dir) {
    switch (dir) {
        case Direction::U:  return '^';
        case Direction::UR: return '/';
        case Direction::R:  return '>';
        case Direction::RD: return '\\';
        case Direction::D:  return 'v';
        case Direction::DL: return '/';
        case Direction::L:  return '<';
        case Direction::UL: return '\\';
        default: return '?';
    }
}

void Visualizer::draw_view(const SatelliteView& view, size_t round_index) const {
    const auto* my_view = dynamic_cast<const MySatelliteView*>(&view);
    if (!my_view) return;

    const auto& grid = my_view->get_grid();

    clear();  // Clear ncurses screen
    mvprintw(0, 0, "-- ROUND %zu --", round_index);


    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            char ch = grid[i][j];
            // if digits add color and direction
            if (ch >= '1' && ch <= '9') {
                int color_pair = ch - '0';  // 1-9
                auto it = my_view->get_directions().find(std::make_pair(i, j));
                char dir_char = (it != my_view->get_directions().end()) ? direction_to_char(it->second) : ch;

                attron(COLOR_PAIR(color_pair));
                mvaddch(i + 2, j + 1, dir_char);
                attroff(COLOR_PAIR(color_pair));
            } else {
                mvaddch(i + 2, j + 1, ch);
            }
        }
    }

    mvprintw(grid.size() + 3, 0, "[<-] Previous | [->] Next | [q] Quit");
    int base_y = grid.size() + 5;
    for (int i = 1; i <= 9; ++i) {
        attron(COLOR_PAIR(i));
        mvprintw(base_y + (i - 1) / 3, (i - 1) % 3 * 12, "Player %d", i);
        attroff(COLOR_PAIR(i));
    }
    refresh();
}



