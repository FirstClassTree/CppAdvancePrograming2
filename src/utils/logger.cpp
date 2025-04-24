#include "utils.hpp"

Logger::Logger(){}
void Logger::log(std::string line){
    std::cout << line << std::endl;
}
void Logger::log_action(Action action,int player_id){
    std::cout << "Player " << player_id << " did action: " << action_to_string(action) << std::endl;
}