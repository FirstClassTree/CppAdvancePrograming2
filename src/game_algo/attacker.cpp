#include <game_algo.hpp>

Action not_so_smart_move(){
    
}

Action rotate_toward(Direction from, Direction to)
{
    auto mod8 = [](int x) { return (x % 8 + 8) % 8; };


    int fi = static_cast<int>(from);
    int ti = static_cast<int>(to);

    int diff_pos = mod8(ti - fi);
    int diff_neg = mod8(fi - ti);
    if (diff_pos == 0)
        return get_none_action();

    if (diff_pos < diff_neg)
    {
        if (diff_pos ==1){
            return Action{ActionType::RotateR1};
        } else {
            return Action{ActionType::RotateR2};
        }
    } else {
        if (diff_neg ==1){
            return Action{ActionType::RotateL1};
        } else {
            return Action{ActionType::RotateL2};
        }
    }
}