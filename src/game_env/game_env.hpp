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
    int tick();
    int check_legal_move();
    int check_victory();

    int set_board_item(int x,int y,char item);

    vector<vector<char>> get_board();

private:
    int board_w;
    int board_h;
    vector<vector<char>> game_board;
};
#endif