#include "utils.hpp"

Action get_none_action(){
    Action a;
    a.type = ActionType::None;
    a.x = 0;
    a.y = 0;
    return a;
}
Action get_hit_action(){
    Action a;
    a.type = ActionType::Hit;
    a.x = 0;
    a.y = 0;
    return a;
}