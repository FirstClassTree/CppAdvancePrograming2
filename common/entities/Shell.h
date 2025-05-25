#ifndef SHELL_H
#define SHELL_H

#include "../MoveableEntity.h"

class Shell : public MoveableEntity {
public:
    Shell(int x, int y, Direction direction);
    ~Shell();

    std::string serialize() override;


};

#endif