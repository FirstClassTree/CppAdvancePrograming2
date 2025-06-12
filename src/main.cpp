#include <iostream>
#include "../common/GameManager.h"
#include "../common/GamePlayerFactory.h"
#include "../common/Constants.h"
#include "../common/ConfigManager.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: tanks_game <map_file> [--visual]" << std::endl;
        return 1;
    }

    std::string map_file = argv[1];
    bool visual = argc > 2 && std::string(argv[2]) == "--visual";

    ConfigManager::instance().loadFromFile("../files/config.json");
    initialize_config();

    GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, visual);
    //GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}); also works
    game.load_map(map_file);
    game.run();
}