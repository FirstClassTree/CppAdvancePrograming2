#include "utils.hpp"

Logger::Logger(){}
void Logger::log(std::string line){
    std::cout << line << std::endl;
}
void Logger::log_action(Action action,int player_id){
    std::cout << "Player " << player_id << " did action: " << action_to_string(action) << std::endl;
}

void Logger::log_collision(int e1, int e2){
    std::cout << "Entity: " << e1 << " collided with: " << e2 << std::endl;
}
void Logger::log_death(int id){
    std::cout << "Entity: " << id << "died!"<< std::endl;
}