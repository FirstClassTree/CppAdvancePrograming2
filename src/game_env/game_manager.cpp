#include "game_env.hpp"

GameManager::GameManager()
{
    this->game_state.status = GameStatus::Idle;
    this->game_state.winner = -1;
}

int inline has_ammo(vector<Player *> players)
{
    for (Player *player : players)
    {
        for (Tank *tank : player->tank_list)
        {
            if (tank->get_ammo() != 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int GameManager::check_victory()
{
    int p1 = this->game_players[0]->tank_list.size();
    int p2 = this->game_players[1]->tank_list.size();
    if (p1 == p2 && p1 == 0)
    {
        this->game_state.status = GameStatus::Tie;
        return 1;
    }
    if (p1 == 0)
    {
        this->game_state.status = GameStatus::Winner;
        this->game_state.winner = 2;
        return 1;
    }
    if (p2 == 0)
    {
        this->game_state.status = GameStatus::Winner;
        this->game_state.winner = 1;
        return 1;
    }
    if (has_ammo(this->game_players))
    {
        return 0;
    }
    else
    {
        if (this->no_ammo_index == 40)
        {
            this->game_state.status = GameStatus::Tie;
            return 1;
        }
        this->no_ammo_index++;
    }
    return 0;
}
vector<Tank *> GameManager::collect_game_tanks(Tank *ignore)
{
    vector<Tank *> tanks = {};
    for (Player *player : this->game_players)
    {
        for (Tank *tank : player->tank_list)
        {
            if (tank != ignore)
            {
                tanks.push_back(tank);
            }
        }
    }
    return tanks;
}
int GameManager::game_loop()
{
    while (this->check_victory() == 0)
    {
        this->tick();
        std::cout << this->turn_index <<endl;
    }
    return 0;
}
void GameManager::subscribe_entity(Entity *e)
{
    this->game_entities.push_back(e);
}
void GameManager::subscribe_shell(Shell *e)
{
    this->game_shells.push_back(e);
}
int GameManager::tick()
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = game_shells.size() - 1; i >= 0; --i)
        {
            Shell *shell = this->game_shells[i];
            Action action = shell->declare_move();
            Entity *collided = this->test_collision(shell, action);
            if (collided)
            {
                action = shell->colide(collided);
                Action collided_result = collided->colide(shell);
                if (collided_result.type == ActionType::None)
                {
                    this->apply_action(shell, action);
                }
                else
                {
                    this->apply_action(collided, collided_result);
                }
            }
            else
            {
                this->apply_action(shell, action);
            }
        }
    }
    for (int i = this->game_players.size() - 1; i >= 0; --i)
    {
        Player *current = this->game_players[i];
        for (Tank *tank : current->tank_list)
        {
            vector<Tank *> potentials = this->collect_game_tanks(tank);
            Tank *target = tank->closest_target(potentials);
            Action action = not_so_smart_move(tank, target, this->game_shells, this->board_w, this->board_h, this->get_board());
            if (this->check_legal_move(action, tank))
            {
                this->apply_action(tank, action);
                this->logger.log_action(action,current->id);
            }
        }
    }
    this->turn_index++;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
vector<vector<Tile>> GameManager::get_board()
{
    return this->game_board;
}

int GameManager::check_legal_move(Action action, Tank *commiter)
{
    switch (action.type)
    {
    case ActionType::Move:
    {
        Tile tile = this->game_board[action.x][action.y];
        pair<int, int> dir_move = get_direction_delta(commiter->entity_dir);
        if (!(commiter->pos_x + dir_move.first == action.x && commiter->pos_y + dir_move.second == action.y))
        {
            return 0;
        }
        if (tile.ground->get_type() == EntityType::Wall)
        {
            return 0;
        }
        return 1;
    }
    case ActionType::Shoot:
    {
        if (commiter->is_reloading())
        {
            return 0;
        }
        return 1;
    }
    default:
        return 1;
    }
    return 0;
}
Entity *test_tiles(vector<Tile *> tiles, LivingEntity *self)
{
    for (Tile *tile : tiles)
    {
        if (tile->actor && tile->actor != self)
            return tile->actor;
        if (tile->projectile && tile->projectile != self)
            return tile->projectile;
        if (tile->ground)
            return tile->ground;
    }
    return nullptr;
}
Entity *GameManager::test_collision(LivingEntity *self, Action action)
{
    int x = wrap_pos(action.x, board_w);
    int y = wrap_pos(action.y, board_h);

    Direction dir = self->entity_dir;

    std::vector<Tile *> tiles_to_check;

    tiles_to_check.push_back(&game_board[y][x]);

    switch (dir)
    {
    case Direction::DL:
        tiles_to_check.push_back(&game_board[y][wrap_pos(x + 1, board_w)]);
        tiles_to_check.push_back(&game_board[wrap_pos(y - 1, board_h)][x]);
        break;
    case Direction::DR:
        tiles_to_check.push_back(&game_board[y][wrap_pos(x - 1, board_w)]);
        tiles_to_check.push_back(&game_board[wrap_pos(y - 1, board_h)][x]);
        break;
    case Direction::UL:
        tiles_to_check.push_back(&game_board[y][wrap_pos(x + 1, board_w)]);
        tiles_to_check.push_back(&game_board[wrap_pos(y + 1, board_h)][x]);
        break;
    case Direction::UR:
        tiles_to_check.push_back(&game_board[y][wrap_pos(x - 1, board_w)]);
        tiles_to_check.push_back(&game_board[wrap_pos(y + 1, board_h)][x]);
        break;
    default:
        break;
    }

    return test_tiles(tiles_to_check, self);
}
void GameManager::apply_action(Entity *e, Action action)
{
    EntityType type = e->get_type();
    if (action.type == ActionType::Move)
    {
        e->update_pos(action.x, action.y);
        return;
    }
    if (action.type == ActionType::Shoot)
    {
        if (Tank *tank = dynamic_cast<Tank *>(e))
        {
            Shell *new_shell = new Shell(-1, e->pos_x, e->pos_y, tank->entity_dir); // TODO add owner
            this->subscribe_shell(new_shell);
            return;
        }
        return;
    }
    if (action.type == ActionType::Stay)
    {
        return;
    }
    if (action.type == ActionType::None)
    {
        return;
    }
    if (action.type == ActionType::Hit)
    {
        if (type == EntityType::Shell)
        {
            auto it = std::find(game_shells.begin(), game_shells.end(), e);
            if (it != game_shells.end())
                game_shells.erase(it);
            return;
        }
        if (type == EntityType::Wall)
        {
            if (Wall *wall = dynamic_cast<Wall *>(e))
            {
                wall->health -= 1;
                if (wall->health == 0)
                {
                    auto it = std::find(game_entities.begin(), game_entities.end(), e);
                    if (it != game_entities.end())
                        game_entities.erase(it);
                    return;
                }
            }
        }
        auto it = std::find(game_entities.begin(), game_entities.end(), e);
        if (it != game_entities.end())
            game_entities.erase(it);
        return;
    }
}

void GameManager::print_result()
{
    std::cout << "Game state: " << game_status_to_string(this->game_state.status) << "\n";
    if (this->game_state.status == GameStatus::Winner)
    {
        std::cout << "Winner: " << this->game_state.winner << "\n";
    }
}

int GameManager::load_game(const std::string &filename)
{
    bool has_errors = false;

    Player *p1 = new Player(1);
    Player *p2 = new Player(2);
    game_players.push_back(p1);
    game_players.push_back(p2);

    std::ofstream error_log("input_errors.txt");
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return -1;
    }
    file >> board_w >> board_h;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    game_board.resize(board_h, vector<Tile>(board_w));
    std::string line;
    int y = 0;
    while (std::getline(file, line))
    {
        if (y >= board_h) {
            has_errors = true;
            error_log << "Extra line beyond declared height at row " << y << ". Ignoring.\n";
            continue;
        }
        if (line.size() < board_w) {
            has_errors = true;
            error_log << "Line " << y << " too short. Padding with spaces.\n";
            line += std::string(board_w - line.size(), ' ');
        } else if (line.size() > board_w) {
            has_errors = true;
            error_log << "Line " << y << " too long. Trimming to board width.\n";
            line = line.substr(0, board_w);
        }

        for (int x = 0; x < std::min((int)line.size(), board_w); ++x)
        {
            char c = line[x];
            switch (c)
            {
            case '#':
            {
                Wall *wall = new Wall(x, y);
                game_entities.push_back(wall);
                game_board[y][x].ground = wall;
                break;
            }
            case '@':
            {
                Mine *mine = new Mine(x, y);
                game_entities.push_back(mine);
                game_board[y][x].ground = mine;
                break;
            }
            case '1':
            case '2':
            {
                Direction dir = (c - '0') == 1 ? Direction::L : Direction::R;
                Tank *tank = new Tank(x, y, dir);
                // tank->set_owner(c - '0'); // store 1 or 2
                game_entities.push_back(tank);
                game_board[y][x].actor = tank;

                if (game_players[c - '1']->tank_list.size() == 1)
                    break;
                game_players[c - '1']->tank_list.push_back(tank);
                break;
            }
            case ' ':
                break;
            default:
                has_errors = true;
                error_log << "Invalid character '" << c << "' at (" << x << "," << y << "). Treated as empty.\n";
                break;
            }
        }
        y++;
    }
    while (y < board_h)
    {
        has_errors = true;
        error_log << "Missing line at row " << y << ", filling with spaces.\n";
        line = std::string(board_w, ' ');
        y++;
    }
    return 0;
}