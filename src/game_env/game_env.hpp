#ifndef GAME_ENV_HPP
#define GAME_ENV_HPP

#include <vector>
#include <entities.hpp>
#include <utils.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Player
{
    public:
        explicit Player(int id);
        int id;
        vector<Tank*> tank_list;
        
};

class GameManager
{
public:
    explicit GameManager();

    int load_game(const std::string& filename);
    int run_game();
    void tick();
    int check_legal_move();
    void apply_action(Entity* e,Action action);
    int check_victory();
    void subscribe_entity(Entity* e);
    void subscribe_shell(Shell* e);
    Entity* test_collision(int x,int y);
    int set_board_item(int x,int y,char item);

    vector<vector<Tile>> get_board();

private:
    int board_w;
    int board_h;
    int turn_index = 0;
    vector<Entity*> game_entities;
    vector<Shell*> game_shells;
    vector<vector<Tile>> game_board;
    vector<Player*> game_players;
};

#endif