#include <iostream>
#include "../common/GameManager.h"
#include "../common/GamePlayerFactory.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: tanks_game <map_file> [--visual]" << std::endl;
        return 1;
    }

    std::string map_file = argv[1];
    bool visual = argc > 2 && std::string(argv[2]) == "--visual";

    GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, visual);
    game.load_map(map_file);
    game.run();
}


// int main(){
//     std::cout << "Hello, World!" << std::endl;
//     return 0;
// }

// int main(int argc, char** argv) {
// GameManager game(MyPlayerFactory{}, MyTankAlgorithmFactory{});
// game.readBoard(<game_board_input_file from command line>);
// 	game.run();
// }
