#include "game_env.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: tanks_game <input_file>\n";
        return 1;
    }

    std::string input_file = argv[1];
    GameManager game;

    if (game.load_game(input_file) != 0)
    {
        std::cerr << "Failed to load game board.\n";
        return 1;
    }
    game.game_loop();
    game.print_result();
    return 0;
}