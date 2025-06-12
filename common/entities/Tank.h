#ifndef TANK_H
#define TANK_H

#include "../MoveableEntity.h"
#include "../TankAlgorithm.h"
#include "../Constants.h"


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

  int get_owner_id() const;
  int get_health() const;
  int get_tank_id() const;
  int get_shell_num() const ;
int get_cooldown() const ;
  TankAlgorithm& get_ai() const;

  void set_shell_num(int shell_num);
  void set_tank_id(int tank_id);
  void set_ai(std::unique_ptr<TankAlgorithm> ai);

  std::string serialize() override;



  void start_backward_sequence();
  void cancel_backward_sequence();
  bool is_ready_to_move_backward() const;
  void advance_backward_state();
  BackwardState get_backward_state() const;
  void damage();


  bool can_shoot() const;
  void mark_shot();
  void tick_cooldown();
  int get_all_tank_index() const;







private:
  BackwardState backward_state = BackwardState::None;
  bool cancel_backward = false;
  int health;
  int player_owner;
  int tank_id;
  int shell_num;
  int shoot_cooldown = 0;
  static int tank_count;
  int all_tank_index;
  
};

#endif