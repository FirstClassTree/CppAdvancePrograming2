#include "utils.hpp"

Logger::Logger(){}
void Logger::log(std::string line){
    std::ofstream file("game_output.txt");
    file << line << std::endl;
}
void Logger::log_action(Action action,int player_id){
    std::ofstream file("game_output.txt");
    file << "Player " << player_id << " did action: " << action_to_string(action) << std::endl;
}

void Logger::log_collision(int e1, int e2){
    std::ofstream file("game_output.txt");
    file << "Entity: " << e1 << " collided with: " << e2 << std::endl;
}
void Logger::log_death(int id){
    std::ofstream file("game_output.txt");
    file << "Entity: " << id << "died!"<< std::endl;
}

void Logger::log_result(GameState state){
    std::ofstream file("game_output.txt");
    file << "Game state: " << game_status_to_string(state.status) << "\n";
    if (state.status == GameStatus::Winner)
    {
        file << "Winner: " << state.winner << "\n";
    }
}