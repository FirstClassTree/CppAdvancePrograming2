#ifndef SHELL_H
#define SHELL_H

#include "../MoveableEntity.h"

class Shell : public MoveableEntity {
public:
    Shell(int x, int y, Direction direction);
    ~Shell();

    void destroy();
    bool is_destroyed() const;

    std::string serialize() override;

private:
    bool destroyed = false;
};

#endif
