#ifndef GAME_WRAPPER_HPP
#define GAME_WRAPPER_HPP

#ifdef __cplusplus
extern "C"
{
#endif

    const char* get_game_board();
    void* create_game();
    void tick(void* game_handler);
    void destroy_game(void* game_handler);

#ifdef __cplusplus
}
#endif

#endif