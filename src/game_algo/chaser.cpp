#include "game_algo.hpp"

pair<int, int> find_shortest(int w, int h,
                             int sx, int sy,
                             int gx, int gy)
{
    const int INF = INT_MAX;
    vector<vector<int>> dist(h, vector<int>(w, INF));
    vector<vector<pair<int, int>>> parent(h, vector<pair<int, int>>(w, {-1, -1}));
    queue<pair<int, int>> q;

    dist[sy][sx] = 0;
    q.push({sx, sy});

    static const vector<pair<int, int>> dirs = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    bool found = false;
    while (!q.empty() && !found)
    {
        auto [x, y] = q.front();
        q.pop();
        for (auto [dx, dy] : dirs)
        {
            int nx = wrap_pos(x + dx, w);
            int ny = wrap_pos(y + dy, h);
            if (dist[ny][nx] > dist[y][x] + 1)
            {
                dist[ny][nx] = dist[y][x] + 1;
                parent[ny][nx] = {x, y};
                q.push({nx, ny});
                if (nx == gx && ny == gy)
                {
                    found = true;
                    break;
                }
            }
        }
    }

    if (dist[gy][gx] == INF)
    {
        return {-1, -1};
    }
    vector<pair<int, int>> path;
    for (int cx = gx, cy = gy; !(cx == sx && cy == sy);)
    {
        path.push_back({cx, cy});
        auto [px, py] = parent[cy][cx];
        cx = px;
        cy = py;
    }

    reverse(path.begin(), path.end());

    return path[0];
}

int find_depth(int w, int h,
               vector<vector<bool>> &visited,
               int x, int y,
               int limiter,
               int goal_x, int goal_y)
{

    if (limiter <= 0)
    {
        return INT_MAX;
    }

    if (visited[y][x])
    {
        return INT_MAX;
    }

    if (x == goal_x && y == goal_y)
    {
        return 0;
    }
    visited[y][x] = true;
    int best = INT_MAX;
    static const vector<pair<int, int>> dirs = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (auto [dx, dy] : dirs)
    {
        int nx = wrap_pos(x + dx, w);
        int ny = wrap_pos(y + dy, h);
        int d = find_depth(w, h, visited, nx, ny, limiter - 1, goal_x, goal_y);
        if (d != INT_MAX)
        {
            best = min(best, 1 + d);
        }
    }

    visited[y][x] = false;
    return best;
}

Action chase_enemy_bfs(Tank *self, Tank *target, const vector<vector<Tile>> &board){
    int h = board.size();
    int w = board[0].size();
    pair<int,int> res = find_shortest(w,h,self->pos_x,self->pos_y,target->pos_x,target->pos_y);
    Action result;
    result.type = ActionType::Move;
    result.x = res.first;
    result.y = res.second;

    pair<int, int> delta_dir = get_direction_delta(self->entity_dir);

    if(self->pos_x + delta_dir.first == res.first && self->pos_y + delta_dir.second == res.second){
        return result;
    }
    Direction rot = get_direction_between(self->get_pos(),{self->pos_x + delta_dir.first,self->pos_y + delta_dir.second});
    return rotate_toward(self->entity_dir,rot);
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