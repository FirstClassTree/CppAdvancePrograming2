#include "game_env.hpp"

GameManager::GameManager() {}

void GameManager::subscribe_entity(Entity *e)
{
    this->game_entities.push_back(e);
}
void GameManager::subscribe_shell(Shell *e)
{
    this->game_shells.push_back(e);
}
void GameManager::tick()
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = game_shells.size() - 1; i >= 0; --i)
        {
            Shell *shell = this->game_shells[i];
            Action action = shell->declare_move();
            Entity *collided = this->test_collision(action.x, action.y);
            if (collided)
            {
                action = shell->colide(collided);
                Action collided_result = collided->colide(shell);
                this->apply_action(collided, collided_result);
            }
            this->apply_action(shell, action);
        }
    }
    for (int i = this->game_players.size() - 1; i >= 0; --i)
    {
        Player* current = this->game_players[i];
        for (Tank *tank : current->tank_list)
        {
            // do tank actions here
        }
    }
}

Entity *GameManager::test_collision(int x, int y)
{
    Tile tile = this->game_board[y][x];
    if (tile.actor)
        return tile.actor;
    if (tile.projectile)
        return tile.projectile;
    return tile.ground;
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
        return; // this needs to be illigal action.
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

int GameManager::load_game(const std::string &filename)
{
    bool has_errors = false;
    game_players.resize(2);

    Player* p1 = new Player(1);
    Player* p2 = new Player(2);
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
    file.ignore();
    game_board.resize(board_h, vector<Tile>(board_w));
    for (int y = 0; y < board_h; ++y)
    {
        std::string line;
        if (!std::getline(file, line))
        {
            // Line missing — fill with spaces
            has_errors = true;
            error_log << "Missing line at row " << y << ", filling with spaces.\n";
            line = std::string(board_w, ' ');
        }

        if (line.size() > board_w)
        {
            has_errors = true;
            error_log << "Line " << y << " too long, trimming extra characters.\n";
            line = line.substr(0, board_w);
        }
        else if (line.size() < board_w)
        {
            has_errors = true;
            error_log << "Line " << y << " too short, padding with spaces.\n";
            line += std::string(board_w - line.size(), ' ');
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
                Tank *tank = new Tank(x, y);
                // tank->set_owner(c - '0'); // store 1 or 2
                game_entities.push_back(tank);
                game_board[y][x].actor = tank;

                if (game_players[c - '1']->tank_list.size() == 1)
                    break;
                game_players[c - '1']->tank_list.push_back(tank);
                break;
            }
            case ' ':
            default:
                break;
            }
        }
    }
    std::string extra;
    int line_num = board_h;
    while (std::getline(file, extra))
    {
        has_errors = true;
        error_log << "Extra line beyond declared height at row " << line_num++ << ". Ignoring.\n";
    }

    return 0;
}