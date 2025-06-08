#ifndef TANK_H
#define TANK_H

#include "../MoveableEntity.h"
#include "../TankAlgorithm.h"


// support for the delay neccarry
enum class BackwardState {
    None,
    Waiting1,
    Waiting2,
    ReadyFast
};

class Tank : public MoveableEntity {
public:
  std::unique_ptr<TankAlgorithm> ai;

  Tank(int x, int y, Direction direction, int player_owner, int tank_id,
       std::unique_ptr<TankAlgorithm> ai);
  ~Tank();

  int get_owner_id();
  int get_health();
  int get_tank_id();
  int get_shell_num();

  void set_shell_num(int shell_num);
  void set_tank_id(int tank_id);
  void set_ai(std::unique_ptr<TankAlgorithm> ai);
  TankAlgorithm& get_ai();

  std::string serialize() override;



  void start_backward_sequence();
  void cancel_backward_sequence();
  bool is_ready_to_move_backward() const;
  void advance_backward_state();
  BackwardState get_backward_state() const;
  void damage();





private:
  BackwardState backward_state = BackwardState::None;
  bool cancel_backward = false;
  int health;
  int player_owner;
  int tank_id;
  int shell_num;
};

#endif