#ifndef GAME_ENV_HPP
#define GAME_ENV_HPP

#include <vector>
#include "entities.hpp"
#include "utils.hpp"
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "game_algo.hpp"
#include <chrono>
#include <thread>


using namespace std;

class Player
{
public:
    explicit Player(int id);
    int id;
    vector<Tank *> tank_list;
};

class GameManager
{
public:
    explicit GameManager();

    int load_game(const std::string &filename);
    int game_loop();
    int tick();
    int check_legal_move(Action action,Tank* commiter);
    void apply_action(Entity *e, Action action);
    int check_victory();
    void subscribe_entity(Entity *e);
    void subscribe_shell(Shell *e);
    Entity *test_collision(LivingEntity *self, Action action);
    int set_board_item(int x, int y, char item);
    void print_result();

    vector<vector<Tile>> get_board();

private:
    vector<Tank*> collect_game_tanks(Tank* ignore);
    int board_w;
    int board_h;
    int turn_index = 0;
    int no_ammo_index = 0;
    int entity_index = 0;
    Logger logger;
    GameState game_state;
    vector<Entity *> game_entities;
    vector<Shell *> game_shells;
    vector<vector<Tile>> game_board;
    vector<Player *> game_players;
};

#endif