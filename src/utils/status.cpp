#include <utils.hpp>

std::string game_status_to_string(GameStatus status){
    switch(status){
        case GameStatus::Idle: return "Idle";
        case GameStatus::Running: return "Running";
        case GameStatus::Tie: return "Tie";
        case GameStatus::Winner: return "Winner";
        default: return "unknown";
    }
}