#include <game_algo.hpp>

Action not_so_smart_move(Tank *self, Tank *target)
{
}
Action is_targeted(Tank *self, const vector<Shell *> &shells, int w, int h)
{
    const auto danger_tiles = get_shell_danger_zones(shells, w, h);
    bool near_danger[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(danger_tiles.count({self->pos_x-1+i,self->pos_y-1+j})){
                near_danger[i][j] = true;
            }
        }
    }
    if(!near_danger[1][1]){
        return Action{ActionType::None};
    }
    if(near_danger[1][1]){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(!near_danger[i][j] &! (i==1 && j==1)){
                    return Action{ActionType::Move,self->pos_x-1+i,self->pos_y-1+j};
                }
            }
        }
    }
}

bool on_line_of_sight(LivingEntity *self, LivingEntity *target)
{
    if (get_direction_between(self->get_pos(), target->get_pos()) == self->entity_dir)
    {
        return true;
    }
    return false;
}

Action rotate_toward(Direction from, Direction to)
{
    auto mod8 = [](int x)
    { return (x % 8 + 8) % 8; };

    int fi = static_cast<int>(from);
    int ti = static_cast<int>(to);

    int diff_pos = mod8(ti - fi);
    int diff_neg = mod8(fi - ti);
    if (diff_pos == 0)
        return get_none_action();

    if (diff_pos < diff_neg)
    {
        if (diff_pos == 1)
        {
            return Action{ActionType::RotateR1};
        }
        else
        {
            return Action{ActionType::RotateR2};
        }
    }
    else
    {
        if (diff_neg == 1)
        {
            return Action{ActionType::RotateL1};
        }
        else
        {
            return Action{ActionType::RotateL2};
        }
    }
}

set<pair<int, int>> get_shell_danger_zones(const vector<Shell *> &shells, int w, int h)
{
    set<pair<int, int>> danger;

    for (Shell *shell : shells)
    {
        Pos pos = shell->get_pos();
        auto [dx, dy] = get_direction_delta(shell->entity_dir);

        for (int i = 1; i <= 2; ++i)
        {
            pos.x = wrap_pos(pos.x + dx*i, w);
            pos.y = wrap_pos(pos.y + dy*i, h);
            danger.insert({pos.x, pos.y});
        }
    }

    return danger;
}