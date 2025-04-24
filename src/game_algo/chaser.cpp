#include "game_algo.hpp"

int find_depth(int w, int h,
    vector<vector<bool>> &visited,
    int x, int y,
    int limiter,
    int goal_x, int goal_y)
{

if (visited[y][x])
return limiter;

if (x == goal_x && y == goal_y)
return 1;

visited[y][x] = true;

int min_val = limiter;
vector<std::pair<int, int>> directions = {
{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

for (auto [dx, dy] : directions)
{
int depth = find_depth(w, h, visited, wrap_pos(x + dx, w), wrap_pos(y + dy, h), limiter, goal_x, goal_y);
min_val = std::min(min_val, 1 + depth);
}

visited[y][x] = false;
return min_val;
}



Action chase_enemy(Tank *self, Tank *target, const vector<vector<Tile>> &board)
{
    int h = board.size();
    int w = board[0].size();
    int min_value = INT_MAX;

    Pos self_pos = self->get_pos();
    Pos target_pos = target->get_pos();

    Direction min_dir;
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            if (board[y][x].ground || board[y][x].actor)
            {
                visited[y][x] = true;
            }
        }
    }
    const std::vector<std::pair<Direction, std::pair<int, int>>> direction_map = {
        {Direction::U, {0, -1}},
        {Direction::UR, {1, -1}},
        {Direction::R, {1, 0}},
        {Direction::DR, {1, 1}},
        {Direction::D, {0, 1}},
        {Direction::DL, {-1, 1}},
        {Direction::L, {-1, 0}},
        {Direction::UL, {-1, -1}},
    };
    for (auto [dir, pos] : direction_map)
    {
        int nx = wrap_pos(self_pos.x + pos.first, w);
        int ny = wrap_pos(self_pos.y + pos.second, h);

        if (visited[ny][nx])
            continue;

        int candidate = find_depth(w, h, visited, nx, ny, h * w, target_pos.x, target_pos.y);
        if (candidate < min_value)
        {
            min_value = candidate;
            min_dir = dir;
        }
    }
    if (min_value == INT_MAX)
    {
        return get_none_action();
    }
    Action result;
    result.type = ActionType::Move;
    pair<int, int> delta_dir = get_direction_delta(min_dir);
    result.x = wrap_pos(self_pos.x + delta_dir.first, w);
    result.y = wrap_pos(self_pos.y + delta_dir.second, h);
    return result;
}