#ifndef GAME_ENV_HPP
#define GAME_ENV_HPP

#include <vector>
#include <entities.hpp>
using namespace std;

class Player
{
    public:
        explicit Player(int id);

    private:
        vector<Tank> tank_list;
        
};

class GameManager
{
public:
    explicit GameManager();

    int load_game();
    int run_game();
    void tick();
    int check_legal_move();
    int check_victory();
    void subscribe_entity(Entity e);
    int set_board_item(int x,int y,char item);

    vector<vector<char>> get_board();

private:
    int board_w;
    int board_h;
    int turn_index = 0;
    vector<Entity> game_entities;
    vector<vector<char>> game_board;
};

#endif